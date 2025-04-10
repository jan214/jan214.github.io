#ifndef OBJECT_H
#define OBJECT_H

#pragma once

#if __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else
#if 1
#include "glad/glad.h"
#endif
#endif

#include "GLFW/include/glfw3.h"
#include <vector>

class Object
{
public:
    Object(class Shader &shader, class Collision* const collisionComponent, class SkeletalAnimation* const skeletalAnimationComponent);
    ~Object();

    void translate(float x, float y, float z);
    void translateDelta(float x, float y, float z);
    void rotate(float x,float y,float z);
    void rotateDelta(float x, float y, float z);

    template<typename T> void addElementBuffer(const T* const values, int valuesCount) {
        GLuint newBuffer;
        glGenBuffers(1,&newBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,newBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(T)*valuesCount*3,&values[0],GL_STATIC_DRAW);
        elementIndicesBuffer = newBuffer;
    }

    template <typename T> void addBuffer(const T* const values, int valuesCount) {
        GLuint newBuffer;
        glGenBuffers(1,&newBuffer);
        glBindBuffer(GL_ARRAY_BUFFER,newBuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(T)*valuesCount*3,&values[0],GL_STATIC_DRAW);
        buffers.push_back(newBuffer);
    }

    float translation[3];
    float rotation[3];
    int indicesCount;
    int verticesCount;

    float modelMatrix[16];

    class Collision* collisionComponent;
    class SkeletalAnimation* skeletalAnimationComponent;

    GLuint elementIndicesBuffer;
    std::vector<GLuint> buffers;
    std::vector<GLuint> uniforms;

private:
    class Shader& _shader;
};

#endif // OBJECT_H
