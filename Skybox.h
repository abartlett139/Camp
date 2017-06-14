#ifndef SKYBOX_H
#define SKYBOX_H

#include "D3D.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	bool generate();
	void render();

private:

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DTEXTURE9 textures[6];
	D3D::EVertex mesh[24];
};

#endif