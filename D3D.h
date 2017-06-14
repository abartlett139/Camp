#ifndef D3D_H
#define D3D_H

#include <d3dx9.h>
#include <vector>
#include <fstream>
#include "MATH.h"
#include "resource.h"


extern IDirect3DDevice9 *Device;
static D3DPRESENT_PARAMETERS d3dpp;

#define screenWidth 1600
#define screenHeight 900

namespace D3D {

	bool InitD3D(HINSTANCE hInstance);

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 BLACK_MTRL = InitMtrl(BLACK, BLACK, BLACK, BLACK, 2.0f);

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

	IDirect3DTexture9 * LoadTexture(char * fileName);



	//	lerp for terrain interpolation
	float Lerp(float a, float b, float t);

	//	convert a float value to dword
	DWORD FtoDw(float f);
	//	get a random float
	float GetRandomFloat(float min, float max);
	//	get a random vector
	void GetRandomVector(D3DXVECTOR3 *out, D3DXVECTOR3 *min, D3DXVECTOR3 *max);

	//	vertex struct for environment objects
	struct EVertex {
		EVertex() {}
		EVertex(float X, float Y, float Z, float U, float V) : x(X), y(Y), z(Z), u(U), v(V) {}
		float x, y, z, u, v;
		static const DWORD FVF = D3DFVF_XYZ | D3DFVF_TEX1;
	};

	struct BoundingBox {
		BoundingBox() : MIN(INFINITY, INFINITY, INFINITY), MAX(-INFINITY, -INFINITY, -INFINITY) {}

		D3DXVECTOR3 center() {
			return 0.5f*(MIN + MAX);
		}

		bool isPointInside(D3DXVECTOR3 &p);

		D3DXVECTOR3 MIN, MAX;
	};


	struct BoundingSphere {
		BoundingSphere();

		D3DXVECTOR3 _center;
		float _radius;
	};


	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}


}


#endif