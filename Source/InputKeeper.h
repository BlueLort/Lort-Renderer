#pragma once
#include <unordered_map>
#include "Structures.h"
class InputKeeper
{
public:
	InputKeeper();
	~InputKeeper();
	void updateKeys();//update last keys with current keys 

	static InputKeeper* getInstance() { return &InputKeeperInstance; }
	void addPressKey(const uint32_t& SDL_KEY);
	void addReleaseKey(const uint32_t& SDL_KEY);

	void setMousePosition(const uint32_t& x, const uint32_t& y);
	VEC2 getMouseOffset();
	VEC2 getMousePosition();
	 bool isKeyPressed(const uint32_t& SDL_KEY);
	 bool isKeyHeld(const uint32_t& SDL_KEY);
	 bool isKeyReleased(const uint32_t& SDL_KEY);
	
private:
	VEC2 mousePos=VEC2(0.0f,0.0f);
	VEC2 lastMousePos = VEC2(0.0f, 0.0f);
	static InputKeeper InputKeeperInstance;

	std::unordered_map<uint32_t, bool> currentKeys;
	std::unordered_map<uint32_t, bool> lastKeys;
    bool checkCurrent(const uint32_t& SDL_KEY);
    bool checkLast(const uint32_t&  SDL_KEY);

};

