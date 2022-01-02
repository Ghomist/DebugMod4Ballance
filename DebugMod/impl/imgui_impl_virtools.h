#pragma once

#include "../Include.h"
#include "../DebugMod.h"

#include <d3d9.h>
#include <d3dx9.h>

#include <imgui.h>

// #include "kiero.h"
#include <minhook/include/MinHook.h>

typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice);
void InitImGui(LPDIRECT3DDEVICE9 pDevice);
int kieroExampleThread();
void InitializeKeyMap();
void ProcessGuiInput();