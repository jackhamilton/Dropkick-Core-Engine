#pragma once
#include <list>
#include <vector>
#include <functional>
#include "Renderer.h"
#include "Point.h"
#include "GameObject.h"
#include "Sprite.h"
#include "Rectangle.h"

//Use std::function wrapped to Callback so that the map works
typedef std::function<void()> Callback;

using namespace std;
class Scene {
public:
    vector<GameObject*> getObjects();
	void destroy();
	vector<Sprite*> sprites;
	void addObject(GameObject* object);
	void addSprite(Sprite* sprite);

	//Engine objects - do not modify without very good reason
	vector<pair<Callback, Rectangle>> sceneMouseMovementEvents;
	vector<pair<Callback, Rectangle>> sceneMouseClickEvents;
	vector<pair<Callback, Rectangle>> sceneMouseRightClickEvents;
	vector<pair<Callback, Rectangle>> sceneMouseClickUpEvents;
	vector<pair<Callback, Rectangle>> sceneMouseRightClickUpEvents;
	Renderer* renderer;
	const char* name;
private:
    vector<GameObject*> objects;
};
