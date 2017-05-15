#include "generateSphere.h"
#include <malloc.h>
#include <stdio.h>
#include <math.h>

int generateSphere(int numSlices, float radius, float **vertices,
	float **texCoords, uint16_t **indices, int *numVertices_out) {
	int i;
	int j;
	int numParallels = numSlices / 2;
	int numVertices = (numParallels + 1) * (numSlices + 1);
	int numIndices = numParallels * numSlices * 6;
	float angleStep = (2.0f * ES_PI) / ((float)numSlices);

	if (vertices != NULL)
		*vertices = (float*)malloc(sizeof(float) * 3 * numVertices);

	if (texCoords != NULL)
		*texCoords = (float*)malloc(sizeof(float) * 2 * numVertices);

	if (indices != NULL)
		*indices = (uint16_t*)malloc(sizeof(uint16_t) * numIndices);

	for (int i = 0; i < numParallels + 1; i++) {
		for (int j = 0; j < numSlices + 1; j++) {
			int vertex = (i * (numSlices + 1) + j) * 3;

			if (vertices) {
				(*vertices)[vertex + 0] = radius * sinf(angleStep * (float)i) *
					sinf(angleStep * (float)j);
				(*vertices)[vertex + 1] = radius * cosf(angleStep * (float)i);
				(*vertices)[vertex + 2] = radius * sinf(angleStep * (float)i) *
					cosf(angleStep * (float)j);
			}

			if (texCoords) {
				int texIndex = (i * (numSlices + 1) + j) * 2;
				(*texCoords)[texIndex + 0] = (float)j / (float)numSlices;
				(*texCoords)[texIndex + 1] = 1.0f - ((float)i / (float)numParallels);
			}
		}
	}

	if (indices != NULL) {
		uint16_t *indexBuf = (*indices);
		for (i = 0; i < numParallels; i++) {
			for (j = 0; j < numSlices; j++) {
				*indexBuf++ = i * (numSlices + 1) + j;
				*indexBuf++ = (i + 1) * (numSlices + 1) + j;
				*indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);

				*indexBuf++ = i * (numSlices + 1) + j;
				*indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);
				*indexBuf++ = i * (numSlices + 1) + (j + 1);
			}
		}
	}

	if (numVertices_out) {
		*numVertices_out = numVertices;
	}

	return numIndices;
}