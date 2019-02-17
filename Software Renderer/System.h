#pragma once
#include <SDL\SDL.h>
#include <iostream>
#include <string>
#include "InputKeeper.h"
#include "Renderer.h"
#include "Structures.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspect
#include <random>
const uint16_t    SCR_WIDTH = 1024;
const uint16_t    SCR_HEIGHT = 768;
const std::string SCR_TITLE = "Lort Software Renderer";
#define err(message)  printf(message);system("pause");
#define fatalErr(message)  printf(message);system("pause");exit(-1);

class System
{
public:

	System();
	~System();
	void destroy();//destroy any reserved memory by the system
	static System* getInstance();//get the instance of the system
	void Run();
private:
	//Functions
	void initSDL();
	float getDeltaTime();
	void pollInputs();
	void processInputs();
	void render();
	void update();

	//Members
	bool           running ;
	float		   dTime;
	MAT4		   projection;
	static System* systemInstance;//singleton 
	SDL_Window*    window=nullptr;//points to screenwindow
	SDL_Surface*   surface = nullptr;//points to screen surface
	InputKeeper*   inputKeeper = nullptr;//input keeper class that keeps press,held,released keys
	Renderer*      LortRenderer;


};

