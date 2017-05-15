#ifndef __GENERATESPHERE_H__
#define __GENERATESPHERE_H__

#define ES_PI  (3.14159265f)
typedef unsigned short uint16_t;

int generateSphere(int numSlices, float radius, float **vertices,
	float **texCoords, uint16_t **indices, int *numVertices_out);


#endif