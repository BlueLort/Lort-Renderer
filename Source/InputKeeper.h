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
	Vec4f getMouseOffset();
	Vec4f getMousePosition();
	bool isKeyPressed(const uint32_t& SDL_KEY);
	bool isKeyHeld(const uint32_t& SDL_KEY);
	bool isKeyReleased(const uint32_t& SDL_KEY);
	
private:
	Vec4f mousePos;
	Vec4f lastMousePos;
	static InputKeeper InputKeeperInstance;

	std::unordered_map<uint32_t, bool> currentKeys;
	std::unordered_map<uint32_t, bool> lastKeys;
    bool checkCurrent(const uint32_t& SDL_KEY);
    bool checkLast(const uint32_t&  SDL_KEY);

};

