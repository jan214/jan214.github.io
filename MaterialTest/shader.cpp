#include "shader.h"

#include <iostream>

Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource) :
program(),
hasFramebuffer(false),
_vertexShaderSource(vertexShaderSource),
_fragmentShaderSource(fragmentShaderSource),
_vertexShader(),
_fragmentShader(),
_framebuffer(),
_objects(),
_attributes(),
_uniforms(),
_uniformStrides(),
_uniformSamplers(),
_textures()
{
    _vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexShader,1,&vertexShaderSource,nullptr);
    glCompileShader(_vertexShader);
    check_error(_vertexShader,"vertexShader");

    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragmentShader,1,&fragmentShaderSource,nullptr);
    glCompileShader(_fragmentShader);
    check_error(_fragmentShader,"fragmentShader");

    program = glCreateProgram();
    glAttachShader(program,_vertexShader);
    glAttachShader(program,_fragmentShader);
    glLinkProgram(program);
    glUseProgram(program);
}

void Shader::addObject(Object* object){
    _objects.push_back(object);
}

void Shader::useProgram() {
    glUseProgram(program);
}

void Shader::addAttribute(const char* attributeName,int stride){
    GLuint newGLAttribute = glGetAttribLocation(program,attributeName);
    glVertexAttribPointer(newGLAttribute,stride,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(newGLAttribute);
    _attributes.push_back(AttributeInformation(newGLAttribute,stride));
}

void Shader::bindAttributes(Object* object){
    for(unsigned int attributeCounter = 0; attributeCounter < _attributes.size(); attributeCounter++){
        glBindBuffer(GL_ARRAY_BUFFER,object->buffers[attributeCounter]);
        glVertexAttribPointer(_attributes[attributeCounter].attribute,_attributes[attributeCounter].stride,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(_attributes[attributeCounter].attribute);
    }
}

void Shader::addSamplerUniform(const char* uniformName){
    GLuint newUniformLocation = glGetUniformLocation(program,uniformName);
    glUniform1i(newUniformLocation,_uniformSamplers.size());
    _uniformSamplers.push_back(newUniformLocation);
}

void Shader::addTexture(const int width, const int height, const unsigned char* source) {
    GLuint newTexture;
    glGenTextures(1, &newTexture);
    glBindTexture(GL_TEXTURE_2D, newTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (source == nullptr){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
    }else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
    }


    _textures.push_back(newTexture);
}

void Shader::bindSamplerUniforms(std::vector<GLuint>& textures){
    for(unsigned int samplerCounter = 0; samplerCounter < _uniformSamplers.size(); samplerCounter++){
        glActiveTexture(GL_TEXTURE0 + samplerCounter);
        glBindTexture(GL_TEXTURE_2D,textures[samplerCounter]);
        glUniform1i(_uniformSamplers[samplerCounter],samplerCounter);
    }
}

void Shader::addUniform(const char* uniformName,int stride,float* values){
    GLuint newUniformLocation = glGetUniformLocation(program,uniformName);
    _uniforms.push_back(newUniformLocation);
    _uniformStrides.push_back(stride);
    if (values != nullptr) {
        if (stride == 3) {
            printf("name: %s", uniformName);
            printf("value: %f\n", values[0]);
        }
    }
    uniformValues.push_back(values);
}

void Shader::bindUniforms(Object* object){
//    for(unsigned int uniformCounter = 0; uniformCounter < object.uniforms.size(); uniformCounter++){
    for(unsigned int uniformCounter = 0; uniformCounter < _uniforms.size(); uniformCounter++){
        if(_uniformStrides[uniformCounter] == 3){
            if (uniformCounter == 0) {
                glUniform3fv(_uniforms[uniformCounter], 1, uniformValues[uniformCounter]);
            }else {
                glUniform3fv(_uniforms[uniformCounter], 2, uniformValues[uniformCounter]);
            }
            //glUniform3fv(_uniforms[uniformCounter],1,object.translation);
//            printf("translationx: %f\n",object.translation[0]);
        }else if(_uniformStrides[uniformCounter] == 16){
            if(uniformCounter == 0){
                glUniformMatrix4fv(_uniforms[uniformCounter],1,true,&object->modelMatrix[0]);
            }else if(uniformCounter == 1){
                glUniformMatrix4fv(_uniforms[uniformCounter],1,false,&perspectiveMatrix[0]);
            }else if(uniformCounter == 2){
                glUniformMatrix4fv(_uniforms[uniformCounter],1,true,&viewMatrix[0]);
            }
        }
    }

 /*   for (unsigned int uniformCounter = 0; uniformCounter < _uniforms.size(); uniformCounter++) {
        if (_uniformStrides[uniformCounter] == 3) {
            glUniform3fv(_uniforms[uniformCounter], 1, uniformValues[uniformCounter]);
        }else if (_uniformStrides[uniformCounter] == 16) {
            if (uniformCounter == 0) {
                glUniformMatrix4fv(_uniforms[uniformCounter], 1, true, &object.modelMatrix[0]);
            }else{
                if (uniformCounter == 1) {
                    glUniformMatrix4fv(_uniforms[uniformCounter], 1, true, &perspectiveMatrix[0]);
                }else if (uniformCounter == 2) {
                    printf("uniformMatrix:\n");
                    for (int counter = 0; counter < 16; counter++) {
                        printf("%f %f %f %f\n", viewMatrix[counter * 4], viewMatrix[counter * 4 + 1], viewMatrix[counter * 4 + 2], viewMatrix[counter * 4 + 3]);
                    }
                    printf("\n");
                    glUniformMatrix4fv(_uniforms[uniformCounter], 1, true, &viewMatrix[0]);
                }
            }
        }
    }*/
}

void Shader::draw(){
    for(Object* object : _objects){
        bindAttributes(object);
        bindUniforms(object);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->elementIndicesBuffer);
        glDrawElements(GL_TRIANGLES,object->indicesCount,GL_UNSIGNED_INT,0);
//        glDrawArrays(GL_TRIANGLES,0,object->verticesCount);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Shader::check_error(GLuint shader,const char* shaderName)
{
    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        GLint log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> log(log_length);

        GLsizei length;
        glGetShaderInfoLog(shader, log.size(), &length, log.data());

        printf("Error:%s %s\n",shaderName,log.data());
    }
}
