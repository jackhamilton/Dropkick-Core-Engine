#pragma once
#include <SDL2/SDL.h>
#include <thread>
#include <vector>
#include <map>

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

class GameLoop;

class Input {
public:
	Input();
	void addKeyboardEvent(std::function<void()> func, SDL_EventType eventType, std::vector<SDL_Keycode> keys);
	void handleInput(GameLoop* gameLoop);
private:
	// Saves the state(true=pressed; false=released) of each SDL_Key
	std::map<SDL_Keycode, bool> keyboard; 
	//a map of all keyboard events and functions to call
	std::map<std::pair<SDL_Keycode, SDL_EventType>, Callback> keyboardEventMap;
};
