#include "Renderer.h"

Renderer::Renderer(ObjectManager* objManager, unsigned int scr_width, unsigned int scr_height) :
    shader(Shader("shaders/shader.vs", "shaders/shader.fs")),
    projection(glm::mat4(1.0f)),
    model(glm::mat4(1.0f)),
    objectManager(objManager),
    SCR_WIDTH(scr_width),
    SCR_HEIGHT(scr_height)
{
    objectsptr = objectManager->getObjects();
    renderedObjects = {};
    indexOffset = 0;
    // Setup globally applied matrices
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // Using EBO buffers, so vertices can be reused
    // Indices are always needed with vertices in this approach.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);

    // Setup shader
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", glm::mat4(1.0f));
    shader.setMat4("projection", projection);
}

void Renderer::setCamera(Camera* camera) {
    globalCamera = camera;
    view = &(globalCamera->view);
}

void Renderer::render() {
    glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     if (view) {
        shader.setMat4("view", *view);
    }

    /*
    Explanation of how objects are rendered:
    object -> vertices = a vector of vertices 
	eg A -> vertices = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}}
    object -> indices = a vector of indices, where an index points to a vertex, indicating that a vertex is being reused.
	eg A -> indices = {0, 1, 2, 1, 2, 3} ( a square )

    firstIndices -> a vector where, if we input the index i-1 of an object, it outputs the position of the first index of that object inside the combinedIndices vector.
	eg firstIndices = {0, 4, 6} means obj1 begins at index 0, obj2 begins at index 4, so on

	firstVertices -> a vector where, if we input the index i-1 of an object, it outputs the position of the first index of that object inside the combinedVertices vector.
	eg firstVertices = {0, 4, 6} means obj1 begins at index 0, obj2 begins at index 4, so on

	numIndices -> a vector where, if we input the index i-1 of an object, it outputs the number of indices of that object.

	numVertices -> a vector where, if we input the index i-1 of an object, it outputs the number of vertices of that object.

	combinedVertices -> a vector of all vertices of all objects
    stored as {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}, {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}} (IT CAN REPEAT!)

	combinedIndices -> a vector of all indices of all objects
    these point to vertices inside combinedVertices. Obviously, for distinct separate objects, these indices should not repeat through multiple objects.
    Therefore, when an object is rendered, it must be added to combinedIndices with an offset so indices cannot overlap.

    When an object is rendered:
    - we add the amount of indices currently rendered as an element of firstIndices
    so if 0 indices rendered, firstIndices = {0}
	- we add the amount of vertices currently as an element of firstVertices
	so if 0 vertices rendered, firstVertices = {0}
	- we add the amount of indices of the object as an element of numIndices
	so object A would have numIndices = {6}
	- we add the amount of vertices of the object as an element of numVertices
	so object A would have numVertices = {4}

	- we add the vertices of the object to combinedVertices
    as there are no objects, it would just be {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}}

	- we add the indices of the object to combinedIndices, with an offset
	as there are no objects, it would just be {0, 1, 2, 1, 2, 3}

	////////////////////////////////////////////
    suppose we want to add another object B, which has the same vertices and indices as A
    NOW:
    firstIndices = {0, 6}
	firstVertices = {0, 4}
    numIndices = {6, 6}
	numVertices = {4, 4}
	combinedVertices = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}, {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}}

	combinedIndices = vertices, OFFSET+vertices ->
	combinedIndices = {0, 1, 2, 1, 2, 3,(OFFSET, last elem in firstVertices=4) 4, 5, 6, 5, 6, 7}

	////////////////////////////////////////////
    another object C, the same vertices and indices again
	NOW:
	firstIndices = {0, 6, 12}
	firstVertices = {0, 4, 8}
	numIndices = {6, 6, 6}
	numVertices = {4, 4, 4}
	combinedVertices = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}, {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}, {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}} 
	combinedIndices = {0, 1, 2, 1, 2, 3, (OFFSET, last elem in firstVertices=4) 4, 5, 6, 5, 6, 7, (OFFSET, last elem in firstVertices=8) 8, 9, 10, 9, 10, 11}
    */


	 bool largeRender = false;
	 bool smallRender = false;

	 objects = *(objectsptr);


	 // If objects were added or removed, render or unrender them
	 if (objectManager->addedObjects.size() > 0) {
		 largeRender = true;

		 // For each added object, render it
		 for (auto& obj : objectManager->addedObjects) {
			  renderObject(obj);
	
		 }
		 objectManager->addedObjects.clear();
	 }

	 if (objectManager->removedObjects.size() > 0) {
		 largeRender = true;

		 // For each added object, unrender it
		 for (auto& obj : objectManager->removedObjects) {
			 unrenderObject(obj);
		 }
		 objectManager->removedObjects.clear();
	 }
	 
	 // If objects were transformed, update their vertices
	 float transformedSize = transformedObjects.size();
	 if (transformedSize > 0) {
		 smallRender = true;

		 if (transformedSize >= objects.size()/2) { // Redo all vertices if more than half of all objects were transformed. This is faster
			 redoVertices();
		 }
		 else { // Otherwise, just redo the vertices of each transformed object individually
			 for (auto& obj : transformedObjects) {
				 redoObjVertices(obj);
			 }
		 }
		 transformedObjects.clear();
	 }

	 if (vertChangedObjects.size() > 0) {
		 largeRender = true;

		 // Completely remove + re render each object that had its vertices changed.
		 for (auto& obj : vertChangedObjects) {
			 redoObj(obj);
		 }
		 vertChangedObjects.clear();
	 }

    glBindVertexArray(VAO);

	if (smallRender && !largeRender) { // Smaller, vertice-only update.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, combinedVertices.size() * sizeof(glm::vec3), combinedVertices.data());
	}
	else if (largeRender) { // Larger update, resending all data.
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, combinedVertices.size() * sizeof(glm::vec3), combinedVertices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, combinedIndices.size() * sizeof(unsigned int), combinedIndices.data(), GL_DYNAMIC_DRAW);
	}

    for (size_t i = 0; i < numIndices.size(); ++i) {
		GameObject* obj = (objects)[i];
        if (obj->visible) { // ONLY draw if object is visible
            glDrawElements(GL_TRIANGLES, numIndices[i], GL_UNSIGNED_INT, (void*)(firstIndices[i] * sizeof(unsigned int)));
        }
    }
    glBindVertexArray(0);
}

