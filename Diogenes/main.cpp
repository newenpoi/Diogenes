#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>

// Inclure la bibliothèque direct3d.
#pragma comment (lib, "d3d9.lib")

// Déclarations globales.
LPDIRECT3D9 d3d; // The pointer to our Direct3D interface.
LPDIRECT3DDEVICE9 d3ddev; // The pointer to the device class.

// Prototypes.
void initD3D(HWND hWnd); // Sets up and initializes Direct3D.
void render_frame(void); // Renders a single frame.
void cleanD3D(void); // Closes Direct3D and releases memory.

// The WindowProc function prototype.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// This function initializes and prepares direct3d for use.
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION); // Create the Direct3D interface.
    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // Discard old frames.
    d3dpp.hDeviceWindow = hWnd; // Set the window to be used by Direct3D.

    // Create a device class using this information and information from the d3dpp stucture.
    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
}

void cleanD3D(void)
{
    d3ddev->Release();
    d3d->Release();
}

void render_frame(void)
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);

    d3ddev->BeginScene();

    // Do 3D rendering on the back buffer here.

    d3ddev->EndScene();

    d3ddev->Present(NULL, NULL, NULL, NULL); // Displays the created frame.
}

// The entry point for any Windows program.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc; // Holds information for the window class.

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
        L"Viva las Direct3D !", // Titre de la fenêtre.
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

    // Entering the main loop...
    MSG msg;

    // Wait for the next message in the queue, store the result in msg.
    while (TRUE)
    {
        // Check any messages waiting in the queue.
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Translate the message and dispatch it to WindowProc().
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT) break;

        render_frame();
    }

    cleanD3D();

    // Return this part of the WM_QUIT message to Windows.
    return msg.wParam;
}

// This is the main message handler for the program.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Sort through and find what code to run for the message given.
    switch (message)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        break;
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, message, wParam, lParam);
}