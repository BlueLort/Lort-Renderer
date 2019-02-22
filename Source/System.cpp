#include "System.h"

System* System::systemInstance = nullptr;

System::System() : running(true)
{
	initSDL();
	inputKeeper=InputKeeper::getInstance();
	LortRenderer = Renderer::getInstance();
	LortRenderer->setScreenWidth(SCR_WIDTH);
	LortRenderer->setScreenHeight(SCR_HEIGHT);
	LortRenderer->setWindow(window);
	LortRenderer->setSurface(surface);
	LortRenderer->setClearColor(Color(0, 0, 0, 255));
	LortRenderer->updateHalfScrSize();

}


System::~System()
{
	destroy();
	delete this->systemInstance;
}

void System::destroy()
{

	SDL_DestroyWindow(window);
	SDL_Quit();


}

System * System::getInstance()
{
	if (systemInstance == nullptr) {
		systemInstance = new System();
	}
	return systemInstance;
}

void System::initSDL()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(SCR_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH, SCR_HEIGHT,	 NULL);
	if (window == nullptr) {
		fatalErr("FAILED TO INIT WINDOW!!\n")
	}
	surface = SDL_GetWindowSurface(window);
	if (surface == nullptr) {
		fatalErr("FAILED TO INIT SURFACE!!\n")
	}
	//Hide the Mouse Cursor
	//SDL_ShowCursor(SDL_DISABLE);

}

void System::Run()
{
	uint32_t mod = 0;
	projection = MAT4::getPerspective(
		70.0f,
		static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT)
		, 0.1f
		, 1000.0f);

	while (running) {
		dTime = getDeltaTime();
		mod++;
		pollInputs();
		processInputs();
		if (mod == 128) {//print deltatime once each 128 frame
			printf("FPS:%f\n", 1 / dTime);
			mod = 0;
		}
		render();
		update();
	}
	this->destroy();//destroy the program
}
float System::getDeltaTime()
{
	static uint32_t lastTime = 0;
	uint32_t elapsed = SDL_GetTicks() - lastTime;
	lastTime = lastTime + elapsed;
	return ((float)elapsed / 1000.0f);
}


void System::pollInputs()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			inputKeeper->addPressKey(e.key.keysym.sym);
			break;
		case SDL_KEYUP:
			inputKeeper->addReleaseKey(e.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputKeeper->addPressKey(e.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			inputKeeper->addReleaseKey(e.button.button);
			break;
		case SDL_MOUSEMOTION:
			inputKeeper->setMousePosition(e.motion.x, e.motion.y);
			break;

		}

	}
}

void System::processInputs()
{

}

void System::render()
{

	LortRenderer->clearScreen();

	static float counter = 0;
	counter += dTime;
	
	MAT4 model= MAT4::getTranslation(0.0f, 0.0f, 3.0f);
	model = model* MAT4::getRotation(0.0f,counter,0.0f);
	//TODO VIEW MATRIX
	MAT4 MVP = projection * model;

	//Vertex v1=Vertex(-0.5f, -0.5f, 0.0f ,1.0f,0.0f , static_cast<uint32_t>(0x00ff00ff)).Transform(MVP);
	//Vertex v2=Vertex(0.0f , 0.5f , 0.0f ,1.0f,1.0f, static_cast<uint32_t>(0xff0000ff)).Transform(MVP);
	//Vertex v3=Vertex(0.5f , -0.5f , 0.0f,0.0f,0.0f , static_cast<uint32_t>(0x0000ffff)).Transform(MVP);
	Vertex v1 = Vertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, static_cast<uint32_t>(0x00ff00ff)).Transform(MVP);
	Vertex v2 = Vertex(0.0f, 1.0f, 0.0f, 0.5f, 1.0f, static_cast<uint32_t>(0xff0000ff)).Transform(MVP);
	Vertex v3 = Vertex(1.0f, -1.0f, 0.0f, 1.0f, 0.0f, static_cast<uint32_t>(0x0000ffff)).Transform(MVP);
	LortRenderer->drawTriangle(v1, v2, v3);
	LortRenderer->updateScreen();
}



void System::update()
{
	inputKeeper->updateKeys();
}
