#pragma once

#include "DataWindow.h"

class CameraDataWindow : public DataWindow {
public:
	CameraDataWindow(CKCamera* camera, bool has_target);
	void Process() override;
	void Render() override;
	const char* GetObjectName() override;
private:
	CKCamera* camera;
	bool has_target;
	VxVector pos = VxVector(0, 0, 0);
	VxVector dir = VxVector(0, 0, 0);
	VxVector up = VxVector(0, 0, 0);
	bool _radian = false;
	VxQuaternion qua = VxQuaternion(0, 0, 0, 0);
	VxVector ea = VxVector(0, 0, 0);
	float fov = 0.0f;
	//float last_update_time = 0.0f;
};