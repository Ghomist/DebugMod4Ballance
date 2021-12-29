#include "EntityDataWindow.h"
#include "../DebugMod.h"

EntityDataWindow::EntityDataWindow(CK3dEntity* entity) {
	InitializeTitle("3D Entity");
	this->entity = entity;
}

void EntityDataWindow::Process() {
	if (!_active) return;

	if (entity == nullptr) {
		_active = false;
		return;
	}

	// Speed array
	if (_view_speed) {
		// memcpy(&speed[0], &speed[1], 99);
		// memcpy(&speed_x[0], &speed_x[1], 99);
		// memcpy(&speed_y[0], &speed_y[1], 99);
		// memcpy(&speed_z[0], &speed_z[1], 99);
		for (int i = 99; i > 0; --i) {
			speed[i] = speed[i - 1];
			speed_x[i] = speed_x[i - 1];
			speed_y[i] = speed_y[i - 1];
			speed_z[i] = speed_z[i - 1];
		}
	}
	// Speed
	VxVector pos_2;
	entity->GetPosition(&pos_2);
	speed_x[0] = abs(pos.x - pos_2.x) / DebugMod::GetActiveInstance()->GetLastDeltaTime() * 1000;
	speed_y[0] = abs(pos.y - pos_2.y) / DebugMod::GetActiveInstance()->GetLastDeltaTime() * 1000;
	speed_z[0] = abs(pos.z - pos_2.z) / DebugMod::GetActiveInstance()->GetLastDeltaTime() * 1000;
	speed[0] = sqrt(speed_x[0] * speed_x[0] + speed_y[0] * speed_y[0] + speed_z[0] * speed_z[0]);
	float this_update_time = DebugMod::GetActiveInstance()->GetTime();

	// speed_x[0] = abs(pos.x - pos_2.x) / (this_update_time - last_update_time) * 1000;
	// speed_y[0] = abs(pos.y - pos_2.y) / (this_update_time - last_update_time) * 1000;
	// speed_z[0] = abs(pos.z - pos_2.z) / (this_update_time - last_update_time) * 1000;
	// speed[0] = sqrt(speed_x[0] * speed_x[0] + speed_y[0] * speed_y[0] + speed_z[0] * speed_z[0]);
	snprintf(speed_str_x, 50, "%.2f m/s", speed_x[0]);
	snprintf(speed_str_y, 50, "%.2f m/s", speed_y[0]);
	snprintf(speed_str_z, 50, "%.2f m/s", speed_z[0]);
	snprintf(speed_str, 50, "%.2f m/s", speed[0]);
	// Position
	// entity->GetPosition(&pos);
	pos.x = pos_2.x;
	pos.y = pos_2.y;
	pos.z = pos_2.z;
	// Orientation
	entity->GetOrientation(&dir, &up);
	dir *= 90;
	// Quaternion
	entity->GetQuaternion(&qua);
	// Angular velocity
	VxVector ea_2;
	qua.ToEulerAngles(&ea_2.x, &ea_2.y, &ea_2.z);
	ea.Absolute();
	ea_2.Absolute();
	angular_velocity = (ea_2 - ea) / DebugMod::GetActiveInstance()->GetLastDeltaTime() * 1000;
	// Rotation
	qua.ToEulerAngles(&ea.x, &ea.y, &ea.z);
	if (!_radian) ea *= 180 / 3.14159265;
	// Mesh
	mesh_count = entity->GetMeshCount();
	//
	last_update_time = this_update_time;
}

void EntityDataWindow::Render() {
	if (!_active) return;

	if (entity == nullptr) {
		ImGui::Begin(title.c_str(), &_active, _window_flags);
		ImGui::Text("No obj found");
		ImGui::End();
		return;
	}

	ImGui::Begin(title.c_str(), &_active, _window_flags);
	MenuBar();
	ImGui::Text("Name: %s", entity->GetName());
	ImGui::Separator();
	ImGui::Text("Position (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
	ImGui::Checkbox("View speed?", &_view_speed);
	if (_view_speed) {
		// ImGui::SameLine();
		ImGui::PlotLines("speed", speed, IM_ARRAYSIZE(speed), 0, speed_str, 0);
		ImGui::PlotLines("x", speed_x, IM_ARRAYSIZE(speed_x), 0, speed_str_x, 0);
		ImGui::PlotLines("y", speed_y, IM_ARRAYSIZE(speed_y), 0, speed_str_y, 0);
		ImGui::PlotLines("z", speed_z, IM_ARRAYSIZE(speed_z), 0, speed_str_z, 0);
	}
	else {
		ImGui::SameLine();
		ImGui::Text("Speed: %s", speed_str);
		ImGui::Text("Sub-speed: (%s, %s, %s)", speed_str_x, speed_str_y, speed_str_z);
	}
	ImGui::Text("Direction (%.2f, %.2f, %.2f)", dir.x, dir.y, dir.z);
	ImGui::Checkbox("EulerAngles (Radian?)", &_radian);
	ImGui::SameLine();
	ImGui::Text("(%.1f, %.1f, %.1f)%s", ea.x, ea.y, ea.z, _radian ? " rad" : "");
	ImGui::Separator();
	ImGui::Text("Mesh count: %d", mesh_count);
	// ?
	// ImGui::Text("Angular velocity (%.1f, %.1f, %.1f)", angular_velocity.x, data->angular_velocity.y, data->angular_velocity.z);
	ImGui::End();
}

const char* EntityDataWindow::GetObjectName() {
	return entity->GetName();
}
