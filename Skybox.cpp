#include "Skybox.h"



Skybox::Skybox() : vertexBuffer(0)
{
}


Skybox::~Skybox()
{
	for (int i = 0; i < 6; ++i)
	{
		textures[i]->Release();
		textures[i] = 0;
	}
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
	}
}


bool Skybox::generate()
{
	float size = 10;
	D3D::EVertex mesh[24] =
	{
		//Front
		{ -size,-size,size, 0,1 },
		{ -size,size,size, 0,0 },
		{ size,-size,size, 1,1 },
		{ size,size,size, 1,0 },
		//Back
		{ size, -size, -size, 0,1 },
		{ size, size, -size, 0,0 },
		{ -size,-size, -size, 1,1 },
		{ -size,size, -size, 1,0 },
		//Left
		{ -size, -size, -size, 0,1 },
		{ -size, size, -size, 0,0 },
		{ -size,-size, size, 1,1 },
		{ -size, size, size, 1,0 },
		//Right
		{ size,-size,size, 0,1 },
		{ size,size,size, 0,0 },
		{ size,-size,-size, 1,1 },
		{ size,size,-size, 1,0 },
		//Top
		{ -size,size,size, 0,1 },
		{ -size,size,-size, 0,0 },
		{ size,size,size, 1,1 },
		{ size,size,-size, 1,0 },
		//Bottom
		{ -size,-size,-size, 0,1 },
		{ -size,-size,size, 0,0 },
		{ size,-size,-size, 1,1 },
		{ size,-size,size, 1,0 }
	};
	HRESULT hRet;
	hRet = Device->CreateVertexBuffer((int)sizeof(D3D::EVertex) * 24, 0, D3D::EVertex::FVF,
		D3DPOOL_MANAGED, &vertexBuffer, 0);
	if (FAILED(hRet))
	{
		MessageBox(NULL, "Failed to create skybox VB", "Error", MB_OK);
		return false;
	}

	void* pVertices = NULL;
	vertexBuffer->Lock(0, (UINT)sizeof(D3D::EVertex)*24, (void**)&pVertices, 0);
	memcpy(pVertices, mesh, sizeof(D3D::EVertex) * 24);
	vertexBuffer->Unlock();
	////load textures
	hRet = D3DXCreateTextureFromFile(Device, "textures/front.png", &textures[0]);
	hRet = D3DXCreateTextureFromFile(Device, "textures/back.png", &textures[1]);
	hRet = D3DXCreateTextureFromFile(Device, "textures/left.png", &textures[2]);
	hRet = D3DXCreateTextureFromFile(Device, "textures/right.png", &textures[3]);
	hRet = D3DXCreateTextureFromFile(Device, "textures/top.png", &textures[4]);
	hRet = D3DXCreateTextureFromFile(Device, "textures/bottom.png", &textures[5]);

	if (FAILED(hRet))
		MessageBox(NULL, "Skybox Texture BAD", "Error with Skybox", MB_OK);

	return true;
}

void Skybox::render()
{
	//Device->SetRenderState(D3DRS_LIGHTING, false);
	Device->SetRenderState(D3DRS_ZENABLE, false);
	//clamping to get rid of seams
	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	Device->SetFVF(D3D::EVertex::FVF);
	Device->SetStreamSource(0, vertexBuffer, 0, sizeof(D3D::EVertex));
	for (int i = 0; i < 6; ++i)
	{
		Device->SetTexture(0, textures[i]);
		Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, (int)i*4, 2);
	}

	Device->SetRenderState(D3DRS_ZENABLE, true);

	//Device->SetRenderState(D3DRS_ZENABLE, true);
}
