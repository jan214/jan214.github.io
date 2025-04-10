#ifndef RESOURCES_H
#define RESOURCES_H

#pragma once

namespace Resources {
    // geometry
    constexpr int indices[3] = {0,1,2};
    constexpr int quadIndices[6] = { 0,1,2,0,2,5 };
    constexpr float vertices[18] = { -0.75f,-0.75f,0.0f,-0.75f,0.75f,0.0f,0.75f,0.75f,0.0f,-0.75f,-0.75f,0.0f,0.75f,0.75f,0.0f,0.75f,-0.75f,0.0f };
    constexpr float triangle[9] = { -0.5f,-0.5f,0.0f,0.5f,0.5f,0.0f,-0.5f,0.5f,0.0f };
    //float triangleNormals[] = {0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f};
    constexpr float triangleNormals2[9] = { 0.0f,0.0f,1.0, 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f };
    constexpr float triangleTangents2[9] = { 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f };
    constexpr float triangleBitangents2[9] = { 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f };
    constexpr float triangle2[9] = { -0.5f,-0.5f,0.0f,0.5f,-0.5f,0.0f,0.5f,0.5f,0.0f };
    constexpr float textureCoords[12] = { 0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f };
    constexpr float textureCoordstriangle[6] = { 0.0f,0.0f,1.0f,1.0f,0.0f,1.0f };
    constexpr float textureCoordstriangle2[6] = { 0.0f,0.0f,1.0f,0.0f,1.0f,1.0f };

    constexpr float cubeVertices[108] = { -1.0f,-1.0f,1.0f, -1.0f,1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f,1.0f,1.0f ,1.0f,1.0f,-1.0f,
                -1.0f,1.0f,-1.0f, 1.0f,1.0f,1.0f, 1.0f,-1.0f,-1.0f, 1.0f,1.0f,-1.0f, 1.0f,-1.0f,1.0f,
                -1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f,1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f,1.0f,-1.0f,
                -1.0f,1.0f,1.0f, 1.0f,-1.0f,1.0f, 1.0f,1.0f,1.0f, -1.0f,-1.0f,1.0f, -1.0f,1.0f,1.0f,
                -1.0f,1.0f,-1.0f, -1.0f,1.0f,1.0f, 1.0f,1.0f,1.0f, 1.0f,1.0f,-1.0f, 1.0f,1.0f,1.0f,
                1.0f,-1.0f,1.0f, 1.0f,-1.0f,-1.0f, 1.0f,-1.0f,1.0f, -1.0f,-1.0f,1.0f, -1.0f,-1.0f,-1.0f,
                1.0f,1.0f,-1.0f, 1.0f,-1.0f,-1.0f, -1.0f,-1.0f,-1.0f, -1.0f,1.0f,1.0f, -1.0f,-1.0f,1.0f, 1.0f,-1.0f,1.0f };

    // 1,2,-1,3,6,-3,7,4,-7,5,0,-5,6,0,-3,3,5,-8,1,3,-3,3,7,-7,7,5,-5,5,1,-1,6,4,-1,3,1,-6
    constexpr int cubeIndices[108] = /*{1,2,0,3,6,2,7,4,6,5,0,4,6,0,2,3,5,7,1,3,2,3,7,6,7,5,4,5,1,0,6,4,0,3,1,5}*/
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107 };

    constexpr float cubeTextureCoordinates[72] = /*{ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };*/
    { 0.0f,0.0f, 0.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f, 0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f, 1.0f,1.0f, 1.0f,0.0f,
        0.0f,0.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
        0.0f,1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
        0.0f,1.0f, 0.0f,1.0f, 1.0f,1.0f, 1.0f,1.0f, 1.0f,1.0f,
        1.0f,0.0f, 1.0f,0.0f, 1.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f,
        1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f, 0.0f,1.0f, 0.0f,0.0f, 1.0f,0.0f };

    constexpr float cubeNormals[108] = { -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, 
        0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 
        0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 
        0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 
        0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 
        -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, 
        0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 
        0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 
        0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 
        0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f };
    constexpr float cubeTangents[108] = { 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
        0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f,
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f,
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
        0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f,
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f, 1.0f,0.0f,0.0f };
    constexpr float cubeBitangents[108] = {0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 
        0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 
        0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f,
        0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f,
        0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f,
        0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f,
        0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f,
        0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f, 0.0f,0.0f,1.0f,
        0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f,
        0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f, 0.0f,1.0f,0.0f };

    // animations

    // float cubeTextureMap[] = {};
    //int triangleBones[3] = {0,0,0};
    //int bones[36] = {1,0,0, 1,0,0, 1,0,0, 1,0,0, 0,0,0, 1,1,1, 1,1,0, 1,1,0, 1,1,0, 1,1,0, 0,0,0, 1,1,1};
    constexpr float armatureTranslation[3] = { 0.0f,-1.0f,0.0f };
    constexpr float armatureRotation[3] = { -90.0f,0.0f,0.0f };
    constexpr float bone1Translation[3] = { 0.0f,0.0f,0.0f };
    constexpr float bone1Rotation[3] = { 90.0f,-90.0f,0.0f };
    constexpr float bone1Inverse[16] = { 0.0f,1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,-1.0f,0.0f,1.0f };
    constexpr float bone2Translation[3] = { 1.0f,0.0f,0.0f };
    constexpr float bone2Rotation[3] = { 0.0f,0.0f,0.0f };
    constexpr float bone2Inverse[16] = { 0.0f,1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };
    constexpr float animation1testTranslation[3] = { 0.0f,0.0f,0.0f };
    constexpr float animation1testRotation[3] = { 0.2f,0.0f,0.0f };
    // lcl matrixes are scaling*rotation*translation and fbx rotation are in XYZ order.
};

#endif // RESOURCES_H