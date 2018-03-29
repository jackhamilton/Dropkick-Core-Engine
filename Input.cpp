#include "Input.h"
#include <vector>
#include <map>
#include "GameLoop.h"

Input::Input()
{

}

//Currently adds a keyboard event of the event type mapping to func for each key in the vector.
//Eventually change this to all of the keys in the vector having to be pressed simultaneously.
void Input::addKeyboardEvent(std::function<void()> func, SDL_EventType eventType, std::vector<SDL_Keycode> keys)
{
	for (SDL_Keycode key : keys) {
		keyboardEventMap.insert(std::pair<std::pair<SDL_Keycode, SDL_EventType>, Callback>
			(std::pair<SDL_Keycode, SDL_EventType>(key, eventType), (Callback)func));
	}
}
 
void Input::handleInput(GameLoop* gameLoop)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{	
		switch (event.type) {
		case SDL_KEYDOWN:
			keyboard[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			keyboard[event.key.keysym.sym] = false;
			//React to KeyUp events
			for (auto const& x : keyboardEventMap)
			{
				if (SDL_KEYUP == x.first.second && event.key.keysym.sym == x.first.first) {
					x.second();
					//Call the associated function if types are equal
				}
			}
			break;
		case SDL_QUIT:
			gameLoop->stop();
			break;
		}
	}
	//React to KeyDown events
	for (auto const& x : keyboardEventMap)
	{
		if (keyboard.find(x.first.first) != keyboard.end()) {
			if (SDL_KEYDOWN == x.first.second
				&& keyboard.at(x.first.first)) {
				x.second();
				//Call the associated function if types are equal
			}
		}
	}
}
