/*

#include "imgui\imgui.h"
#include "imgui\backends\imgui_impl_win32.h"
#include "imgui\backends\imgui_impl_dx9.h"
#include <d3d9.h>
#include <tchar.h>

// Inclure la bibliothèque direct3d.
#pragma comment (lib, "d3d9.lib")

// Déclare à l'avance le message handler de imgui_impl_win32.cpp.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Déclarations globales.
LPDIRECT3D9 d3d; // Le pointeur vers l'interface Direct3D.
LPDIRECT3DDEVICE9 d3ddev; // Le pointeur vers le dispositif.
D3DPRESENT_PARAMETERS d3dpp; // Paramètres du dispositif Direct3D.

// Prototypes.
void initD3D(HWND hWnd); // Mets en place et initialise Direct3D.
void cleanD3D(void); // Ferme Direct3D et libère la mémoire.
void ResetDevice(); // Réinitialise le dispositif Direct3D.

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Cette fonction initialise et prépare l'utilisation de direct3d.
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION); // Create the Direct3D interface.

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // Discard old frames.

    // Autres paramètres.
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present avec vsync, framerate capped.
    // d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Present sans vsync, framerate maximal.

    // Create a device class using this information and information from the d3dpp stucture.
    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
}

void cleanD3D(void)
{
    // Libère la mémoire (et les pointeurs).
    if (d3ddev) { d3ddev->Release(); d3ddev = NULL; }
    if (d3d) { d3d->Release(); d3d = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = d3ddev->Reset(&d3dpp);

    if (hr == D3DERR_INVALIDCALL) IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Le point d'entrée pour toute application Windows.
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc; // Détiens les informations de la fenêtre (window class).

    // Clear out the window class for use.
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    // Crée la fenêtre et utilise le résultat comme handle.
    hWnd = CreateWindowEx(NULL,
        L"WindowClass", // Nom de la classe de la fenêtre.
        L"Diogenes", // Titre de la fenêtre.
        WS_OVERLAPPEDWINDOW, // Style de la fenêtre.
        300, // Position X de la fenêtre.
        300, // Position Y de la fenêtre.
        800, // Largeur de la fenêtre.
        600, // Hauteur de la fenêtre.
        NULL, // On a pas de fenêtre parent (null).
        NULL, // On utilise pas de menus (null).
        hInstance, // Handle de l'application.
        NULL); // Utilisé pour de multiples fenêtres (null).

    ShowWindow(hWnd, nCmdShow);
    initD3D(hWnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX9_Init(d3ddev);

    bool showDemoWindow = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // On entre dans la boucle principale.
    MSG msg;

    // Attends le prochain message dans la file d'attente et stocke le résultat dans msg.
    while (TRUE)
    {
        // Vérifie tous les messages en attente.
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Traduit et transfert le message vers WindowProc().
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT) break;

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (showDemoWindow) { ImGui::ShowDemoWindow(&showDemoWindow); }

        // Rendering...
        ImGui::EndFrame();
        d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
        d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        d3ddev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

        if (d3ddev->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            d3ddev->EndScene();
        }

        // Affiche la frame à l'écran.
        HRESULT result = d3ddev->Present(NULL, NULL, NULL, NULL);

        // Gère la perte du dispositif D3D9.
        if (result == D3DERR_DEVICELOST && d3ddev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) ResetDevice();
    }

    // Nettoyage...
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    cleanD3D();
    DestroyWindow(hWnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    // Renvoie cette partie du message WM_QUIT vers Windows.
    return msg.wParam;
}

// Ceci est le message handler principal du programme.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    // Fais le tri en exécutant le code selon le message.
    switch (message)
    {
    case WM_SIZE:
        if (d3ddev != NULL && wParam != SIZE_MINIMIZED)
        {
            d3dpp.BackBufferWidth = LOWORD(lParam);
            d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, message, wParam, lParam);
}

*/