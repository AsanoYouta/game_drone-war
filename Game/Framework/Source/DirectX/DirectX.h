#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include<d3d11.h>
#include<d3dcompiler.h>
#include<DirectXTex.h>
#include<DirectXMath.h>
using namespace DirectX;

#include<wrl/client.h>
using Microsoft::WRL::ComPtr;

#include "Direct3D.h"
#include "Source/Game/Texture.h"
#include "Source/DirectX/Direct2D.h"
#include "Direct2D.h"
