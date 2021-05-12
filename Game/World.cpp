#include "World.h"

World::World(Renderer* renderer, Input* input)
{
	World::renderer = renderer;
	World::input = input;
	World::mouse = input->getMouse();
}

void World::addScene(Scene* s, const char* name)
{
	s->renderer = renderer;
	s->name = name;
	scenes[name] = s;
	if (!currentScene) {
		setScene(name);
	}
}

void World::setScene(const char* name)
{
	if (mouse->sceneMouseExitedEvents) {
		vector<pair<function<void()>, Rectangle>> vec = *mouse->sceneMouseExitedEvents;
		for (vector<pair<function<void()>, Rectangle>>::iterator it = vec.begin(); it != vec.end(); it++)
		{
			pair<function<void()>, Rectangle> pair = *it;
			function<void()> func = pair.first;
			func();
		}
	}
	currentScene = scenes[name];
	mouse->sceneMouseClickEvents = &currentScene->sceneMouseClickEvents;
	mouse->sceneMouseEnteredEvents = &currentScene->sceneMouseEnteredEvents;
	mouse->sceneMouseExitedEvents = &currentScene->sceneMouseExitedEvents;
	mouse->sceneMouseClickUpEvents = &currentScene->sceneMouseClickUpEvents;
	mouse->sceneMouseMovementEvents = &currentScene->sceneMouseMovementEvents;
	mouse->sceneMouseRightClickEvents = &currentScene->sceneMouseRightClickEvents;
	mouse->sceneMouseRightClickUpEvents = &currentScene->sceneMouseRightClickUpEvents;
	mouse->activeScene = true;
}

string World::getCurrentSceneName()
{
	return currentScene->name;
}

Scene* World::getCurrentScene()
{
	return currentScene;
}

void World::setRenderer(Renderer* renderer)
{
	World::renderer = renderer;
}

void World::render(int frame, int fps)
{
	if (currentScene) {
		double MSPerFrame = (1 / (double)fps) * 1000;
		renderer->renderBackground();
		for (Sprite* cSprite : currentScene->sprites) {
			if (!cSprite->loaded) {
				cSprite->loadTextures(renderer);
			}
			cSprite->renderTimeBuffer += MSPerFrame;
			Point location = cSprite->getLocation();

			cSprite->render(renderer, Point(0, 0));

			cSprite->tick();
		}
		//Do the same thing but for GameObjects
		for (GameObject* obj : currentScene->getObjects()) {
			if (obj->hasSprite) {
				Sprite* cSprite = obj->getSprite();
				if (!cSprite->loaded) {
					cSprite->loadTextures(renderer);
				}
				cSprite->renderTimeBuffer += MSPerFrame;
				Point location = cSprite->getLocation();
				//If spritesheet

				cSprite->render(renderer, obj->getPosition());

				cSprite->tick();
			}
		}
	}
}

Renderer* World::getRenderer()
{
	return renderer;
}

void World::destroy()
{
	for (map<const char*, Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) {
		it->second->destroy();
	}
}
