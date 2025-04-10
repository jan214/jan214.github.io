#include "skeletalanimation.h"

#include <math.h>

SkeletalAnimation::SkeletalAnimation(){

}

SkeletalAnimation::~SkeletalAnimation(){
    for(Bone* bone : bones){
        delete bone;
    }

    bones.clear();
}

void SkeletalAnimation::addBone(float* translation,float* rotation,float* inverseTransformMatrix,Bone* parentBone){
    float rotationMatrix[16] = {cosf(rotation[0])*cosf(rotation[2]),cosf(rotation[2])*sinf(rotation[0])*sinf(rotation[1])+-sinf(rotation[2])*cosf(rotation[0]),cosf(rotation[2])*sinf(rotation[1])*cosf(rotation[0])+-sinf(rotation[0])*-sinf(rotation[2]),0.0f,
                               cosf(rotation[1])*sinf(rotation[2]),sinf(rotation[2])*sinf(rotation[0])*sinf(rotation[1])+cosf(rotation[2])*cosf(rotation[0]),sinf(rotation[2])*sinf(rotation[1])*cosf(rotation[0])+cosf(rotation[2])*-sinf(rotation[0]),0.0f,
                               -sinf(rotation[0])*cosf(rotation[1]),sinf(rotation[0])*cosf(rotation[1]),cosf(rotation[0])*cosf(rotation[1]),0.0f,
                               0.0f,0.0f,0.0f,1.0f};

    float translationMatrix[16] = {1.0f,0.0f,0.0f,translation[0],
                                  0.0f,1.0f,0.0f,translation[1],
                                  0.0f,0.0f,1.0f,translation[2],
                                  0.0f,0.0f,0.0f,1.0f};

    float* transformMatrix = multiplyMatrix(&rotationMatrix[0],&translationMatrix[0]);
    if(parentBone != nullptr){
        transformMatrix = multiplyMatrix(transformMatrix,parentBone->transformMatrix);
    }
    Bone* newbone = new Bone(transformMatrix,inverseTransformMatrix,parentBone);

    bones.push_back(newbone);
}

void SkeletalAnimation::newTransformMatrix(float* translation, float* rotation, int boneIndex){
    float rotationMatrix[16] = {cosf(rotation[0])*cosf(rotation[2]),cosf(rotation[2])*sinf(rotation[0])*sinf(rotation[1])+-sinf(rotation[2])*cosf(rotation[0]),cosf(rotation[2])*sinf(rotation[1])*cosf(rotation[0])+-sinf(rotation[0])*-sinf(rotation[2]),0.0f,
                               cosf(rotation[1])*sinf(rotation[2]),sinf(rotation[2])*sinf(rotation[0])*sinf(rotation[1])+cosf(rotation[2])*cosf(rotation[0]),sinf(rotation[2])*sinf(rotation[1])*cosf(rotation[0])+cosf(rotation[2])*-sinf(rotation[0]),0.0f,
                               -sinf(rotation[0])*cosf(rotation[1]),sinf(rotation[0])*cosf(rotation[1]),cosf(rotation[0])*cosf(rotation[1]),0.0f,
                               0.0f,0.0f,0.0f,1.0f};

    float translationMatrix[16] = {1.0f,0.0f,0.0f,translation[0],
                                  0.0f,1.0f,0.0f,translation[1],
                                  0.0f,0.0f,1.0f,translation[2],
                                  0.0f,0.0f,0.0f,1.0f};

    float* transformMatrix = multiplyMatrix(&rotationMatrix[0],&translationMatrix[0]);

    bones[boneIndex]->transformMatrices.push_back(&transformMatrix[0]);
}

float* SkeletalAnimation::multiplyMatrix(float* matrix1, float* matrix2){
    float resultMatrix[16];
    for(int index = 0; index < 16; index++){
        resultMatrix[index] = matrix1[index]*matrix2[index]+matrix1[index+1]*matrix2[index+4]+matrix1[index+2]*matrix2[index+8]+matrix1[index+3]*matrix2[index+12];
    }
    return matrix1;
}

SkeletalAnimation::Bone::Bone(float* transformMatrix, float* inverseTransformMatrix, Bone* parentBone) :
transformMatrix(transformMatrix),
inverseTransformMatrix(inverseTransformMatrix),
parentBone(parentBone),
transformMatrices(){

}
