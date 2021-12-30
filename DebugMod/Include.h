#pragma once

#include "View/AllView.h"

#include <BML/BMLAll.h>

#include <imgui.h>
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>
#include "imgui_impl_virtools.h"
#include <d3d9.h>
#include <d3dx9.h>
// #include <tchar.h>
// #include <thread>
// #include <chrono>
// #include "minhook/include/MinHook.h"

#include <kiero.h>
#if defined (KIERO_INCLUDE_D3D9)
# undef KIERO_INCLUDE_D3D9
# define KIERO_INCLUDE_D3D9 1
#else
# define KIERO_INCLUDE_D3D9 1
#endif // KIERO_INCLUDE_D3D9

#if defined (KIERO_USE_MINHOOK)
# undef KIERO_USE_MINHOOK
# define KIERO_USE_MINHOOK 1
#else
# define KIERO_USE_MINHOOK 1
#endif // KIERO_INCLUDE_D3D9

#define KIERO_INCLUDE_D3D9 1
#define KIERO_USE_MINHOOK 1
