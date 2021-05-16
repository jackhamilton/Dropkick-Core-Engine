#include <stdio.h>
#include "GameObject.h"
#include "Point.h"
#include "Line.h"
#include <time.h>

static int cID = 0;

Point GameObject::getPosition()
{
	return GameObject::position;
}

void GameObject::setSize(Size size) {
	GameObject::size = size;
}

Size GameObject::getSize()
{
	if (size.width == 0 && size.height == 0) {
		pair<int, int> sizes = getSprite()->getDimensions();
		size = { sizes.first, sizes.second };
	}
	return size;
}

void GameObject::setPosition(Point pos)
{
	GameObject::position = pos;
}

GameObject::GameObject(Point position) {
	if (cID == NULL) {
		id = 0;
		cID = 1;
	}
	else {
		id = cID;
		cID++;
	}
	GameObject::position = position;
	hasSprite = false;
}

GameObject::GameObject(Point position, Sprite* sprite)
{
	hasSprite = true;
	if (cID == NULL) {
		id = 0;
		cID = 1;
	}
	else {
		id = cID;
		cID++;
	}
	GameObject::position = position;
	setSprite(sprite);
}

GameObject::~GameObject()
{
	delete sprite;
}

void GameObject::setSprite(Sprite* sprite) {
	hasSprite = true;
	GameObject::sprite = sprite;
	if (!sprite->loaded) {
		addSpriteToSceneRenderQueue(sprite);
	}
	else {
		updateSize();
	}
}

Sprite* GameObject::getSprite() {
	return GameObject::sprite;
}

void GameObject::addChild(GameObject* obj)
{
	children.push_back(obj);
}

void GameObject::queueEvent(GameObjectEvent* event)
{
	eventQueue.push(event);
}

void GameObject::queueEvents(vector<GameObjectEvent*> events)
{
	for (GameObjectEvent* e : events) {
		eventQueue.push(e);
	}
}

//Only works if in current scene.
void GameObject::move(ModifiableProperty<Vector, double> vector)
{
	function<void(GameObject*, ModifiableProperty<Vector, double>)> cb = *movementCallback;
	cb(this, vector);
}

//Only works if in current scene.
void GameObject::move(Vector vector)
{
	function<void(GameObject*, ModifiableProperty<Vector, double>)> cb = *movementCallback;
	cb(this, ModifiableProperty<Vector, double>(vector));
}

void GameObject::setPhysics(Physics * p)
{
	physics = p;
}

Physics* GameObject::getPhysics()
{
	return physics;
}

void GameObject::getMouseEvents(function<void()>* ret)
{
	ret[0] = mouseMoveEvent;
	ret[1] = mouseEnteredEvent;
	ret[2] = mouseExitedEvent;
	ret[3] = mouseClickGraphicEvent;
	ret[4] = mouseClickEvent;
	ret[5] = mouseRightClickEvent;
	ret[6] = mouseClickUpGraphicEvent;
	ret[7] = mouseClickUpEvent;
	ret[8] = mouseRightClickUpEvent;
}

void GameObject::removeFromParents()
{
	for (GameObject* c : children) {
		c->removeFromParents();
	}
	for (function<void(GameObject*)> func : removeCalls) {
		func(this);
	}
}

bool GameObject::testInBounds(Point p)
{
	return (p.x > position.x and p.x < position.x + size.width and p.y > position.y and p.y < position.y + size.height);
}

void GameObject::enableHitbox()
{
	//called from world to set up a default hitbox after sprite size loads
	hitboxEnabled = true;
	Size objSize = getSize();
	if (objSize.width != 0 && objSize.height != 0) {
		Rectangle r;
		r.x = 0;
		r.y = 0;
		r.width = objSize.width;
		r.height = objSize.height;
		hitbox = new Hitbox(r, &position);
	}
}

void GameObject::renderHitbox() {
	if (hitbox && !hitboxRendered) {
		vector<Point> corners = hitbox->getCorners();
		for (int i = 0; i < 4; i++) {
			Line* l = new Line({ 255, 0, 0 }, corners.at(i), corners.at(i == 3 ? 0 : i+1));
			addChild(l);
		}
		hitboxRendered = true;
	}
}

void GameObject::addCollisionEvent(function<void(Point)> event)
{
	collisionEvents.push_back(event);
}

void GameObject::addSpriteToSceneRenderQueue(Sprite* s)
{
	renderQueue.push_back(s);
}

void GameObject::updateSize()
{
	pair<int, int> sizeOfSprite = sprite->getDimensions();
	size = { sizeOfSprite.first, sizeOfSprite.second };
}

void GameObject::handleEvents()
{
	if (!eventQueue.empty()) {
		GameObjectEvent* cEvent = eventQueue.front();
		while (!eventQueue.empty() && eventQueue.front()->completed) {
			eventQueue.pop();
		}
		if (eventQueue.empty()) {
			return;
		}
		switch (cEvent->type) {
		case EventType::DESTROY:
			removeFromParents();
			break;
		case EventType::DELAY:
		{
			DelayEvent* eventAsProper = (DelayEvent*)cEvent;
			if (!eventAsProper->started) {
				eventAsProper->time = clock();
				eventAsProper->started = true;
			}
			if (clock() >= eventAsProper->time + eventAsProper->sceneTransitionDelayCycles + (eventAsProper->popTime) * (CLOCKS_PER_SEC / 1000)) {
				eventQueue.pop();
				handleEvents();
			}
		}
			break;
		case EventType::WAIT_FOR_ANIMATION_COMPLETION:
			if (sprite->startedAnimation && (sprite->paused || sprite->isOnFinalFrame())) {
				eventQueue.pop();
				handleEvents();
			}
			break;
		}
	}
}
