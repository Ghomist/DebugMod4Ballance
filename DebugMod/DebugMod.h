#pragma once

#include "Include.h"
#include "View/AllView.h"

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}

class DebugMod : public IMod {
public:
	DebugMod(IBML* bml) : IMod(bml) { this_instance_ = this; }

	// Basic
	ILogger* Logger() { return GetLogger(); }
	CK3dEntity* GetEntity(CKSTRING name) { return m_bml->Get3dObjectByName(name); }
	CKDataArray* GetArray(CKSTRING name) { return m_bml->GetArrayByName(name); }
	CKGroup* GetGroup(CKSTRING name) { return m_bml->GetGroupByName(name); }
	CKContext* GetCKContext() { return m_bml->GetCKContext(); }
	CKRenderContext* GetCKRenderContext() { return m_bml->GetRenderContext(); }
	CKCollisionManager* GetCollisionManager() { return m_bml->GetCollisionManager(); }
	CKInputManager* GetInputManager() { return m_bml->GetInputManager(); }
	unsigned char* GetKeyboardState() { return m_bml->GetInputManager()->GetKeyboardState(); }
	CKDWORD GetCurrentRenderOptions() { return m_bml->GetRenderContext()->GetCurrentRenderOptions(); }
	float GetTime() { return _time_manager->GetTime(); }
	float GetLastDeltaTime() { return _time_manager->GetLastDeltaTime(); }

	// Render options
	void SetGlobalShadeMode(VxShadeType type, bool textures, bool wireframe);
	void SetCurrentRenderOptions(CKDWORD flags);
	CKDWORD GetAmbientLight();
	void SetAmbientLight(float r, float g, float b);
	void SetFog(VXFOG_MODE mode, float start, float end, float density, int a, int r, int g, int b);

	// imgui
	void RenderAll();
	void AddDataWindow(DataWindow* _data);
	void PopDataWindow(int index);
	void PopAllDataWindows();
	std::vector<DataWindow*>* GetAllDataWindows() { return &all_data_windows; }
	bool* MainMenuEnable() { return &show_main_menu; }
	bool* AlwaysTopMostEnable() { return &always_top_most; }

	// Trafo display
	bool* TrafoDisplay() { return &display_trafo_trigger; }

	// Control
	static DebugMod* GetActiveInstance() { return this_instance_; }
	bool ReachNextProcess();

	// RSC
	void LoadResource(XString filename, CKObjectArray* arr);

	void AddAllToScene(CKObjectArray* arr);

	// On events
	virtual void OnLoadObject(CKSTRING filename, BOOL isMap, CKSTRING masterName,
		CK_CLASSID filterClass, BOOL addtoscene, BOOL reuseMeshes, BOOL reuseMaterials,
		BOOL dynamic, XObjectArray* objArray, CKObject* masterObj) override;
	virtual void OnLoad() override;
	virtual void OnStartLevel() override;
	virtual void OnPostLoadLevel() override;
	virtual void OnRender(CK_RENDER_FLAGS flags) override;
	virtual void OnProcess() override;
	virtual void OnExitGame() override;

	virtual CKSTRING GetID() override { return "DebugMod"; }
	virtual CKSTRING GetVersion() override { return BML_VERSION; }
	virtual CKSTRING GetName() override { return "Debug Mod"; }
	virtual CKSTRING GetAuthor() override { return "Ghomist"; }
	virtual CKSTRING GetDescription() override { return "Debugger of Ballance Developer."; }
	DECLARE_BML_VERSION;

private:
	// IProperty* props[2];

	CKTimeManager* _time_manager = m_bml->GetTimeManager();

	float _time_limit = 100.0f; // default: 100 ms
	float _current_time;
	float _last_time;

	static inline DebugMod* this_instance_ = nullptr;
	std::vector<DataWindow*> all_data_windows;

	bool show_main_menu = true;

	bool always_top_most = false;

	bool display_trafo_trigger = true;

	CKObjectArray* all_triger_display = CreateCKObjectArray();
	CKObjectArray* all_test_floor = CreateCKObjectArray();
	CKObjectArray* all_trace = CreateCKObjectArray();
};
