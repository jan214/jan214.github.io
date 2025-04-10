#ifndef SKELETALANIMATION_H
#define SKELETALANIMATION_H

#include <vector>


class SkeletalAnimation
{    
public:
    struct Bone{
        Bone(float* transformMatrix,float* inverseTransformMatrix,Bone* parentBone);

        float* transformMatrix;
        float* inverseTransformMatrix;
        Bone* parentBone;
        std::vector<float*> transformMatrices;
    };

    SkeletalAnimation();
    ~SkeletalAnimation();

    void addBone(float* translation, float *rotation, float* inverseTransformMatrix, Bone* parentBone);

    void newTransformMatrix(float* translation, float* rotation, int boneIndex);

    std::vector<Bone*> bones;

private:
    float* multiplyMatrix(float* matrix1, float* matrix2);
};

#endif // SKELETALANIMATION_H
