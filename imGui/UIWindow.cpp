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

UIWindow::UIWindow() : _x(0), _y(0), _width(0), _height(0), _hInstance(0), _hWnd(0), _showDemoWindow(false), _showMainWindow(true)
{
    uiWindow = this;
}

UIWindow::~UIWindow()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    DestroyWindow(_hWnd);
}

VOID UIWindow::ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    // DIRECTX9.
    
    // HRESULT hr = _d3ddev->Reset(&_d3dpp);
    // if (hr == D3DERR_INVALIDCALL) IM_ASSERT(0);
    
    ImGui_ImplDX9_CreateDeviceObjects();
}

BOOL UIWindow::Init(UINT x, UINT y, UINT width, UINT height)
{
    WNDCLASSEX wc; // Détiens les informations de la fenêtre (window class).

    // Nettoie le bloc de mémoire correspondant à la classe.
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
    DIRECTX9.InitD3D(_hWnd);

    // Initialisation ImGui.
    if (!InitImGui()) return false;

    return true;
}

BOOL UIWindow::InitImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Active la gestion du clavier.
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Active la gestion d'ancrage des fenêtres.
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Active les fenêtres multi-vues / plate-formes.

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
    ImGui_ImplDX9_Init(DIRECTX9.GetDeviceContext());

    // Charge le/les fonts.
    SetFontList();

    // Charge la texture (pour la fenêtre Monica).
    _imageWidth = 0;
    _imageHeight = 0;
    _texture = NULL;
    bool ret = DIRECTX9.LoadTextureFromFile("texture/monika.jpg", &_texture, &_imageWidth, &_imageHeight);
    IM_ASSERT(ret != NULL && "Erreur lors de la lecture de la texture.");

    return true;
}

BOOL UIWindow::SetFontList()
{
    /*
        Initialise une police compatible avec les caractères exotiques.
    */

    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;

    config.OversampleH = 1;
    config.OversampleV = 1;

    io.Fonts->AddFontFromFileTTF("font/VT323-Regular.ttf", 18, &config, io.Fonts->GetGlyphRangesJapanese());

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

    if (!_showMainWindow) exit(1);

    if (_showDemoWindow) ImGui::ShowDemoWindow(&_showDemoWindow);

    {
        ImGui::Begin("Diogenes", &_showMainWindow, ImGuiWindowFlags_NoResize);
        ImGui::SetWindowSize(ImVec2(256, 512));

        // On peut également utiliser du texte formaté.
        ImGui::TextWrapped(u8"Initialisation du dispositif DirectX terminé.\nEn attente...");

        // Afficher la démo.
        ImGui::Checkbox(u8"Démonstration", &_showDemoWindow);

        ImGui::Text("Latence CPU %.3f MS/Frame.\nIPS %.1f", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }

    {
        ImGui::Begin("Display");
        ImGui::Text(u8"Adresse Mémoire de la Texture : "); ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 1, 1), "%p", _texture);
        ImGui::Image((void*)_texture, ImVec2(_imageWidth, _imageHeight));
        ImGui::End();
    }

    // Rendering...
    ImGui::EndFrame();
    DrawScene();
}

VOID UIWindow::DrawScene()
{
    DIRECTX9.GetDeviceContext()->SetRenderState(D3DRS_ZENABLE, FALSE);
    DIRECTX9.GetDeviceContext()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    DIRECTX9.GetDeviceContext()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    ImGuiIO& io = ImGui::GetIO();
    D3DCOLOR clear_col_dx = Color::Black();

    // Nettoie le buffer.
    DIRECTX9.GetDeviceContext()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

    if (DIRECTX9.GetDeviceContext()->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        DIRECTX9.GetDeviceContext()->EndScene();
    }

    // Mets à jour et affiche les fenêtres supplémentaires.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    // Affiche la frame à l'écran.
    HRESULT result = DIRECTX9.GetDeviceContext()->Present(NULL, NULL, NULL, NULL);

    // Gère la perte du dispositif D3D9.
    if (result == D3DERR_DEVICELOST && DIRECTX9.GetDeviceContext()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) ResetDevice();
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
        if (DIRECTX9.GetDeviceContext() != NULL && wParam != SIZE_MINIMIZED)
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