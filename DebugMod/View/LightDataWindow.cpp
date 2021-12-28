#include "LightDataWindow.h"

LightDataWindow::LightDataWindow(CKLight* light, bool has_target) {
	InitializeTitle("Light");
	this->light = light;
}

void LightDataWindow::Process() {
	if (!_active) return;

	if (light == nullptr) {
		_active = false;
		return;
	}

	// Position
	light->GetPosition(&pos);
	// Orientation
	light->GetOrientation(&dir, &up);
	dir *= 90;
	// Quaternion
	light->GetQuaternion(&qua);
	// Rotation
	qua.ToEulerAngles(&ea.x, &ea.y, &ea.z);
	if (!_radian) ea *= 180 / 3.1415;
	// Activity
	activity = light->GetActivity();
	// Type
	switch (light->GetType()) {
	case VX_LIGHTPOINT: type = "Point"; break;
	case VX_LIGHTSPOT: type = "Spot"; break;
	case VX_LIGHTDIREC: type = "Directional"; break;
	case VX_LIGHTPARA: type = "??"; break;
	}
	// Color
	VxColor _color = light->GetColor();
	color.x = _color.r;
	color.y = _color.g;
	color.z = _color.b;
	color.w = _color.a;
	// Power
	power = light->GetLightPower();
}

void LightDataWindow::Render() {
	if (!_active) return;

	ImGui::Begin(title.c_str(), &_active, _window_flags);
	MenuBar();
	ImGui::Text("Name: %s", light->GetName());
	ImGui::SameLine(); ImGui::Text("Activity: %s", activity ? "on" : "off");
	ImGui::Text("Type: %s", type);
	ImGui::Text("Target: %s", has_target ? light->GetTarget()->GetName() : "NA");
	ImGui::Separator();
	ImGui::Text("Position (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
	ImGui::Text("Dir (%.2f, %.2f, %.2f)", dir.x, dir.y, dir.z);
	ImGui::Checkbox("Radian?", &_radian);
	ImGui::Text("EulerAngles (%.1f, %.1f, %.1f)", ea.x, ea.y, ea.z);
	ImGui::Separator();
	ImGui::Text("Color: ");
	ImGui::SameLine(); ImGui::ColorButton("Color", color);
	ImGui::Text("Power: %f", power);
	ImGui::End();
}

const char* LightDataWindow::GetObjectName() {
	return light->GetName();
}
