#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <vector>

#include "object.h"

class PhysicsSystem {
public:
	PhysicsSystem();

	void addObject(const Object& object);
	void checkCollision();

private:
	std::vector<Object> objects;
};

#endif // PHYSICSSYSTEM_H