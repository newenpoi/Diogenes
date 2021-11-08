#pragma once

#define UI_WINDOW_CLASSNAME _T("UI_WINDOW_CLASSNAME")
#define UIWINDOW UIWindow::getInstance()

#include <tchar.h>
#include <math.h>
#include <io.h>
#include <string>

#include "DX9.h"
#include "imgui\imgui.h"
#include "imgui\backends\imgui_impl_win32.h"
#include "imgui\backends\imgui_impl_dx9.h"

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
	HINSTANCE _hInstance; // Instance de la fenêtre win32.
	HWND _hWnd; // Handle de la fenêtre.

	UINT _x;
	UINT _y;
	UINT _width;
	UINT _height;

	bool _showDemoWindow;
	bool _showMainWindow;
	ImVec4 _clearColor;

	PDIRECT3DTEXTURE9 _texture;
	int _imageWidth;
	int _imageHeight;

	VOID ResetDevice(); // Réinitialise le dispositif d3d.

	VOID Update(); // Met à jour la frame.
	VOID DrawScene(); // Dessine la frame.
	BOOL InitImGui();
	BOOL SetFontList();
public:
	UIWindow();
	~UIWindow();

	VOID Run();
	BOOL Init(UINT x, UINT y, UINT width, UINT height);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

