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
	mousePos.arr[0] = (float)x;
	mousePos.arr[1] = (float)y;
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

 
 VEC2 InputKeeper::getMouseOffset() {
	 return   VEC2(mousePos - lastMousePos); }

VEC2 InputKeeper::getMousePosition()
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
