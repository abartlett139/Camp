#ifndef MESH_H
#define MESH_H

#include "D3D.h"


class Mesh
{
public:
	Mesh();
	~Mesh();

	bool Initialize(char *fileName);

	bool Render();

	ID3DXMesh *mesh;
	std::vector<D3DMATERIAL9>materials;
	std::vector<IDirect3DTexture9*>textures;
	ID3DXBuffer *adjBuffer = 0;
	ID3DXBuffer *mtrlBuffer = 0;
	DWORD numMtrls = 0;

	D3DXVECTOR3 min, max;
};

#endif