#pragma once
#include "InstanceObject.h"

class InstanceGroup : public Object {
public:
	unsigned int id;

	std::vector<std::shared_ptr<InstanceObject>> instances;
	InstanceGroup(const std::string& _name = "Unnamed InstanceGroup");

	void draw();

	void addInstance(std::shared_ptr<InstanceObject> instance); // Add instance to group
	std::shared_ptr<InstanceObject> addInstance(const std::string& name); // Create instance with name and add to group

	void removeInstance(std::shared_ptr<InstanceObject> instance); // Remove instance from list, meaning it is no longer rendered

	std::vector<std::shared_ptr<InstanceObject>>* getInstances(); // Get all instances in group
	std::shared_ptr<InstanceObject> getInstanceById(unsigned int id); // Get instance by Relative id
	std::vector<std::shared_ptr<InstanceObject>> getInstancesByName(std::string name); // Get all instances with a given name

private:
	void setupModelBuffer(); // Setup model buffer for instances, reserve space

	void batchResendModels(); // Resend all model matrices from instances to model buffer

	void updateInstances();
	void resizeModelBuffer(); // Increase model buffer size by modelBufferIncrement
	
private:
	unsigned int modelBufferIncrement = 1000; // How much to increase model buffer size by when necessary
	unsigned int nextModelBufferIndex = 0; // Next free index in the model buffer
	unsigned int modelBufferSize = 1000; // Size of model buffer (in matrices)
	unsigned int nextInstanceId = 0; // Next free instance id
};
