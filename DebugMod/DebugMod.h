#pragma once

#include "Include.h"

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}

class DebugMod : public IMod {
public:
	DebugMod(IBML* bml) : IMod(bml) { this_instance_ = this; }

	// Basic
	ILogger* Logger() { return GetLogger(); }

	// Control
	static DebugMod* GetActiveInstance() { return this_instance_; }

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
};
