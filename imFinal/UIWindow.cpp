#include "UIWindow.h"

// Déclare à l'avance le message handler de imgui_impl_win32.cpp.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace {
    UIWindow* uiWindow = 0;
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return uiWindow->WindowProc(hWnd, message, wParam, lParam);
}

UIWindow::UIWindow() : _x(0), _y(0), _width(0), _height(0), _d3d(nullptr), _d3ddev(nullptr), _hInstance(0), _hWnd(0), _showDemoWindow(false), _clearColor({})
{
    _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.0f);
    uiWindow = this;
}

UIWindow::~UIWindow()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanD3D();
    DestroyWindow(_hWnd);
}

// Cette fonction initialise et prépare l'utilisation de direct3d.
VOID UIWindow::InitD3D(HWND hWnd)
{
    _d3d = Direct3DCreate9(D3D_SDK_VERSION); // Create the Direct3D interface.

    ZeroMemory(&_d3dpp, sizeof(_d3dpp));
    _d3dpp.Windowed = TRUE;
    _d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // Discard old frames.

    // Autres paramètres.
    _d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    _d3dpp.EnableAutoDepthStencil = TRUE;
    _d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    _d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Présent avec vsync, framerate capped.
    // d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Présent sans vsync, framerate maximal.

    // Create a device class using this information and information from the d3dpp stucture.
    _d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &_d3dpp, &_d3ddev);
}

VOID UIWindow::CleanD3D(void)
{
    // Libère la mémoire (et les pointeurs).
    if (_d3ddev) { _d3ddev->Release(); _d3ddev = NULL; }
    if (_d3d) { _d3d->Release(); _d3d = NULL; }
}

VOID UIWindow::ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = _d3ddev->Reset(&_d3dpp);

    if (hr == D3DERR_INVALIDCALL) IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

BOOL UIWindow::Init(UINT x, UINT y, UINT width, UINT height)
{
    WNDCLASSEX wc; // Détiens les informations de la fenêtre (window class).

    // Clear out the window class for use.
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWindowProc;
    wc.cbClsExtra = 0; // Extra.
    wc.cbWndExtra = 0; // Extra.
    wc.hInstance = _hInstance;
    wc.hIcon = NULL; // Icône de la fenêtre.
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL; // Extra.
    wc.lpszClassName = UI_WINDOW_CLASSNAME;
    wc.hIconSm = NULL; // Extra.

    RegisterClassEx(&wc);

    // Crée la fenêtre et utilise le résultat comme handle.
    _hWnd = CreateWindowEx(
        WS_EX_TOPMOST,
        wc.lpszClassName, // Nom de la classe de la fenêtre.
        L"Diogenes", // Titre de la fenêtre.
        WS_POPUP, // Style de la fenêtre.
        x, // Position X de la fenêtre.
        y, // Position Y de la fenêtre.
        width, // Largeur de la fenêtre.
        height, // Hauteur de la fenêtre.
        NULL, // On a pas de fenêtre parent (null).
        NULL, // On utilise pas de menus (null).
        _hInstance, // Handle de l'application.
        NULL); // Utilisé pour de multiples fenêtres (null).

    _x = x; _y = y; _width = width, _height = height;

    ShowWindow(_hWnd, SW_SHOWDEFAULT);
    UpdateWindow(_hWnd);

    // Initialisation DirectX.
    InitD3D(_hWnd);

    // Initialisation ImGui.
    if (!InitImGui()) return false;

    return true;
}

BOOL UIWindow::InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Définition du style d'imgui.
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    style.WindowMinSize = ImVec2(20, 20);

    ImGui_ImplWin32_Init(_hWnd);
    ImGui_ImplDX9_Init(_d3ddev);

    return true;
}

VOID UIWindow::Run()
{
    // On entre dans la boucle principale.
    MSG msg = { 0 };

    // Attends le prochain message dans la file d'attente et stocke le résultat dans msg.
    while (msg.message != WM_QUIT)
    {
        // Vérifie tous les messages en attente.
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Traduit et transfert le message vers WindowProc().
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Update();
    }
}

VOID UIWindow::Update()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (_showDemoWindow) { ImGui::ShowDemoWindow(&_showDemoWindow); }

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Transmissions");

        ImGui::Text("This is some useful text."); // Utilisation possible de texte formaté.
        ImGui::Checkbox("Demo Window", &_showDemoWindow); // Edit bools storing our window open / close state.

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&_clearColor); // Edit 3 floats representing a color

        // Buttons return true when clicked (most widgets return true when edited / activated).
        if (ImGui::Button("Button")) counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // Rendering...
    ImGui::EndFrame();
    DrawScene();
}

VOID UIWindow::DrawScene()
{
    _d3ddev->SetRenderState(D3DRS_ZENABLE, FALSE);
    _d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    _d3ddev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    ImGuiIO& io = ImGui::GetIO();
    D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(_clearColor.x * _clearColor.w * 0.0f), (int)(_clearColor.y * _clearColor.w * 0.0f), (int)(_clearColor.z * _clearColor.w * 0.0f), (int)(_clearColor.w * 0.0f));

    // Nettoie le buffer.
    _d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

    if (_d3ddev->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        _d3ddev->EndScene();
    }

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    // Affiche la frame à l'écran.
    HRESULT result = _d3ddev->Present(NULL, NULL, NULL, NULL);

    // Gère la perte du dispositif D3D9.
    if (result == D3DERR_DEVICELOST && _d3ddev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) ResetDevice();
}

// Ceci est le message handler principal du programme.
LRESULT CALLBACK UIWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    // Fais le tri en exécutant le code selon le message.
    switch (message)
    {
    case WM_SIZE:
        if (_d3ddev != NULL && wParam != SIZE_MINIMIZED)
        {
            _width = LOWORD(lParam);
            _height = HIWORD(lParam);
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