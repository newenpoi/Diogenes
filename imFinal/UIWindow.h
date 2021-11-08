#pragma once

#define UI_WINDOW_CLASSNAME _T("UI_WINDOW_CLASSNAME")
#define UIWINDOW UIWindow::getInstance()

#include "imgui\imgui.h"
#include "imgui\backends\imgui_impl_win32.h"
#include "imgui\backends\imgui_impl_dx9.h"
#include <d3d9.h>
#include <tchar.h>

#pragma comment (lib, "d3d9.lib")

#include "Singleton.h"

/*
	Auteur : Newen
	Dernière Modification : Octobre 2021.

	Cette classe gère l'interface utilisateur.
*/

class UIWindow : public Singleton<UIWindow>
{
private:
	LPDIRECT3D9 _d3d; // Le pointeur vers l'interface Direct3D.
	LPDIRECT3DDEVICE9 _d3ddev; // Le pointeur vers le dispositif.
	D3DPRESENT_PARAMETERS _d3dpp; // Paramètres du dispositif Direct3D.

	HINSTANCE _hInstance;
	HWND _hWnd;

	UINT _x;
	UINT _y;
	UINT _width;
	UINT _height;

	bool _showDemoWindow;
	ImVec4 _clearColor;

	VOID InitD3D(HWND hWnd); // Mets en place et initialise Direct3D.
	VOID CleanD3D(void); // Ferme Direct3D et libère la mémoire.
	VOID ResetDevice(); // Réinitialise le dispositif Direct3D.

	VOID Update(); // Met à jour la frame.
	VOID DrawScene(); // Dessine la frame.
	BOOL InitImGui();
public:
	UIWindow();
	~UIWindow();

	VOID Run();
	BOOL Init(UINT x, UINT y, UINT width, UINT height);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

