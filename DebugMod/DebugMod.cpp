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

bool DebugMod::ReachNextProcess() {
	_current_time = _time_manager->GetTime();
	if (_current_time > _last_time + _time_limit) {
		_last_time = _current_time;
		return true;
	}
	return false;
}

void DebugMod::RenderAll() {
	ImGui::NewFrame();

	// Render main menu bar
	if (show_main_menu) RenderMainMenuBar();

	// Update and render all data windows
	for (DataWindow* data : all_data_windows) {
		data->Process();
		data->Render();
	}

	ImGui::EndFrame();
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
		delete win; // Release?
	all_data_windows.clear();
}

std::vector<DataWindow*> DebugMod::GetAllDataWindows() {
	return all_data_windows;
}

void DebugMod::SetGlobalShadeMode(VxShadeType type, bool use_textures, bool wireframe) {
	m_bml->GetRenderContext()->SetGlobalRenderMode(type, use_textures, wireframe);
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

void DebugMod::OnLoadObject(CKSTRING filename, BOOL isMap, CKSTRING masterName, CK_CLASSID filterClass, BOOL addtoscene, BOOL reuseMeshes, BOOL reuseMaterials, BOOL dynamic, XObjectArray* objArray, CKObject* masterObj) {
	// Display trafo trigger
	if (isMap && display_trafo_trigger) {
		CK3dObject* oDisplay = m_bml->Get3dObjectByName("Triger_display");
		CKTexture* t = m_bml->GetTextureByName("Triger_display_texture");
		if (t == nullptr || oDisplay == nullptr) return;

		t->SetDesiredVideoFormat(_32_ARGB8888);

		// CK_OBJECTCREATION_OPTIONS opt = CK_OBJECTCREATION_DYNAMIC | CK_OBJECTCREATION_RENAME;
		CKDependencies dep;
		dep.Resize(40); dep.Fill(0);
		dep.m_Flags = CK_DEPENDENCIES_CUSTOM;
		dep[CKCID_OBJECT] = CK_DEPENDENCIES_COPY_OBJECT_NAME | CK_DEPENDENCIES_COPY_OBJECT_UNIQUENAME;
		dep[CKCID_MESH] = CK_DEPENDENCIES_COPY_MESH_MATERIAL;
		dep[CKCID_3DENTITY] = CK_DEPENDENCIES_COPY_3DENTITY_MESH;

		// Find all trafos
		std::string all_trafos[] = { "P_Trafo_Wood", "P_Trafo_Paper", "P_Trafo_Stone" };
		for (int t = 0; t < 3; ++t) {
			CKGroup* trafos = m_bml->GetGroupByName(all_trafos[t].c_str());

			for (int i = 0; i < trafos->GetObjectCount(); ++i) {
				CK3dObject* obj = (CK3dObject*)trafos->GetObject(i);
				if (obj == nullptr) continue;

				// Get position
				VxVector pos;
				obj->GetPosition(&pos);

				// Set display
				CK3dObject* cp = (CK3dObject*)m_bml->GetCKContext()->CopyObject(oDisplay, &dep, "_copied"); // &dep, 0, CK_OBJECTCREATION_RENAME);
				cp->SetPosition(pos);
				//cp->GetCurrentMesh()->GetMaterial(0)->SetDiffuse(VxColor(0, 0, 0, 0));
				cp->Show();
			}
		}
		GetLogger()->Info("Trafo display enable");
	}
	// Display all the loaded files
	// GetLogger()->Info(filename);
}

void DebugMod::OnLoad() {
	// Info
	GetLogger()->Info("Loading [Debug Mod] for Ballance");

	// Configuration
	// props[0] = GetConfig()->GetProperty("Integers", "open_tips")

	// Commands
	m_bml->RegisterCommand(new DebugCmd());
	m_bml->RegisterCommand(new ImGuiSetup());

	// Create thread to setup render hook
	window = (HWND)m_bml->GetRenderContext()->GetWindowHandle(); // Get window's handle
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)kieroExampleThread, NULL, 0, NULL); // Hook thread

	GetLogger()->Info("Loading resources..."); // Load RCS
	LoadResource("3D Entities\\PH\\TrafoTriggerDisplay.nmo", all_triger_display);
	LoadResource("3D Entities\\Trace.nmo", all_trace);
}

void DebugMod::LoadResource(XString filename, CKObjectArray* arr) {
	XString oName(filename);
	m_bml->GetPathManager()->ResolveFileName(filename, DATA_PATH_IDX, -1); // Resolve
	CKERROR err = m_bml->GetCKContext()->Load(filename.Str(), arr);
	if (err == CK_OK)
		GetLogger()->Info("%s loaded", oName.Str());
	else
		GetLogger()->Error("%s: %d", oName.Str(), err);
}

void DebugMod::OnStartLevel() {
	{ // Trace enable
		m_bml->GetCKContext()->GetObjectByNameAndClass("Trace", CKCID_CURVE)->Show();
		CKBehavior* beh = m_bml->GetScriptByName("Trace_script");
		beh->Activate();
		if (beh->IsActive())
			GetLogger()->Info("Trace enable");
	}
}

void DebugMod::OnPostLoadLevel() {}

void DebugMod::OnProcess() {
	if (m_bml->IsPlaying())
		m_bml->GetInputManager()->ShowCursor(TRUE);
}

void DebugMod::OnRender(CK_RENDER_FLAGS flags) {}

void DebugMod::OnExitGame() {
	// Remove all cache files
	// std::filesystem::remove("..\\ModLoader\\Cache\\Mods\\DebugMod.zip\\");
}
