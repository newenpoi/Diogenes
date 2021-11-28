#pragma once

#include "DX9.h"

struct Color
{
	static constexpr D3DCOLOR Black() { return D3DCOLOR_RGBA(0, 0, 0, 0); }
	static constexpr D3DCOLOR White() { return D3DCOLOR_RGBA(255, 255, 255, 0); }
};