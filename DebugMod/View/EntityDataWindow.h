#pragma once

#include "DataWindow.h"

class EntityDataWindow : public DataWindow {
public:
	EntityDataWindow(CK3dEntity* entity);
	void Process() override;
	void Render() override;
	const char* GetObjectName() override;
private:
	CK3dEntity* entity;
	VxVector pos = VxVector(0, 0, 0);
	VxVector dir = VxVector(0, 0, 0);
	VxVector up = VxVector(0, 0, 0);
	bool _radian = false;
	VxQuaternion qua = VxQuaternion(0, 0, 0, 0);
	VxVector ea = VxVector(0, 0, 0);
	float last_update_time = 0.0f;
	bool _view_speed = false;
	float speed[100];
	float speed_x[100];
	float speed_y[100];
	float speed_z[100];
	char speed_str[50] = "";
	char speed_str_x[50] = "";
	char speed_str_y[50] = "";
	char speed_str_z[50] = "";
	VxVector angular_velocity = VxVector(0, 0, 0); // xx rad/s
	bool has_impact = false;
	ImpactDesc desc;
	int mesh_count = 0;
};