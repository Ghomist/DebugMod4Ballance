#pragma once

#include <imgui.h>
#include "DebugMod.h"

#define ENABLE_FOG_TOOLS true // If enable fog tools
#define ENABLE_RENDER_TOOLS true // If enable render tools

#define DEFAULT_SHOW_TIP false // If set show tip window as default

void Tip(const char* txt);
void ShowInfoWindow(bool* p_open);
void RenderMainMenuBar();