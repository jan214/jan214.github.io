#ifndef COLLISION_H
#define COLLISION_H

#pragma once

class Collision {
public:
	Collision(float* moveDirection, float* rotationDirection);

	virtual bool overlap(Collision* collisionComponent) = 0;
	float* modelMatrix;
	float* center;
	float* rotation;
	float* scale;

	float* moveDirection;
	float* rotationDirection;
protected:
	void resolveCollision(float* resolveVector);
private:
};

class CollisionBox : public Collision {
public:
    static constexpr float vertices[24] = {1.0f,1.0f,1.0f, -1.0f,1.0f,1.0f, 1.0f,-1.0f,1.0f, -1.0f,-1.0f,1.0f, 1.0f,1.0f,-1.0f, -1.0f,1.0f,-1.0f, 1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f };
    static constexpr float normals[9] = { 1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,0.0f,1.0f };

	CollisionBox(float* moveDirection,float* rotationDirection);

	virtual bool overlap(Collision* collisionComponent) override;
};

class CollisionSphere : public Collision {
public:
	CollisionSphere(float* moveDirection,float* rotationDirection);
};

#endif // COLLISION_H
