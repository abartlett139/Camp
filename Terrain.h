#ifndef TERRAIN_H
#define TERRAIN_H

#include "D3D.h"

class Terrain {
public:
	Terrain();
	~Terrain();

	

	void Initialize(int nVPR, int nVPC, int cS, float hS);
	void Render();

	void readRawFile(char *fileName);
	void vertexCompute();
	void indexCompute();
	float getHeight(float x, float z);
	int getHeightmapEntry(int x, int z);



	IDirect3DTexture9 *terrainTexture;
	IDirect3DVertexBuffer9 *vertexBuffer;
	IDirect3DIndexBuffer9 *indexBuffer;

	std::vector<int> heightmap;
	int numVertices, numTriangles, numVertsPerRow, numVertsPerCol, numCellsPerRow, numCellsPerCol, cellSpacing, width, depth;
	float heightScale;
};

#endif