#include "Mesh.h"



Mesh::Mesh()
{
	min = D3DXVECTOR3{ -INFINITY, -INFINITY, -INFINITY };
	max = D3DXVECTOR3{ INFINITY, INFINITY, INFINITY };
}


Mesh::~Mesh()
{
	D3D::Release<ID3DXMesh*>(mesh);
	for (int i = 0; i < numMtrls; i++) {
		textures[i]->Release();
	}
}

bool Mesh::Initialize(char * fileName)
{
	HRESULT hr = 0;
	hr = D3DXLoadMeshFromX(fileName, D3DXMESH_MANAGED, Device, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &mesh);
	if (FAILED(hr))
		MessageBox(NULL, "Failed to load mesh", "Error with mesh", MB_OK);
	if (mtrlBuffer != 0 && numMtrls != 0) {
		D3DXMATERIAL* tempMat = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		for (int i = 0; i < numMtrls; i++) {
			tempMat[i].MatD3D.Ambient = tempMat[i].MatD3D.Specular;
			materials.push_back(tempMat[i].MatD3D);

			if (tempMat[i].pTextureFilename != 0) {
				IDirect3DTexture9 *tempTex = 0;
				hr = D3DXCreateTextureFromFile(Device, tempMat[i].pTextureFilename, &tempTex);
				if (FAILED(hr))
					MessageBox(NULL, "Failed to load mesh texture", "Error with mesh", MB_OK);
				textures.push_back(tempTex);
			}
			else {
				textures.push_back(0);
			}
		}
	}
	BYTE* pVertices = NULL;
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
	D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, mesh->GetNumVertices(), D3DXGetFVFVertexSize(mesh->GetFVF()), &min, &max);
	mesh->UnlockVertexBuffer();


	D3D::Release<ID3DXBuffer*>(mtrlBuffer);
	D3D::Release<ID3DXBuffer*>(adjBuffer);

	return true;
}

bool Mesh::Render()
{
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (int i = 0; i < materials.size(); i++) {
		materials[i].Diffuse.a = 0.1f;
		Device->SetMaterial(&materials[i]);
		Device->SetTexture(0, textures[i]);
		mesh->DrawSubset(0);
	}

	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	return true;
}