// Adds object to renderedObjects list.
// Setups the object's vertices and indices in the combinedVertices and combinedIndices vectors.
// Also sets up the object's position in the firstIndices, firstVertices, numIndices, and numVertices vectors.
void Renderer::renderObject(GameObject* obj) {
	// Get vertices and indices. May have to change this later to get vertices as references
	const auto& verts = obj->getVertices();
	const auto& inds = obj->getIndices();

	renderedObjects.push_back(obj); // Add object to renderedObjects

	firstIndices.push_back(static_cast<GLint>(combinedIndices.size())); // Push back the starting index of the object in combinedIndices
	firstVertices.push_back(static_cast<GLint>(combinedVertices.size())); // Push back the starting index of the object in combinedVertices
	numIndices.push_back(static_cast<GLsizei>(inds.size())); // Push back the number of indices of the object
	numVertices.push_back(static_cast<GLsizei>(verts.size())); // Push back the number of vertices of the object

	combinedVertices.insert(combinedVertices.end(), verts.begin(), verts.end()); // Add vertices to flat vector 
	for (const auto& ind : inds) {
		combinedIndices.push_back(ind + firstVertices.back()); // Add offset (firstVertices.back()) to index and push back, for each index.
	}
}

// Removes object from renderedObjects list
// Removes object's vertices and indices from combinedVertices and combinedIndices vectors
// Also removes object's position in the firstIndices, firstVertices, numIndices, and numVertices vectors, and updates positions of next objects.
void Renderer::unrenderObject(GameObject* obj) {
	auto it = std::find(renderedObjects.begin(), renderedObjects.end(), obj);
	if (it == renderedObjects.end()) {
		// Object not found in renderedObjects, cannot unrender something that was not rendered in the first place!
		std::cout << "Unrendering of object that was never rendered...?" << std::endl;
		return;
	}

	// Get place of object in renderedObjects, to find things in other vectors
	size_t place = std::distance(renderedObjects.begin(), it);

	const auto& verts = obj->getVertices();
	const auto& inds = obj->getIndices();

	GLint firstIndexPosition = firstIndices[place]; // position of first index of object in combinedIndices
	GLint firstVertexPosition = firstVertices[place]; // position of first vertex of object in combinedVertices
	GLsizei numIndex = numIndices[place]; // amount of indices of object
	GLsizei numVertex = numVertices[place]; // amount of vertices of object

	renderedObjects.erase(it); // Erases from renderedObjects

	firstIndices.erase(firstIndices.begin() + place); // Erases from firstIndices
	// update firstIndices, so that the first index of all the next objects is correct
	for (size_t i = place; i < firstIndices.size(); ++i) {
		firstIndices[i] -= numIndex;	
	}

	firstVertices.erase(firstVertices.begin() + place); // Erases from firstVertices
	// update firstVertices, so that the first index of all the next objects is correct
	for (size_t i = place; i < firstVertices.size(); ++i) {
		firstVertices[i] -= numVertex;
	}

	numIndices.erase(numIndices.begin() + place); // Erases from numIndices
	numVertices.erase(numVertices.begin() + place); // Erases from numVertices

	combinedVertices.erase(combinedVertices.begin() + firstVertexPosition, combinedVertices.begin() + firstVertexPosition + numVertex); // Erases vertices from combinedVertices
	combinedIndices.erase(combinedIndices.begin() + firstIndexPosition, combinedIndices.begin() + firstIndexPosition + numIndex); // Erases indices from combinedIndices

	// update relative combinedIndices, so that the indices of all the next objects are correct
	for (size_t i = firstIndexPosition; i < combinedIndices.size(); ++i) {
		combinedIndices[i] -= numVertex;
	}
}

