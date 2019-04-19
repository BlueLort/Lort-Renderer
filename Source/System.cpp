#include "System.h"

System::System() : running(true)
{
	initSDL();
	inputKeeper=InputKeeper::getInstance();
	LortRenderer = Renderer::getInstance();
	LortRenderer->setScreenWidth(SCR_WIDTH);
	LortRenderer->setScreenHeight(SCR_HEIGHT);
	LortRenderer->initZBuffer();
	LortRenderer->setWindow(window);
	LortRenderer->setSurface(surface);
	LortRenderer->setClearColor(0xff000000);
	LortRenderer->updateHalfScrSize();

}


System::~System()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

std::shared_ptr<System> System::getInstance()
{
	static std::shared_ptr<System> instance = nullptr;
	if (!instance) {
		instance = std::make_unique<System>();
	}
	return instance;
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
	projection = Mat4x4f::getPerspective(
		1.22173f,
		static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT)
		, 0.1f
		, 100.0f);

	while (running) {
		dTime = getDeltaTime();
		pollInputs();
		processInputs(dTime);
	   //printf("FPS:%f\n", 1 / dTime);
		render();
		update();
	}
}
inline float System::getDeltaTime()const
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

void System::processInputs(const float deltaTime)
{
	
	if (inputKeeper->isKeyPressed(SDLK_ESCAPE)) {
		running = false;
	}
	if (inputKeeper->isKeyHeld(SDLK_w)) {
		Camera::getInstance().ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	if (inputKeeper->isKeyHeld(SDLK_s)) {
		Camera::getInstance().ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (inputKeeper->isKeyHeld(SDLK_a)) {
		Camera::getInstance().ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	if (inputKeeper->isKeyHeld(SDLK_d)) {
		Camera::getInstance().ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}



	if (inputKeeper->isKeyHeld(SDLK_UP)) {
		Camera::getInstance().ProcessMouseMovement(0.0f, 512.0f*deltaTime);
	}
	if (inputKeeper->isKeyHeld(SDLK_DOWN)) {
		Camera::getInstance().ProcessMouseMovement(0.0f, -512.0f*deltaTime);
	}
	if (inputKeeper->isKeyHeld(SDLK_LEFT)) {
		Camera::getInstance().ProcessMouseMovement(-512.0f*deltaTime, 0.0f);
	}
	if (inputKeeper->isKeyHeld(SDLK_RIGHT)) {
		Camera::getInstance().ProcessMouseMovement(512.0f*deltaTime, 0.0f);
	}

}

void System::render()
{

	LortRenderer->clearScreen();

	static float counter = 0;
	counter += dTime;
	Mat4x4f VP = projection * Camera::getInstance().getViewMatrix();

	Mat4x4f modelMat = Mat4x4f::getTranslation(0.0f, 0.0f, -6.0f);
	modelMat = modelMat * Mat4x4f::getRotation(0.0f, counter, 0.0f);
	model->setVP(VP);
	model->setModel(modelMat);
	model->render(LortRenderer);
	modelMat = Mat4x4f();
	VP = projection * Camera::getInstance().getViewMatrix();
	plane->setVP(VP);
	plane->render(LortRenderer);
	LortRenderer->updateScreen();
}



void System::update()const
{
	inputKeeper->updateKeys();
}
