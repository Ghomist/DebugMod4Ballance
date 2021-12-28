#include "CameraDataWindow.h"

CameraDataWindow::CameraDataWindow(CKCamera* camera, bool has_target) {
	InitializeTitle("Camera");
	this->camera = camera;
	this->has_target = has_target;
}

void CameraDataWindow::Process() {
	if (!_active) return;

	if (camera == nullptr) {
		_active = false;
		return;
	}

	// Position
	camera->GetPosition(&pos);
	// Orientation
	camera->GetOrientation(&dir, &up);
	dir *= 90;
	// Quaternion
	camera->GetQuaternion(&qua);
	// Rotation
	qua.ToEulerAngles(&ea.x, &ea.y, &ea.z);
	if (!_radian) ea *= 180 / 3.1415;
	// Fov
	fov = camera->GetFov();
}

void CameraDataWindow::Render() {
	if (!_active) return;

	ImGui::Begin(title.c_str(), &_active, _window_flags);
	MenuBar();
	ImGui::Text("Name: %s", camera->GetName());
	ImGui::Text("Target: %s", has_target ? camera->GetTarget()->GetName() : "NA");
	ImGui::Separator();
	ImGui::Text("Position (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
	ImGui::Text("Dir (%.2f, %.2f, %.2f)", dir.x, dir.y, dir.z);
	ImGui::Checkbox("Radian?", &_radian);
	ImGui::Text("EulerAngles (%.1f, %.1f, %.1f)", ea.x, ea.y, ea.z);
	ImGui::Separator();
	ImGui::Text("Fov: %f", fov);
	ImGui::End();
}

const char* CameraDataWindow::GetObjectName() {
	return camera->GetName();
}
