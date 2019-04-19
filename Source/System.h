#pragma once
#include <SDL\SDL.h>
#include <iostream>
#include <string>
#include <memory>
#include "InputKeeper.h"
#include "Renderer.h"
#include "Structures.h"
#include "Camera.h"
#include "Mesh.h"
#include "Plane.h"
const uint32_t    SCR_WIDTH = 1024;
const uint32_t    SCR_HEIGHT = 768;
const std::string SCR_TITLE = "Lort Software Renderer";
#define err(message)  printf(message);system("pause");
#define fatalErr(message)  printf(message);system("pause");exit(-1);

class System
{
public:
	System();
	~System();
	static std::shared_ptr<System> getInstance();//get the instance of the system
	void Run();
private:
	//Functions
	void initSDL();
	float getDeltaTime()const;
	void pollInputs();
	void processInputs(const float deltaTime);
	void render();
	void update()const;

	//Members
	bool           running ;
	float		   dTime;
	Mat4x4f		   projection;
	SDL_Window*    window=nullptr;//points to screenwindow
	SDL_Surface*   surface = nullptr;//points to screen surface
	InputKeeper*   inputKeeper = nullptr;//input keeper class that keeps press,held,released keys
	Renderer*     LortRenderer;


	std::unique_ptr<Mesh> model = std::make_unique<Mesh>(std::string("res/MonkeyH.obj"));
	std::unique_ptr<Plane> plane = std::make_unique<Plane>(std::string("res/hMap.png"));

};

