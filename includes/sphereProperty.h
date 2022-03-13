//���嶥�����꼰�������������࣬������vertexbuffer��indexbuffer����ģʽ

#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>


static const float PI = 3.14159265;
static const float PI_2 = 3.14159265 / 2;

#define RADIAN(angle) ((angle) / 180 * PI)  //�Ƕ�ת����

using namespace std;
using namespace glm;

class SphereProperty {

public:
	SphereProperty(unsigned int* sphereVAO, int* numPoint,float radius, int angleSpan = 9);
	SphereProperty(unsigned int* sphereVAO, int* numIndexPoints, float radius, int rings, int sectors);
	~SphereProperty();


private:
	void generateMeshWithVertex(int angleSpanG, int* numPoint,float radius);

	vector<vec3> mVertexCoords;
	vector<vec2> mTextureCoords;

	GLuint mVertexBuffer;
	GLuint mTexCoordBuffer;
	GLuint mIndexBuffer;
};
