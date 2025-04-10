#ifndef MATH_H
#define MATH_H

#pragma once

class MyMath {
public:
    MyMath();
    ~MyMath();

	static void matrixMultiplication(const float* matrix, float* matrix2);
	static void matrixVectorMultiplication(const float* matrix, float* vector);
	static void matrixVectorMultiplicationRotationOnly(const float* matrix, float* vector);
};

#endif // MATH_H
