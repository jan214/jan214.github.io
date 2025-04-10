#include "mymath.h"

MyMath::MyMath() {

}

MyMath::~MyMath() {

}

void MyMath::matrixMultiplication(const float* matrix, float* matrix2) {
    // I think this is wrong I need to save matrix2 in a variable and then multiply the saved variable with matrix1 and save in matrix2
    float copyMatrix2[16] = { matrix2[0],matrix2[1],matrix2[2],matrix2[3],matrix2[4],matrix2[5],matrix2[6],matrix2[7],matrix2[8],matrix2[9],matrix2[10],matrix2[11],matrix2[12],matrix2[13],matrix2[14],matrix2[15] };

    matrix2[0] = matrix[0] * copyMatrix2[0] + matrix[1] * copyMatrix2[4] + matrix[2] * copyMatrix2[8] + matrix[3] * copyMatrix2[12];
    matrix2[1] = matrix[0] * copyMatrix2[1] + matrix[1] * copyMatrix2[5] + matrix[2] * copyMatrix2[9] + matrix[3] * copyMatrix2[13];
    matrix2[2] = matrix[0] * copyMatrix2[2] + matrix[1] * copyMatrix2[6] + matrix[2] * copyMatrix2[10] + matrix[3] * copyMatrix2[14];
    matrix2[3] = matrix[0] * copyMatrix2[3] + matrix[1] * copyMatrix2[7] + matrix[2] * copyMatrix2[11] + matrix[3] * copyMatrix2[15];
    matrix2[4] = matrix[4] * copyMatrix2[0] + matrix[5] * copyMatrix2[4] + matrix[6] * copyMatrix2[8] + matrix[7] * copyMatrix2[12];
    matrix2[5] = matrix[4] * copyMatrix2[1] + matrix[5] * copyMatrix2[5] + matrix[6] * copyMatrix2[9] + matrix[7] * copyMatrix2[13];
    matrix2[6] = matrix[4] * copyMatrix2[2] + matrix[5] * copyMatrix2[6] + matrix[6] * copyMatrix2[10] + matrix[7] * copyMatrix2[14];
    matrix2[7] = matrix[4] * copyMatrix2[3] + matrix[5] * copyMatrix2[7] + matrix[6] * copyMatrix2[11] + matrix[7] * copyMatrix2[15];
    matrix2[8] = matrix[8] * copyMatrix2[0] + matrix[9] * copyMatrix2[4] + matrix[10] * copyMatrix2[8] + matrix[11] * copyMatrix2[12];
    matrix2[9] = matrix[8] * copyMatrix2[1] + matrix[9] * copyMatrix2[5] + matrix[10] * copyMatrix2[9] + matrix[11] * copyMatrix2[13];
    matrix2[10] = matrix[8] * copyMatrix2[2] + matrix[9] * copyMatrix2[6] + matrix[10] * copyMatrix2[10] + matrix[11] * copyMatrix2[14];
    matrix2[11] = matrix[8] * copyMatrix2[3] + matrix[9] * copyMatrix2[7] + matrix[10] * copyMatrix2[11] + matrix[11] * copyMatrix2[15];
    matrix2[12] = matrix[12] * copyMatrix2[0] + matrix[13] * copyMatrix2[4] + matrix[14] * copyMatrix2[8] + matrix[15] * copyMatrix2[12];
    matrix2[13] = matrix[12] * copyMatrix2[1] + matrix[13] * copyMatrix2[5] + matrix[14] * copyMatrix2[9] + matrix[15] * copyMatrix2[13];
    matrix2[14] = matrix[12] * copyMatrix2[2] + matrix[13] * copyMatrix2[6] + matrix[14] * copyMatrix2[10] + matrix[15] * copyMatrix2[14];
    matrix2[15] = matrix[12] * copyMatrix2[3] + matrix[13] * copyMatrix2[7] + matrix[14] * copyMatrix2[11] + matrix[15] * copyMatrix2[15];

    //matrix2[0] = matrix[0] * matrix2[0] + matrix[1] * matrix2[4] + matrix[2] * matrix2[8] + matrix[3] * matrix2[12];
    //matrix2[1] = matrix[0] * matrix2[1] + matrix[1] * matrix2[5] + matrix[2] * matrix2[9] + matrix[3] * matrix2[13];
    //matrix2[2] = matrix[0] * matrix2[2] + matrix[1] * matrix2[6] + matrix[2] * matrix2[10] + matrix[3] * matrix2[14];
    //matrix2[3] = matrix[0] * matrix2[3] + matrix[1] * matrix2[7] + matrix[2] * matrix2[11] + matrix[3] * matrix2[15];
    //matrix2[4] = matrix[4] * matrix2[0] + matrix[5] * matrix2[4] + matrix[6] * matrix2[8] + matrix[7] * matrix2[12];
    //matrix2[5] = matrix[4] * matrix2[1] + matrix[5] * matrix2[5] + matrix[6] * matrix2[9] + matrix[7] * matrix2[13];
    //matrix2[6] = matrix[4] * matrix2[2] + matrix[5] * matrix2[6] + matrix[6] * matrix2[10] + matrix[7] * matrix2[14];
    //matrix2[7] = matrix[4] * matrix2[3] + matrix[5] * matrix2[7] + matrix[6] * matrix2[11] + matrix[7] * matrix2[15];
    //matrix2[8] = matrix[8] * matrix2[0] + matrix[9] * matrix2[4] + matrix[10] * matrix2[8] + matrix[11] * matrix2[12];
    //matrix2[9] = matrix[8] * matrix2[1] + matrix[9] * matrix2[5] + matrix[10] * matrix2[9] + matrix[11] * matrix2[13];
    //matrix2[10] = matrix[8] * matrix2[2] + matrix[9] * matrix2[6] + matrix[10] * matrix2[10] + matrix[11] * matrix2[14];
    //matrix2[11] = matrix[8] * matrix2[3] + matrix[9] * matrix2[7] + matrix[10] * matrix2[11] + matrix[11] * matrix2[15];
    //matrix2[12] = matrix[12] * matrix2[0] + matrix[13] * matrix2[4] + matrix[14] * matrix2[8] + matrix[15] * matrix2[12];
    //matrix2[13] = matrix[12] * matrix2[1] + matrix[13] * matrix2[5] + matrix[14] * matrix2[9] + matrix[15] * matrix2[13];
    //matrix2[14] = matrix[12] * matrix2[2] + matrix[13] * matrix2[6] + matrix[14] * matrix2[10] + matrix[15] * matrix2[14];
    //matrix2[15] = matrix[12] * matrix2[3] + matrix[13] * matrix2[7] + matrix[14] * matrix2[11] + matrix[15] * matrix2[15];
}

void MyMath::matrixVectorMultiplication(const float* matrix, float* vector) {
    float vectorx = vector[0];
    float vectory = vector[1];
    float vectorz = vector[2];
    vector[0] = matrix[0] * vectorx + matrix[1] * vectory + matrix[2] * vectorz + matrix[3];
    vector[1] = matrix[4] * vectorx + matrix[5] * vectory + matrix[6] * vectorz + matrix[7];
    vector[2] = matrix[8] * vectorx + matrix[9] * vectory + matrix[10] * vectorz + matrix[11];
}

void MyMath::matrixVectorMultiplicationRotationOnly(const float* matrix, float* vector) {
    float vectorx = vector[0];
    float vectory = vector[1];
    float vectorz = vector[2];
    vector[0] = matrix[0] * vectorx + matrix[1] * vectory + matrix[2] * vectorz,
    vector[1] = matrix[4] * vectorx + matrix[5] * vectory + matrix[6] * vectorz,
    vector[2] = matrix[8] * vectorx + matrix[9] * vectory + matrix[10] * vectorz;
}
