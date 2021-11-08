#include "DX9.h"

DX9::DX9()
{

}

DX9::~DX9()
{
    CleanD3D();
}

// Cette fonction initialise et prépare l'utilisation de direct3d.
LPDIRECT3D9 DX9::InitD3D(HWND hWnd)
{
    _d3d = Direct3DCreate9(D3D_SDK_VERSION); // Créé l'interface Direct3D.

    ZeroMemory(&_d3dpp, sizeof(_d3dpp));
    _d3dpp.Windowed = TRUE;
    _d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // Se débarasse des frames précédentes (permutation).

    // Autres paramètres (voir documentation).
    _d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    _d3dpp.EnableAutoDepthStencil = TRUE;
    _d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    _d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Présent avec vsync, framerate capped.
    // d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Présent sans vsync, framerate maximal.

    // Crée le dispositif utilisant ces informations et ceux de la structure d3d parameters (d3dpp).
    _d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &_d3dpp, &_d3ddev);

    return _d3d;
}

VOID DX9::CleanD3D(void)
{
    // Libère la mémoire (et les pointeurs).
    if (_d3ddev) { _d3ddev->Release(); _d3ddev = NULL; }
    if (_d3d) { _d3d->Release(); _d3d = NULL; }
}

BOOL DX9::LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* output, int* outputWidth, int* outputHeight)
{
    // Charge la texture depuis le disque.
    PDIRECT3DTEXTURE9 texture;
    HRESULT hr = D3DXCreateTextureFromFileA(_d3ddev, filename, &texture);

    if (hr != S_OK) return false;

    // Récupère les infos de la texture afin de récupérer sa taille.
    D3DSURFACE_DESC imageDesc;
    texture->GetLevelDesc(0, &imageDesc);
    *output = texture;
    *outputWidth = (int)imageDesc.Width;
    *outputHeight = (int)imageDesc.Height;

    return true;
}

LPDIRECT3D9 DX9::GetDevice() {
    return _d3d;
}

LPDIRECT3DDEVICE9 DX9::GetDeviceContext() {
    return _d3ddev;
}