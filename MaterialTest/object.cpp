#include "object.h"

#include <iostream>
#include <math.h>

#include "shader.h"
#include "collision.h"
#include "skeletalanimation.h"
#include "mymath.h"

Object::Object(Shader& shader, Collision* const collisionComponent, SkeletalAnimation* const skeletalAnimationComponent) :
translation{0.0f,0.0f,0.0},
rotation{0.0f,0.0f,0.0f},
verticesCount(0),
modelMatrix{1.0f,0.0f,0.0f,0.0f,
  0.0f,1.0f,0.0f,0.0f,
  0.0f,0.0f,1.0f,0.0f,
  0.0f,0.0f,0.0f,1.0f},
collisionComponent(collisionComponent),
skeletalAnimationComponent(skeletalAnimationComponent),
buffers(),
uniforms(),
_shader(shader){
    if (_shader.uniformValues.size() != 0) {
        _shader.uniformValues[0] = &modelMatrix[0];
    } else {
        printf("shader has no modelMatrix\n");
    }

    if (this->collisionComponent != nullptr) {
        this->collisionComponent->modelMatrix = &modelMatrix[0];
        this->collisionComponent->center = &translation[0];
    }
}

Object::~Object() {
}

void Object::translate(float x,float y,float z){
    translation[0] = x;
    translation[1] = y;
    translation[2] = z;

    float rotationx[16] = { 1.0f,0.0f,0.0f,0.0f,0.0f,cosf(rotation[0]),-sinf(rotation[0]),0.0f,0.0f,sinf(rotation[0]),cosf(rotation[0]),0.0f,0.0f,0.0f,0.0f,1.0f };
    float rotationy[16] = { cosf(rotation[1]),0.0f,sinf(rotation[1]),0.0f,0.0f,1.0f,0.0f,0.0f,-sinf(rotation[1]),0.0f,cosf(rotation[1]),0.0f,0.0f,0.0f,0.0f,1.0f };
    float rotationz[16] = { cosf(rotation[2]),-sinf(rotation[2]),0.0f,0.0f,sinf(rotation[2]),cosf(rotation[2]),0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };

    MyMath::matrixMultiplication(rotationy, rotationx);
    MyMath::matrixMultiplication(rotationx, rotationz);

    float translationMatrix[16] = { 1.0f,0.0f,0.0f,translation[0],0.0f,1.0f,0.0f,translation[1],0.0f,0.0f,1.0f,translation[2],0.0f,0.0f,0.0f,1.0f };

    MyMath::matrixMultiplication(translationMatrix, rotationz);

    for (int counter = 0; counter < 16; counter++) {
        modelMatrix[counter] = rotationz[counter];
    }
}

void Object::translateDelta(float x, float y, float z) {
    translation[0] += x;
    translation[1] += y;
    translation[2] += z;

    float rotationx[16] = { 1.0f,0.0f,0.0f,0.0f,0.0f,cosf(rotation[0]),-sinf(rotation[0]),0.0f,0.0f,sinf(rotation[0]),cosf(rotation[0]),0.0f,0.0f,0.0f,0.0f,1.0f };
    float rotationy[16] = { cosf(rotation[1]),0.0f,sinf(rotation[1]),0.0f,0.0f,1.0f,0.0f,0.0f,-sinf(rotation[1]),0.0f,cosf(rotation[1]),0.0f,0.0f,0.0f,0.0f,1.0f };
    float rotationz[16] = { cosf(rotation[2]),-sinf(rotation[2]),0.0f,0.0f,sinf(rotation[2]),cosf(rotation[2]),0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };

    MyMath::matrixMultiplication(rotationy, rotationx);
    MyMath::matrixMultiplication(rotationx, rotationz);

    float translationMatrix[16] = { 1.0f,0.0f,0.0f,translation[0],0.0f,1.0f,0.0f,translation[1],0.0f,0.0f,1.0f,translation[2],0.0f,0.0f,0.0f,1.0f };

    MyMath::matrixMultiplication(translationMatrix, rotationz);

    for (int counter = 0; counter < 16; counter++) {
        modelMatrix[counter] = rotationz[counter];
    }
}

