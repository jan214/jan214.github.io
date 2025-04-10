#include "collision.h"

#include <iostream>
#include <limits>
#include <math.h>

#include "object.h"
#include "mymath.h"

constexpr float CollisionBox::vertices[24];
constexpr float CollisionBox::normals[9];

Collision::Collision(float* moveDirection,float* rotationDirection) :
modelMatrix(nullptr),
center(center),
rotation(rotation),
scale(scale),
moveDirection(moveDirection),
rotationDirection(rotationDirection) {
}

void Collision::resolveCollision(float* resolveVector) {
	center[0] += resolveVector[0];
	center[1] += resolveVector[1];
	center[2] += resolveVector[2];
}

CollisionBox::CollisionBox(float* moveDirection,float* rotationDirection) : Collision(moveDirection,rotationDirection) {
	
}

bool CollisionBox::overlap(Collision* collisionComponent) {
	// currently when the shapes are the same position and scale no collision occures which is wrong!!!
	if (CollisionBox* const otherBox = static_cast<CollisionBox*>(collisionComponent)) {
		float minOverlap = std::numeric_limits<float>().max();
		float minimalTranslationVector[3] = { 0.0f,0.0f,0.0f };
		float overlappingVertex[3] = {0.0f,0.0f,0.0f};
		for (int axisIndex = 0; axisIndex < 9; axisIndex += 3) {
			float minimum = std::numeric_limits<float>().max();
			float maximum = std::numeric_limits<float>().min();

			float otherMinimum = std::numeric_limits<float>().max();
			float otherMaximum = std::numeric_limits<float>().min();

			float normalAxis[3] = {normals[axisIndex], normals[axisIndex + 1], normals[axisIndex + 2]};

			// calculate normals
            MyMath::matrixVectorMultiplicationRotationOnly(&modelMatrix[0], &normalAxis[0]);

			for (int verticesIndex = 0; verticesIndex < 24; verticesIndex += 3) {
				float vertex[3] = { vertices[verticesIndex],vertices[verticesIndex + 1],vertices[verticesIndex + 2] };
                MyMath::matrixVectorMultiplication(&modelMatrix[0], &vertex[0]);
				const float tempValue = vertex[0] * normalAxis[0] + vertex[1] * normalAxis[1] + vertex[2] * normalAxis[2];
				if (verticesIndex == 0) {
					overlappingVertex[0] = vertex[0];
					overlappingVertex[1] = vertex[1];
					overlappingVertex[2] = vertex[2];
					minimum = tempValue;
					maximum = tempValue;
				}
				if (tempValue < minimum) {
					overlappingVertex[0] = vertex[0];
					overlappingVertex[1] = vertex[1];
					overlappingVertex[2] = vertex[2];
					minimum = tempValue;
				}
				if (tempValue > maximum) {
					overlappingVertex[0] = vertex[0];
					overlappingVertex[1] = vertex[1];
					overlappingVertex[2] = vertex[2];
					maximum = tempValue;
				}
				float otherVertex[3] = { otherBox->vertices[verticesIndex],otherBox->vertices[verticesIndex + 1],otherBox->vertices[verticesIndex + 2] };
                MyMath::matrixVectorMultiplication(&otherBox->modelMatrix[0], &otherVertex[0]);
				const float tempOtherValue = otherVertex[0] * normalAxis[0] + otherVertex[1] * normalAxis[1] + otherVertex[2] * normalAxis[2];
				if (verticesIndex == 0) {
					otherMinimum = tempOtherValue;
					otherMaximum = tempOtherValue;
				}
				if (tempOtherValue < otherMinimum) {
					otherMinimum = tempOtherValue;
				}
				if (tempOtherValue > otherMaximum) {
					otherMaximum = tempOtherValue;
				}
			}

			const bool leftBool = maximum >= otherMaximum && minimum <= otherMaximum;
			const bool rightBool = maximum >= otherMinimum && minimum <= otherMinimum;
			const bool outsideBool = maximum >= otherMaximum && minimum <= otherMinimum;
			const bool insideBool = maximum <= otherMaximum && minimum >= otherMinimum;

			if (leftBool || rightBool || outsideBool || insideBool) {
				float overlap = std::fmin(maximum, otherMaximum) - std::fmax(minimum, otherMinimum);
				if (overlap > 0.0f && overlap < minOverlap) {
					minOverlap = overlap;
					minimalTranslationVector[0] = normalAxis[0];
					minimalTranslationVector[1] = normalAxis[1];
					minimalTranslationVector[2] = normalAxis[2];
				}
			} else {
				return false;
			}
		}

		for (int axisIndex = 0; axisIndex < 9; axisIndex += 3) {
			float minimum = std::numeric_limits<float>().max();
			float maximum = std::numeric_limits<float>().min();

			float otherMinimum = std::numeric_limits<float>().max();
			float otherMaximum = std::numeric_limits<float>().min();

			float normalAxis[3] = { otherBox->normals[axisIndex], otherBox->normals[axisIndex + 1], otherBox->normals[axisIndex + 2] };

			// calculate normals
            MyMath::matrixVectorMultiplicationRotationOnly(&otherBox->modelMatrix[0], &normalAxis[0]);

			for (int verticesIndex = 0; verticesIndex < 24; verticesIndex += 3) {
				float vertex[3] = { vertices[verticesIndex],vertices[verticesIndex + 1],vertices[verticesIndex + 2] };
                MyMath::matrixVectorMultiplication(&modelMatrix[0], &vertex[0]);
				const float tempValue = vertex[0] * normalAxis[0] + vertex[1] * normalAxis[1] + vertex[2] * normalAxis[2];
				if (verticesIndex == 0) {
					overlappingVertex[0] = vertex[0];
					overlappingVertex[1] = vertex[1];
					overlappingVertex[2] = vertex[2];
					minimum = tempValue;
					maximum = tempValue;
				}
				if (tempValue < minimum) {
					overlappingVertex[0] = vertex[0];
					overlappingVertex[1] = vertex[1];
					overlappingVertex[2] = vertex[2];
					minimum = tempValue;
				}
				if (tempValue > maximum) {
					overlappingVertex[0] = vertex[0];
					overlappingVertex[1] = vertex[1];
					overlappingVertex[2] = vertex[2];
					maximum = tempValue;
				}
				float otherVertex[3] = { otherBox->vertices[verticesIndex],otherBox->vertices[verticesIndex + 1],otherBox->vertices[verticesIndex + 2] };
                MyMath::matrixVectorMultiplication(&otherBox->modelMatrix[0], &otherVertex[0]);
				const float tempOtherValue = otherVertex[0] * normalAxis[0] + otherVertex[1] * normalAxis[1] + otherVertex[2] * normalAxis[2];
				if (otherMinimum > tempOtherValue) {
					otherMinimum = tempOtherValue;
				}
				if (otherMaximum < tempOtherValue) {
					otherMaximum = tempOtherValue;
				}
			}

			if (maximum < minimum) {
				const float temp = minimum;
				minimum = maximum;
				maximum = temp;
			}

			const bool leftBool = maximum >= otherMaximum && minimum <= otherMaximum;
			const bool rightBool = maximum >= otherMinimum && minimum <= otherMinimum;
			const bool outsideBool = maximum >= otherMaximum && minimum <= otherMinimum;
			const bool insideBool = maximum <= otherMaximum && minimum >= otherMinimum;

			if (leftBool || rightBool || outsideBool || insideBool) {
				float overlap = std::fmin(maximum, otherMaximum) - std::fmax(minimum, otherMinimum);
				if (overlap > 0.0f && overlap < minOverlap) {
					minOverlap = overlap;
					minimalTranslationVector[0] = normalAxis[0];
					minimalTranslationVector[1] = normalAxis[1];
					minimalTranslationVector[2] = normalAxis[2];
				}
			} else {
				return false;
			}

		}

		float crossProductAxes[27];

		for (int axisIndex = 0; axisIndex < 9; axisIndex += 3) {
			for (int axisIndex2 = 0; axisIndex2 < 9; axisIndex2 += 3) {
				crossProductAxes[axisIndex*3 + axisIndex2] = normals[axisIndex + 1] * otherBox->normals[axisIndex2 + 2] - normals[axisIndex + 2] * otherBox->normals[axisIndex2 + 1];
				crossProductAxes[axisIndex*3 + axisIndex2 + 1] = normals[axisIndex + 2] * otherBox->normals[axisIndex2] - normals[axisIndex] * otherBox->normals[axisIndex2 + 2];
				crossProductAxes[axisIndex*3 + axisIndex2 + 2] = normals[axisIndex] * otherBox->normals[axisIndex2 + 1] - normals[axisIndex + 1] * otherBox->normals[axisIndex2];
			}
		}

		const int arraySize = sizeof(crossProductAxes)/sizeof(float);

		for (int axisIndex = 0; axisIndex < arraySize; axisIndex += 3) {
			float minimum = std::numeric_limits<float>().max();
			float maximum = std::numeric_limits<float>().min();

			float otherMinimum = std::numeric_limits<float>().max();
			float otherMaximum = std::numeric_limits<float>().min();

			float normalAxis[3] = { crossProductAxes[axisIndex], crossProductAxes[axisIndex + 1], crossProductAxes[axisIndex + 2] };

			for (int verticesIndex = 0; verticesIndex < 24; verticesIndex += 3) {
				float vertex[3] = { vertices[verticesIndex],vertices[verticesIndex + 1],vertices[verticesIndex + 2] };
                MyMath::matrixVectorMultiplication(&modelMatrix[0], &vertex[0]);
				const float tempValue = vertex[0] * normalAxis[0] + vertex[1] * normalAxis[1] + vertex[2] * normalAxis[2];
				if (verticesIndex == 0) {
					overlappingVertex[0] = vertex[0];
					overlappingVertex[1] = vertex[1];
					overlappingVertex[2] = vertex[2];
					minimum = tempValue;
					maximum = tempValue;
				}
				if (tempValue < minimum) {
					overlappingVertex[0] = vertex[0];
					overlappingVertex[1] = vertex[1];
					overlappingVertex[2] = vertex[2];
					minimum = tempValue;
				}
				if (tempValue > maximum) {
					overlappingVertex[0] = vertex[0];
					overlappingVertex[1] = vertex[1];
					overlappingVertex[2] = vertex[2];
					maximum = tempValue;
				}

				float otherVertex[3] = { otherBox->vertices[verticesIndex],otherBox->vertices[verticesIndex + 1],otherBox->vertices[verticesIndex + 2] };
                MyMath::matrixVectorMultiplication(&otherBox->modelMatrix[0], &otherVertex[0]);
				const float tempOtherValue = otherVertex[0] * normalAxis[0] + otherVertex[1] * normalAxis[1] + otherVertex[2] * normalAxis[2];
				if (verticesIndex == 0) {
					minimum = tempOtherValue;
					maximum = tempOtherValue;
				}
				if (otherMinimum > tempOtherValue) {
					otherMinimum = tempOtherValue;
				}
				if (otherMaximum < tempOtherValue) {
					otherMaximum = tempOtherValue;
				}
			}

			if (maximum < minimum) {
				const float temp = minimum;
				minimum = maximum;
				maximum = temp;
			}

			const bool leftBool = maximum >= otherMaximum && minimum <= otherMaximum;
			const bool rightBool = maximum >= otherMinimum && minimum <= otherMinimum;
			const bool outsideBool = maximum >= otherMaximum && minimum <= otherMinimum;
			const bool insideBool = maximum <= otherMaximum && minimum >= otherMinimum;

			if (leftBool || rightBool || outsideBool || insideBool) {
				float overlap = std::fmin(maximum, otherMaximum) - std::fmax(minimum, otherMinimum);
				if (overlap > 0.0f && overlap < minOverlap) {
					minOverlap = overlap;
					minimalTranslationVector[0] = normalAxis[0];
					minimalTranslationVector[1] = normalAxis[1];
					minimalTranslationVector[2] = normalAxis[2];
				}
			} else {
				return false;
			}
		}

		if (minOverlap <= 0.001f) {
			return false;
		}

		float minimalTranslationVectorLength = minimalTranslationVector[0] * minimalTranslationVector[0] + minimalTranslationVector[1] * minimalTranslationVector[1] + minimalTranslationVector[2] * minimalTranslationVector[2];
		minimalTranslationVector[0] = minimalTranslationVector[0] / minimalTranslationVectorLength;
		minimalTranslationVector[1] = minimalTranslationVector[1] / minimalTranslationVectorLength;
		minimalTranslationVector[2] = minimalTranslationVector[2] / minimalTranslationVectorLength;

		// collision response should be moved into another function!
		if (minimalTranslationVector[0] != 0.0f || minimalTranslationVector[1] != 0.0f || minimalTranslationVector[2] != 0.0f) {
			moveDirection[0] = minimalTranslationVector[0];
			moveDirection[1] = minimalTranslationVector[1];
			moveDirection[2] = minimalTranslationVector[2];

			//moveDirection[0] = 0.0f;
			//moveDirection[1] = 0.0f;
			//moveDirection[2] = 0.0f;

			printf("minimalTranslationVector: %f %f %f minOverlap: %f\n", minimalTranslationVector[0], minimalTranslationVector[1], minimalTranslationVector[2], minOverlap);

			//if (minOverlap < 0.0001f){
			//	minOverlap = 0.0001f;
			//}

			const float collisionPoint[3] = /*{ center[0] + (center[0] - otherBox->center[0])*0.5f, center[1] + (center[1] - otherBox->center[1])*0.5f, center[2] + (center[2] - otherBox->center[2])*0.5f };*/
			 { overlappingVertex[0] + minimalTranslationVector[0] * 0.5f, overlappingVertex[1] + minimalTranslationVector[1] * 0.5f, overlappingVertex[2] + minimalTranslationVector[2] * 0.5f};
			printf("center: %f %f %f\n", center[0], center[1], center[2]);
			printf("collisionPoint: %f %f %f\n", collisionPoint[0], collisionPoint[1], collisionPoint[2]);
			const float leverArm[3] = {  center[0] - collisionPoint[0], center[1] - collisionPoint[1], center[2] - collisionPoint[2] };

			center[0] += minimalTranslationVector[0] * minOverlap;
			center[1] += minimalTranslationVector[1] * minOverlap;
			center[2] += minimalTranslationVector[2] * minOverlap;

			printf("leverArm: %f %f %f\n", leverArm[0], leverArm[1], leverArm[2]);
			printf("minimalTranslationVectorProcessed: %f %f %f\n", minimalTranslationVector[0], minimalTranslationVector[1], minimalTranslationVector[2]);

			float torque[3] = { 0.0f };
			torque[0] = leverArm[1]*minimalTranslationVector[2]-leverArm[2]*minimalTranslationVector[1];
			torque[1] = leverArm[2]*minimalTranslationVector[0]-leverArm[0]*minimalTranslationVector[2];
			torque[2] = leverArm[0]*minimalTranslationVector[1]-leverArm[1]*minimalTranslationVector[0];

			printf("torque: %f %f %f\n", torque[0], torque[1], torque[2]);

			rotationDirection[0] = torque[0];
			rotationDirection[1] = torque[1];
			rotationDirection[2] = torque[2];

			//rotationDirection[0] = 0.0f;
			//rotationDirection[1] = 0.0f;
			//rotationDirection[2] = 0.0f;

			//printf("leverArm: %f %f %f\n", leverArm[0], leverArm[1], leverArm[2]);
		}
	}

	return true;
}

CollisionSphere::CollisionSphere(float* moveDirection,float* rotationDirection) : Collision(moveDirection,rotationDirection){

}
