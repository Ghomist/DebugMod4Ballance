#include "DataWindow.h"

void DataWindow::MenuBar() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Options")) {
			if (ImGui::MenuItem("Auto resize", "", (_window_flags & ImGuiWindowFlags_AlwaysAutoResize) != 0))
				_window_flags ^= ImGuiWindowFlags_AlwaysAutoResize;
			if (ImGui::MenuItem("No background", "", (_window_flags & ImGuiWindowFlags_NoBackground) != 0))
				_window_flags ^= ImGuiWindowFlags_NoBackground;
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void DataWindow::InitializeTitle(const char* type) {
	title.clear();
	title.append(type).append("##").append(std::to_string(windows_cnt++));
}

bool DataWindow::IsActive() {
	return _active;
}

void DataWindow::ChangeActiveState() {
	_active = !_active;
}

const char* DataWindow::GetTitle() {
	return title.c_str();
}