#pragma once

#include "Include.h"
#include "View/AllView.h"

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}

class DebugMod : public IMod {
public:
	DebugMod(IBML* bml) : IMod(bml) { this_instance_ = this; }

	ILogger* Logger();
	bool ReachNextProcess();
	unsigned char* GetKeyboardState();
	CK3dEntity* GetEntity(CKSTRING name);
	CKDataArray* GetArray(CKSTRING name);
	CKGroup* GetGroup(CKSTRING name);
	void AddDataWindow(DataWindow* _data);
	void PopDataWindow(int index);
	void PopAllDataWindows();
	std::vector<DataWindow*> GetAllDataWindows();
	CKContext* GetCKContext();
	CKRenderContext* GetCKRenderContext();
	CKCollisionManager* GetCollisionManager();
	CKInputManager* GetInputManager();
	float GetTime();
	float GetLastDeltaTime();
	static DebugMod* GetActiveInstance();
	bool IsActiveMainMenu();
	void ShowMainMenu();
	void HideMainMenu();
	void SetGlobalShadeMode(VxShadeType type, bool textures, bool wireframe);
	CKDWORD GetCurrentRenderOptions();
	void SetCurrentRenderOptions(CKDWORD flags);
	CKDWORD GetAmbientLight();
	void SetAmbientLight(float r, float g, float b);
	void SetFog(VXFOG_MODE mode, float start, float end, float density, int a, int r, int g, int b);

	virtual CKSTRING GetID() override { return "DebugMod"; }
	virtual CKSTRING GetVersion() override { return BML_VERSION; }
	virtual CKSTRING GetName() override { return "Debug Mod"; }
	virtual CKSTRING GetAuthor() override { return "Ghomist"; }
	virtual CKSTRING GetDescription() override { return "Debugger of Ballance Developer."; }
	DECLARE_BML_VERSION;
	virtual void OnLoad() override;
	virtual void OnStartLevel() override;
	virtual void OnRender(CK_RENDER_FLAGS flags) override;
	virtual void OnProcess() override;
	virtual void OnExitGame() override;

private:
	// IProperty* props[2];

	CKTimeManager* _time_manager = m_bml->GetTimeManager();

	float _time_limit = 100.0f; // default: 100 ms
	float _current_time;
	float _last_time;

	static inline DebugMod* this_instance_ = nullptr;
	std::vector<DataWindow*> all_data_windows;

	bool show_main_menu = true;

	CKCurve* m_curve;
};
