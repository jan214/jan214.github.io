#ifndef SHADER_H
#define SHADER_H

#pragma once

#include <vector>

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

#include "object.h"

class Shader
{
public:
    Shader(const char *vertexShaderSource, const char *fragmentShaderSource);

    void addObject(Object* object);

    void useProgram();
    void addAttribute(const char* attributeName, int stride);
    void bindAttributes(Object* object);

    void addSamplerUniform(const char* uniformName);
    void addTexture(const int width, const int height, const unsigned char* source);
    void bindSamplerUniforms(std::vector<GLuint>& textures);

    void addUniform(const char* uniformName, int stride, float* values);
    void bindUniforms(Object* object);

    void draw();

    GLuint program;

    float* viewMatrix;
    float* perspectiveMatrix;

    std::vector<float*> uniformValues;

    bool hasFramebuffer;

private:
    struct AttributeInformation{
        AttributeInformation(GLuint attribute,int stride) : attribute(attribute),stride(stride){};
        GLuint attribute;
        int stride;
    };

    const char* _vertexShaderSource;
    const char* _fragmentShaderSource;
    GLuint _vertexShader,_fragmentShader;

    GLuint _framebuffer;

    std::vector<Object*> _objects;
    std::vector<AttributeInformation> _attributes;
    std::vector<GLuint> _uniforms;
    std::vector<int> _uniformStrides;
    std::vector<GLuint> _uniformSamplers;
    std::vector<GLuint> _textures;

    static void check_error(GLuint shader,const char* shaderName);
};

#endif // SHADER_H