// Redo combinedVertices vector, re-inserting all object vertices.
void Renderer::redoVertices() {
	combinedVertices.clear();
	for (size_t i = 0; i < objects.size(); ++i) {
		GameObject* obj = (objects)[i];
		const auto& verts = obj->getVertices();
		combinedVertices.insert(combinedVertices.end(), verts.begin(), verts.end());

		renderedObjects = objects; // UNSAFE? Depends on the position this is run in
	}
}

// Redo vertices of a specific object, erasing old vertices and re-inserting new ones into combinedVertices vector.
void Renderer::redoObjVertices(GameObject* object) {
	auto it = std::find(renderedObjects.begin(), renderedObjects.end(), object);
	if (it == renderedObjects.end()) {
		// Object not found in objects, cannot update vertices of something that does not exist!
		std::cout << "Updating vertices of object that does not exist...?" << std::endl;
		return;
	}

	size_t place = std::distance(renderedObjects.begin(), it);
	const auto& verts = object->getVertices();
	GLint firstVertexPosition = firstVertices[place];
	GLsizei numVertex = verts.size();

	combinedVertices.erase(combinedVertices.begin() + firstVertexPosition, combinedVertices.begin() + firstVertexPosition + numVertex);
	combinedVertices.insert(combinedVertices.begin() + firstVertexPosition, verts.begin(), verts.end());
}

// Completely wipes everything and re-renders all objects.
void Renderer::redoAll() {
	combinedVertices.clear();
	combinedIndices.clear();
	firstIndices.clear();
	firstVertices.clear();
	numIndices.clear();
	numVertices.clear();

	renderedObjects.clear();
	for (size_t i = 0; i < objects.size(); ++i) {
		GameObject* obj = (objects)[i];
		renderObject(obj);
	}
}

// Unrenders and re-renders an object entirely.
void Renderer::redoObj(GameObject* object) {
	auto it = std::find(renderedObjects.begin(), renderedObjects.end(), object);
	if (it == renderedObjects.end()) {
		// Object not found in objects, cannot update vertices of something that does not exist!
		std::cout << "Updating vertices of object that does not exist...?" << std::endl;
		return;
	}

	unrenderObject(object);
	renderObject(object);
}