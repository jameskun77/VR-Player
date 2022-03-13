#include "sphereProperty.h"

SphereProperty::SphereProperty(unsigned int* sphereVAO, int* numPoint, float radius, int angleSpan)
{
	generateMeshWithVertex(angleSpan, numPoint,radius);

	glGenVertexArrays(1, sphereVAO);
	glBindVertexArray(*sphereVAO);

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mVertexCoords.size(), &mVertexCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glGenBuffers(1, &mTexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * mTextureCoords.size(), &mTextureCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindVertexArray(GL_NONE);
}

SphereProperty::SphereProperty(unsigned int* sphereVAO, int* numIndexPoints, float radius, int rings, int sectors)
{
	float R = 1.0f / (float)rings;
	float S = 1.0f / (float)sectors;
	short r, s;
	float x, y, z;

	int numPoint = (rings + 1) * (sectors + 1);
	*numIndexPoints = numPoint * 6;
	float* vertexs = new float[numPoint * 3];
	float* texcoords = new float[numPoint * 2];
	short* indices = new short[numPoint * 6];

	int t = 0, v = 0;
	for (r = 0; r < rings + 1; r++) {
		for (s = 0; s < sectors + 1; s++) {
			x = (float)(cosf(2 * PI * s * S) * sinf(PI * r * R));
			y = (float)sinf(-PI_2 + PI * r * R);
			z = (float)(sinf(2 * PI * s * S) * sinf(PI * r * R));
			texcoords[t++] = s * S;
			texcoords[t++] = r * R;

			vertexs[v++] = x * radius;
			vertexs[v++] = y * radius;
			vertexs[v++] = z * radius;
		}
	}

	int counter = 0;
	int sectorsPlusOne = sectors + 1;
	for (r = 0; r < rings; r++) {
		for (s = 0; s < sectors; s++) {
			indices[counter++] = (short)(r * sectorsPlusOne + s);       //(a)
			indices[counter++] = (short)((r + 1) * sectorsPlusOne + (s));    //(b)
			indices[counter++] = (short)((r)* sectorsPlusOne + (s + 1));  // (c)
			indices[counter++] = (short)((r)* sectorsPlusOne + (s + 1));  // (c)
			indices[counter++] = (short)((r + 1) * sectorsPlusOne + (s));    //(b)
			indices[counter++] = (short)((r + 1) * sectorsPlusOne + (s + 1));  // (d)
		}
	}

	glGenVertexArrays(1, sphereVAO);
	glBindVertexArray(*sphereVAO);

	// 加载顶点坐标 position
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numPoint * 3 * sizeof(float), vertexs, GL_STATIC_DRAW);

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numPoint * 6 * sizeof(short), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	//加载纹理坐标 UV
	glGenBuffers(1, &mTexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mTexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * numPoint, texcoords, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);

	// 释放内存
	delete[] vertexs;
	delete[] indices;
	delete[] texcoords;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

SphereProperty::~SphereProperty()
{
	mVertexCoords.clear();
	mTextureCoords.clear();
}

void SphereProperty::generateMeshWithVertex(int angleSpan, int* numPoint, float radius)
{
	//构建顶点坐标 z轴朝上，x轴朝外，y轴朝右
	for (float vAngle = -90; vAngle < 90; vAngle += angleSpan) // 垂直方向-90 - 90度
	{
		for (float hAngle = 0; hAngle < 360; hAngle += angleSpan)
		{
			double xozLen = radius * cos(RADIAN(vAngle));
			float x1 = (float)(xozLen * cos(RADIAN(hAngle)));
			float z1 = (float)(xozLen * sin(RADIAN(hAngle)));
			float y1 = (float)radius  * sin(RADIAN(vAngle));

			xozLen = radius * cos(RADIAN(vAngle + angleSpan));
			float x2 = (float)(xozLen * cos(RADIAN(hAngle)));
			float z2 = (float)(xozLen * sin(RADIAN(hAngle)));
			float y2 = (float)radius  * sin(RADIAN(vAngle + angleSpan));

			xozLen = radius * cos(RADIAN(vAngle + angleSpan));
			float x3 = (float)(xozLen * cos(RADIAN(hAngle + angleSpan)));
			float z3 = (float)(xozLen * sin(RADIAN(hAngle + angleSpan)));
			float y3 = (float)radius  * sin(RADIAN(vAngle + angleSpan));

			xozLen = radius * cos(RADIAN(vAngle));
			float x4 = (float)(xozLen * cos(RADIAN(hAngle + angleSpan)));
			float z4 = (float)(xozLen * sin(RADIAN(hAngle + angleSpan)));
			float y4 = (float)radius  * sin(RADIAN(vAngle));

			vec3 v1(x1, y1, z1);
			vec3 v2(x2, y2, z2);
			vec3 v3(x3, y3, z3);
			vec3 v4(x4, y4, z4);

			//构建第一个三角形
			mVertexCoords.push_back(v1);
			mVertexCoords.push_back(v2);
			mVertexCoords.push_back(v4);

			//构建第二个三角形
			mVertexCoords.push_back(v4);
			mVertexCoords.push_back(v2);
			mVertexCoords.push_back(v3);
		}
	}

	//构建纹理球面坐标，球面展开后的矩形
	int width = 360 / angleSpan; //列数
	int height = 180 / angleSpan; //行数

	float dw = 1.0f / width;
	float dh = 1.0f / height;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float s = j * dw;
			float t = i * dh;

			vec2 v1(s, t);
			vec2 v2(s, t + dh);
			vec2 v3(s + dw, t + dh);
			vec2 v4(s + dw, t);

			mTextureCoords.push_back(v1);
			mTextureCoords.push_back(v2);
			mTextureCoords.push_back(v4);

			//构建第二个三角形
			mTextureCoords.push_back(v4);
			mTextureCoords.push_back(v2);
			mTextureCoords.push_back(v3);
		}
	}

	*numPoint = mVertexCoords.size();
}
