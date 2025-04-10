#include "physicssystem.h"

PhysicsSystem::PhysicsSystem() {

}

void PhysicsSystem::addObject(const Object& object) {
	objects.push_back(object);
}

void PhysicsSystem::checkCollision() {
	for (const Object& object : objects) {
		if (object.collisionComponent == nullptr)
			continue;
	}
}