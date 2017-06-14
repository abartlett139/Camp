#include "Terrain.h"

Terrain::Terrain() : vertexBuffer(0), indexBuffer(0)
{
}


//	releases vertex buffer, index buffer, and texture
Terrain::~Terrain()
{
	if (vertexBuffer)
		D3D::Release(vertexBuffer);
	if (indexBuffer)
		D3D::Release(indexBuffer);
	if (terrainTexture)
		D3D::Release(terrainTexture);
}


//	returns the height at any given x,z coordinates
float Terrain::getHeight(float x, float z)
{
	//	find the location in reference to the unscaled heightmap
	x = (((float)width / 2.0f) + x) / cellSpacing;
	z = (((float)depth / 2.0f) - z) / cellSpacing;

	//	round x and z to the nearest row and column
	float col = floorf(x);
	float row = floorf(z);

	//	get the heights of all 4 corners of the current quad
	float A = getHeightmapEntry(row, col);
	float B = getHeightmapEntry(row, col + 1);
	float C = getHeightmapEntry(row + 1, col);
	float D = getHeightmapEntry(row + 1, col + 1);

	//	get the triangle in the quad and lerp based on which one the coordinates are in
	float deltaX = x - col;
	float deltaZ = z - row;

	float height = 0.0f;

	if (deltaZ < 1.0f - deltaX) {
		float uy = B - A;
		float vy = C - A;			  
		height = A + D3D::Lerp(0.0f, uy, deltaX) + D3D::Lerp(0.0f, vy, deltaZ);
	}
	else {
		float uy = C - D; 
		float vy = B - D; 
		height = D + D3D::Lerp(0.0f, uy, 1.0f - deltaX) + D3D::Lerp(0.0f, vy, 1.0f - deltaZ);
	}


	//	return the height 
	return height;
}

//	get height helper function -- accesses the heightap
int Terrain::getHeightmapEntry(int x, int z)
{
	unsigned int index = x * numVertsPerRow + z;
	if (index >= 0 && index < heightmap.size()) {
		return heightmap[index];
	}
	else {
		return 0;
	}
}



void Terrain::Initialize(int nVPR, int nVPC, int cS, float hS)
{
	//	set all variables
	numVertsPerRow = nVPR;
	numVertsPerCol = nVPC;
	cellSpacing = cS;
	heightScale = hS;

	numCellsPerRow = numVertsPerRow - 1;
	numCellsPerCol = numVertsPerCol - 1;

	width = numCellsPerRow * cellSpacing;
	depth = numCellsPerCol * cellSpacing;

	numVertices = numVertsPerRow * numVertsPerCol;
	numTriangles = numCellsPerRow * numCellsPerCol * 2;

	//	load texture
	terrainTexture = D3D::LoadTexture("textures/terrain.png");

	//	read raw file
	readRawFile("textures/heightmap.raw");

	//	set heights
	for (int i = heightmap.size() - 1; i >= 0; i--) {
		heightmap[i] = heightmap[i] * heightScale;
	}

	//	compute vertices and indices
	vertexCompute();
	indexCompute();
}

//	sets stream source, FVF, index buffer, texture, and renders
void Terrain::Render()
{
	Device->SetStreamSource(0, vertexBuffer, 0, sizeof(D3D::EVertex));
	Device->SetFVF(D3D::EVertex::FVF);
	Device->SetIndices(indexBuffer);
	Device->SetTexture(0, terrainTexture);

	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numTriangles);
}



//	reads binary raw file, byte by byte and assigns it to an int vector (the heightmap)
void Terrain::readRawFile(char * fileName)
{
	std::vector<BYTE> in(numVertices);

	std::ifstream inF(fileName, std::ios_base::binary);

	inF.read((char*)&in[0], in.size());

	inF.close();

	heightmap.resize(numVertices);

	for (int i = 0; i < in.size(); i++)
		heightmap[i] = in[i];
}

void Terrain::vertexCompute()
{
	//	create vertex buffer
	Device->CreateVertexBuffer(numVertices * sizeof(D3D::EVertex), D3DUSAGE_WRITEONLY, D3D::EVertex::FVF, D3DPOOL_MANAGED, &vertexBuffer, 0);

	//	define start and stop points
	int startX = -width / 2;
	int startZ = depth / 2;
	int endX = width / 2;
	int endZ = -depth / 2;

	//	size of increments inbetween cells 
	float uCoordIncrementSize = 1.0f / (float)numCellsPerRow;
	float vCoordIncrementSize = 1.0f / (float)numCellsPerCol;

	//	lock vertex buffer and compute vertices based on heightmap
	D3D::EVertex* v = 0;
	vertexBuffer->Lock(0, 0, (void**)&v, 0);

	int i = 0;
	for (int z = startZ; z >= endZ; z -= cellSpacing)
	{
		int j = 0;
		for (int x = startX; x <= endX; x += cellSpacing)
		{ 
			int index = i * numVertsPerRow + j;

			v[index] = D3D::EVertex(
				(float)x,
				(float)heightmap[index],
				(float)z,
				(float)j * uCoordIncrementSize,
				(float)i * vCoordIncrementSize);
			j++;
		}
		i++;
	}

	vertexBuffer->Unlock();
}

void Terrain::indexCompute()
{
	//	create index buffer and lock it
	Device->CreateIndexBuffer(numTriangles * 3 * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &indexBuffer, 0);

	DWORD* indices = 0;
	indexBuffer->Lock(0, 0, (void**)&indices, 0);

	int baseIndex = 0;

	// loop through and compute the triangles of each quad
	for (int i = 0; i < numCellsPerCol; i++)
	{
		for (int j = 0; j < numCellsPerRow; j++)
		{
			indices[baseIndex] = i   * numVertsPerRow + j;
			indices[baseIndex + 1] = i   * numVertsPerRow + j + 1;
			indices[baseIndex + 2] = (i + 1) * numVertsPerRow + j;

			indices[baseIndex + 3] = (i + 1) * numVertsPerRow + j;
			indices[baseIndex + 4] = i   * numVertsPerRow + j + 1;
			indices[baseIndex + 5] = (i + 1) * numVertsPerRow + j + 1;

			// next quad
			baseIndex += 6;
		}
	}

	indexBuffer->Unlock();

}
