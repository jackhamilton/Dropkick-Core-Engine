#include <list>
#include "Scene.h"
#include "Point.h"
#include "Spritesheet.h"
#include "Button.h"

std::vector<GameObject*> Scene::getObjects()
{
	return objects;
}

void Scene::addObject(GameObject* object) {
	if (!renderer) {
		printf("ERROR: Scene has no renderer. Cannot load sprite textures.");
		return;
	}
	//Load the sprite's images with the scene renderer
	if (object->hasSprite) {
		Sprite* sprite = object->getSprite();
		sprite->loadTextures(renderer);
		Scene::objects.push_back(object);
	}

	function<void()> objectMouseEvents[5];
	object->getMouseEvents(objectMouseEvents);

	Rectangle objectSizeRectangle;
	objectSizeRectangle.x = object->getPosition().x;
	objectSizeRectangle.y = object->getPosition().y;
	objectSizeRectangle.width = object->getSize().width;
	objectSizeRectangle.height = object->getSize().height;
	if (object->hasMouseMoveEvent) {
		sceneMouseMovementEvents.push_back(make_pair(objectMouseEvents[0], objectSizeRectangle));
	}
	if (object->hasMouseClickEvent) {
		sceneMouseClickEvents.push_back(make_pair(objectMouseEvents[1], objectSizeRectangle));
	}
	if (object->hasMouseRightClickEvent) {
		sceneMouseRightClickEvents.push_back(make_pair(objectMouseEvents[2], objectSizeRectangle));
	}
	if (object->hasMouseClickUpEvent) {
		sceneMouseClickUpEvents.push_back(make_pair(objectMouseEvents[3], objectSizeRectangle));
	}
	if (object->hasMouseRightClickUpEvent) {
		sceneMouseRightClickUpEvents.push_back(make_pair(objectMouseEvents[4], objectSizeRectangle));
	}
}

void Scene::addSprite(Sprite* sprite) {
	if (!renderer) {
		printf("ERROR: Scene has no renderer. Cannot load sprite textures.");
		return;
	}
	//Load the sprite's images with the scene renderer
	sprite->loadTextures(renderer);
	sprites.push_back(sprite);
}

void Scene::destroy()
{
    for (Sprite* s: sprites)
		s->destroy();
	sprites.clear();
	renderer->destroy();
}