void Object::rotate(float x,float y,float z){
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;

    if (rotation[0] > 6.283f) {
        rotation[0] -= 6.283f;
    }
    else if (rotation[0] < 0.0f) {
        rotation[0] += 6.283f;
    }

    if (rotation[1] > 6.283f) {
        rotation[1] -= 6.283f;
    }
    else if (rotation[1] < 0.0f) {
        rotation[1] += 6.283f;
    }

    if (rotation[2] > 6.283f) {
        rotation[2] -= 6.283f;
    }
    else if (rotation[2] < 0.0f) {
        rotation[2] += 6.283f;
    }

    float rotationx[16] = { 1.0f,0.0f,0.0f,0.0f,0.0f,cosf(rotation[0]),-sinf(rotation[0]),0.0f,0.0f,sinf(rotation[0]),cosf(rotation[0]),0.0f,0.0f,0.0f,0.0f,1.0f };
    float rotationy[16] = { cosf(rotation[1]),0.0f,sinf(rotation[1]),0.0f,0.0f,1.0f,0.0f,0.0f,-sinf(rotation[1]),0.0f,cosf(rotation[1]),0.0f,0.0f,0.0f,0.0f,1.0f };
    float rotationz[16] = { cosf(rotation[2]),-sinf(rotation[2]),0.0f,0.0f,sinf(rotation[2]),cosf(rotation[2]),0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };

    MyMath::matrixMultiplication(rotationy, rotationx);
    MyMath::matrixMultiplication(rotationx, rotationz);

    float translationMatrix[16] = { 1.0f,0.0f,0.0f,translation[0],0.0f,1.0f,0.0f,translation[1],0.0f,0.0f,1.0f,translation[2],0.0f,0.0f,0.0f,1.0f };

    MyMath::matrixMultiplication(translationMatrix, rotationz);

    for (int counter = 0; counter < 16; counter++) {
        modelMatrix[counter] = rotationz[counter];
    }
}

void Object::rotateDelta(float x, float y, float z) {
    rotation[0] += x;
    rotation[1] += y;
    rotation[2] += z;

    if (rotation[0] > 6.283f) {
        rotation[0] -= 6.283f;
    }else if (rotation[0] < 0.0f) {
        rotation[0] += 6.283f;
    }

    if (rotation[1] > 6.283f) {
        rotation[1] -= 6.283f;
    }else if (rotation[1] < 0.0f) {
        rotation[1] += 6.283f;
    }

    if (rotation[2] > 6.283f) {
        rotation[2] -= 6.283f;
    }else if (rotation[2] < 0.0f) {
        rotation[2] += 6.283f;
    }

    float rotationx[16] = { 1.0f,0.0f,0.0f,0.0f,0.0f,cosf(rotation[0]),-sinf(rotation[0]),0.0f,0.0f,sinf(rotation[0]),cosf(rotation[0]),0.0f,0.0f,0.0f,0.0f,1.0f };
    float rotationy[16] = { cosf(rotation[1]),0.0f,sinf(rotation[1]),0.0f,0.0f,1.0f,0.0f,0.0f,-sinf(rotation[1]),0.0f,cosf(rotation[1]),0.0f,0.0f,0.0f,0.0f,1.0f };
    float rotationz[16] = { cosf(rotation[2]),-sinf(rotation[2]),0.0f,0.0f,sinf(rotation[2]),cosf(rotation[2]),0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };

    MyMath::matrixMultiplication(rotationy, rotationx);
    MyMath::matrixMultiplication(rotationx, rotationz);

    float translationMatrix[16] = { 1.0f,0.0f,0.0f,translation[0],0.0f,1.0f,0.0f,translation[1],0.0f,0.0f,1.0f,translation[2],0.0f,0.0f,0.0f,1.0f };

    MyMath::matrixMultiplication(translationMatrix, rotationz);

    for (int counter = 0; counter < 16; counter++) {
        modelMatrix[counter] = rotationz[counter];
    }
}
