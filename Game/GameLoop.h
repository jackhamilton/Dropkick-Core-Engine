#pragma once
#include <list>
#include "Scene.h"
#include "World.h"

class Input;

class GameLoop {
public:
	GameLoop(World* world, Input* input) : GameLoop(60, world, input) {};
	GameLoop(int fps, World* world, Input* input);
	void start();
	void stop();
	void setInput(Input* input);
	void setWorld(World* world);
	bool running;
	void destroy();
private:
	Input* input;
	int frame;
	int fps;
	vector<int> cFPS;
	vector<int> cPotentialFPS;
	double frameTimeMS;
	World* world;
};
