#include "InputKeeper.h"

InputKeeper InputKeeper::InputKeeperInstance = InputKeeper();

InputKeeper::InputKeeper()
{
	
}


InputKeeper::~InputKeeper()
{
}

void InputKeeper::updateKeys()
{
	lastMousePos = mousePos;
	for (std::unordered_map<unsigned int, bool>::iterator ite = currentKeys.begin(); ite != currentKeys.end(); ite++) {
		lastKeys[ite->first] = ite->second;
	}
}

void InputKeeper::addPressKey(const uint32_t& SDL_KEY)
{
	currentKeys[SDL_KEY] = true;
}

 void InputKeeper::addReleaseKey(const uint32_t & SDL_KEY)
 {
	 currentKeys[SDL_KEY] = false;
 }


void InputKeeper::setMousePosition(const uint32_t& x, const uint32_t& y)
{
	mousePos.insert(static_cast<float>(x), 0);
	mousePos.insert(static_cast<float>(y), 1);
}

 bool InputKeeper::isKeyPressed(const uint32_t & SDL_KEY)
{
	
	return(checkCurrent(SDL_KEY) && !checkLast(SDL_KEY));
}

 bool InputKeeper::isKeyHeld(const uint32_t & SDL_KEY)
{
	return(checkCurrent(SDL_KEY) && checkLast(SDL_KEY));
}

bool InputKeeper::isKeyReleased(const uint32_t & SDL_KEY)
 {
	 return(!checkCurrent(SDL_KEY) && checkLast(SDL_KEY));
 }

 
Vec4f InputKeeper::getMouseOffset() {
	 return   Vec4f(mousePos - lastMousePos); }

Vec4f InputKeeper::getMousePosition()
{
	return mousePos;
}


bool InputKeeper::checkCurrent(const uint32_t& SDL_KEY)
{
	std::unordered_map<unsigned int, bool>::iterator ite = currentKeys.find(SDL_KEY);
	if (ite != currentKeys.end())return ite->second;
	return false;
}

bool InputKeeper::checkLast(const uint32_t& SDL_KEY)
{
	std::unordered_map<unsigned int, bool>::iterator ite = lastKeys.find(SDL_KEY);
	if (ite != lastKeys.end())return ite->second;
	return false;
}
