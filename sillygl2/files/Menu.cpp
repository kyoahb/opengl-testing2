#include "Menu.h"

Menu::Menu(std::function<void()> _showFunction, std::string _name) : showFunction(_showFunction), name(_name), visible(false){

}

void Menu::show() {
	visible = true;
}

void Menu::hide() {
	visible = false;
}