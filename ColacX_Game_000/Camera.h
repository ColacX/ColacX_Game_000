#pragma once

#include "Entity.h"

#include "..\external\include\glm\gtx\matrix_cross_product.hpp"
#include "..\external\include\glm\glm.hpp"
#include "..\external\include\glm\gtc\matrix_transform.hpp"

class Camera: public Entity
{
private:	
public:
	Camera();
	void SetView();

	void fetchWorldMatrix(float* wm)
	{
		float ri[16] = {
			rm[ 0], rm[ 4], rm[ 8], 0,
			rm[ 1], rm[ 5], rm[ 9], 0,
			rm[ 2], rm[ 6], rm[10], 0,
			0, 0, 0, 1
		};

		float tm[16] = {
			ri[ 0],      0,      0, ri[ 3] * (-px),
			     0, ri[ 5],      0, ri[ 7] * (-py),
			     0,      0, ri[10], ri[11] * (-pz),
			     0,      0,      0,         ri[15]
		};

		float pm[16] = {
			ri[ 0],      0,      0, (-px),
			     0,  ri[ 5],     0, (-py),
			     0,      0, ri[10], (-pz),
			     0,      0,      0, ri[15]
		};

		for( int i=0; i<16; i++)
		{
			wm[i] = tm[i];
		}
	}

	glm::mat4 CamerMatrix()
    {
        float ri[16] = {
			rm[ 0], rm[ 4], rm[ 8], 0,
			rm[ 1], rm[ 5], rm[ 9], 0,
			rm[ 2], rm[ 6], rm[10], 0,
			0, 0, 0, 1
		};

        glm::mat4 matrix;
        matrix[0][0] = ri[ 0];
        matrix[0][1] = ri[ 1];
        matrix[0][2] = ri[ 2];
        matrix[0][3] = ri[ 3];

        matrix[1][0] = ri[ 4];
        matrix[1][1] = ri[ 5];
        matrix[1][2] = ri[ 6];
        matrix[1][3] = ri[ 7];

        matrix[2][0] = ri[ 8];
        matrix[2][1] = ri[ 9];
        matrix[2][2] = ri[10];
        matrix[2][3] = ri[11];

        matrix[3][0] = ri[12];
        matrix[3][1] = ri[13];
        matrix[3][2] = ri[14];
        matrix[3][3] = ri[15];

        glm::vec3 pos(-px, -py, -pz);

        return glm::translate(matrix, pos);
    }
};
