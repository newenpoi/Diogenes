#pragma once

#define DIRECTX9 DX9::getInstance()

#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment (lib, "d3d9.lib")

#include "Singleton.h"

/*
	Auteur : Newen
	Dernière Modification : Octobre 2021.

	Cette classe gère le module directx.

	Documentation :
	- https://docs.microsoft.com/fr-fr/windows/win32/direct3d9/d3dswapeffect
*/

class DX9 : public Singleton<DX9>
{
private:
	LPDIRECT3D9 _d3d; // Le pointeur vers l'interface Direct3D.
	LPDIRECT3DDEVICE9 _d3ddev; // Le pointeur vers le dispositif (contexte).
	D3DPRESENT_PARAMETERS _d3dpp; // Paramètres du dispositif Direct3D.

	VOID CleanD3D(void); // Ferme Direct3D et libère la mémoire.
public:
	DX9();
	~DX9();

	LPDIRECT3D9 InitD3D(HWND hWnd); // Mets en place et initialise Direct3D.
	BOOL LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* output, int* outputWidth, int* outputHeight);

	LPDIRECT3D9 GetDevice();
	LPDIRECT3DDEVICE9 GetDeviceContext();
};

