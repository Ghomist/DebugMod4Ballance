#pragma once

#include "DataWindow.h"

class LightDataWindow : public DataWindow {
public:
	LightDataWindow(CKLight* light, bool has_target);
	void Process() override;
	void Render() override;
	const char* GetObjectName() override;
private:
	bool activity;
	CKLight* light;
	bool has_target = false;
	VxVector pos;
	VxVector dir;
	VxVector up;
	VxQuaternion qua;
	VxVector ea;
	bool _radian = false;
	ImVec4 color;
	CKSTRING type;
	float power = 0.0f;
};