#include <filesystem>

#include "Include.h"
#include "DebugCmd.h"
#include "DebugMod.h"
#include "ImGuiSetup.h"
#include "MainMenu.h"
#include "impl/imgui_impl_virtools.h"

extern HWND window;

IMod* BMLEntry(IBML* bml) {
	return new DebugMod(bml);
}

void DebugMod::OnLoadObject(CKSTRING filename, BOOL isMap, CKSTRING masterName, CK_CLASSID filterClass, BOOL addtoscene, BOOL reuseMeshes, BOOL reuseMaterials, BOOL dynamic, XObjectArray* objArray, CKObject* masterObj) { }

void DebugMod::OnLoad() {
	// Info
	GetLogger()->Info("Loading [Debug Mod] for Ballance");
	GetLogger()->Info("v2");

	// Configuration
	// props[0] = GetConfig()->GetProperty("Integers", "open_tips")

	// Commands
	m_bml->RegisterCommand(new DebugCmd());
	m_bml->RegisterCommand(new ImGuiSetup());

	// Create thread to setup render hook
	window = (HWND)m_bml->GetRenderContext()->GetWindowHandle(); // Get window's handle
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)kieroExampleThread, NULL, 0, NULL); // Hook thread
}

void DebugMod::OnStartLevel() {}

void DebugMod::OnPostLoadLevel() {}

void DebugMod::OnProcess() {
	// Cursor display
	m_bml->GetInputManager()->ShowCursor(TRUE);

	//GetLogger()->Info("%d", IsWindowTopMost());
	//GetLogger()->Info("%d", GetFocus() == window);
	InputHook::SetBlock(window != GetFocus());
}

void DebugMod::OnRender(CK_RENDER_FLAGS flags) {}

void DebugMod::OnExitGame() {
	// Remove all cache files
	// std::filesystem::remove("..\\ModLoader\\Cache\\Mods\\DebugMod.zip\\");
}
