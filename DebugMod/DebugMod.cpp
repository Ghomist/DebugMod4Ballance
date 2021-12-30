#include <filesystem>

#include "Include.h"
#include "DebugCmd.h"
#include "DebugMod.h"
#include "ImGuiSetup.h"
#include "imgui_impl_virtools.h"

extern HWND window;

IMod* BMLEntry(IBML* bml) {
	return new DebugMod(bml);
}

ILogger* DebugMod::Logger() {
	return GetLogger();
}

bool DebugMod::ReachNextProcess() {
	_current_time = _time_manager->GetTime();
	if (_current_time > _last_time + _time_limit) {
		_last_time = _current_time;
		return true;
	}
	return false;
}

unsigned char* DebugMod::GetKeyboardState() {
	return m_bml->GetInputManager()->GetKeyboardState();
}

CK3dEntity* DebugMod::GetEntity(CKSTRING name) {
	return m_bml->Get3dObjectByName(name);
}

CKDataArray* DebugMod::GetArray(CKSTRING name) {
	return m_bml->GetArrayByName(name);
}

CKGroup* DebugMod::GetGroup(CKSTRING name) {
	return m_bml->GetGroupByName(name);
}

void DebugMod::AddDataWindow(DataWindow* _data) {
	all_data_windows.emplace_back(_data);
}

void DebugMod::PopDataWindow(int index) {
	delete all_data_windows[index];
	all_data_windows.erase(all_data_windows.begin() + index);
}

void DebugMod::PopAllDataWindows() {
	for (DataWindow* win : all_data_windows)
		delete win;
	all_data_windows.clear();
}

std::vector<DataWindow*> DebugMod::GetAllDataWindows() {
	return all_data_windows;
}

CKContext* DebugMod::GetCKContext() {
	return m_bml->GetCKContext();
}

CKRenderContext* DebugMod::GetCKRenderContext() {
	return m_bml->GetRenderContext();
}

CKCollisionManager* DebugMod::GetCollisionManager() {
	return m_bml->GetCollisionManager();
}

CKInputManager* DebugMod::GetInputManager() {
	return m_bml->GetInputManager();
}

float DebugMod::GetTime() {
	return _time_manager->GetTime();
}

float DebugMod::GetLastDeltaTime() {
	return _time_manager->GetLastDeltaTime();
}

DebugMod* DebugMod::GetActiveInstance() {
	return this_instance_;
}

bool DebugMod::IsActiveMainMenu() {
	return show_main_menu;
}

void DebugMod::ShowMainMenu() {
	show_main_menu = true;
}

void DebugMod::HideMainMenu() {
	show_main_menu = false;
}

void DebugMod::SetGlobalShadeMode(VxShadeType type, bool textures, bool wireframe) {
	m_bml->GetRenderContext()->SetGlobalRenderMode(type, textures, wireframe);
	// m_bml->GetRenderContext()->Render();
}

CKDWORD DebugMod::GetCurrentRenderOptions() {
	return m_bml->GetRenderContext()->GetCurrentRenderOptions();
}

void DebugMod::SetCurrentRenderOptions(CKDWORD flags) {
	m_bml->GetRenderContext()->SetCurrentRenderOptions(flags);
}

CKDWORD DebugMod::GetAmbientLight() {
	return m_bml->GetRenderContext()->GetAmbientLight();
}

void DebugMod::SetAmbientLight(float r, float g, float b) {
	m_bml->GetRenderContext()->SetAmbientLight(r, g, b);
}

void DebugMod::SetFog(VXFOG_MODE mode, float start, float end, float density, int a, int r, int g, int b) {
	CKRenderContext* rc = m_bml->GetRenderContext();
	rc->SetFogMode(mode);
	rc->SetFogStart(start);
	rc->SetFogEnd(end);
	rc->SetFogDensity(density);
	CKDWORD color = 0;
	color = ColorSetAlpha(color, a);
	color = ColorSetRed(color, r);
	color = ColorSetGreen(color, g);
	color = ColorSetBlue(color, b);
	rc->SetFogColor(color);
}

void DebugMod::OnLoad() {
	GetLogger()->Info(GetDescription());
	GetLogger()->Info(GetAuthor());

	// props[0] = GetConfig()->GetProperty("Integers", "open_tips")

	m_bml->RegisterCommand(new DebugCmd());
	m_bml->RegisterCommand(new ImGuiSetup());

	window = (HWND)m_bml->GetRenderContext()->GetWindowHandle();
	// ?
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)kieroExampleThread, NULL, 0, NULL);
}

void DebugMod::OnStartLevel() {
	// m_curve = (CKCurve*)m_bml->GetCKContext()->CreateObject(CKCID_CURVE, "unique curve");
	// (CKCurve*)m_bml->GetCKContext()->GetObjectByName("unique curve");
}

void DebugMod::OnProcess() {
	// Every frame
	ProcessGuiInput();

	if (!m_bml->IsPlaying()) return;
	// While playing
	m_bml->GetInputManager()->ShowCursor(TRUE);

}

VxVector pos = VxVector(0, 0, 0);
void DebugMod::OnRender(CK_RENDER_FLAGS flags) {
	// if (!m_bml->IsPlaying()) return;
	// CK3dObject* player_ball = m_bml->Get3dObjectByName("Ball_Wood");
	// player_ball->GetPosition(&pos);
	// m_bml->GetRenderManager()->RegisterPoint(pos, 0);
	// m_bml->GetRenderManager()->DrawPoint(m_bml->GetRenderContext(), VxVector(0, 0, 0), 0);
}

void DebugMod::OnExitGame() {
	// std::filesystem::remove("..\\ModLoader\\Cache\\Mods\\DebugMod.zip\\");
}
