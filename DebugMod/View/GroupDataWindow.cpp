#include "GroupDataWindow.h"

GroupDataWindow::GroupDataWindow(CKGroup* group) {
	InitializeTitle("Group");
	this->group = group;
	Process();
}

void GroupDataWindow::Process() {
	if (!_active) return;

	if (group == nullptr) {
		_active = false;
		return;
	}

	// Count
	group_cnt = group->GetObjectCount();
	// Name list
	name_list.clear();
	type_list.clear();
	for (int i = 0; i < group_cnt; ++i) {
		CKBeObject* obj = group->GetObject(i);
		name_list.emplace_back(obj->GetName());
		type_list.emplace_back(CKClassIDToString(obj->GetClassID()));
	}
}

void GroupDataWindow::Render() {
	if (!_active) return;

	ImGui::Begin(title.c_str(), &_active, _window_flags);
	MenuBar();
	ImGui::Text("%s (%d)", group->GetName(), group_cnt);
	if (ImGui::BeginTable("group items", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
		// Draw title
		ImGui::TableSetupColumn("Index");
		ImGui::TableSetupColumn("Class");
		ImGui::TableSetupColumn("Name");
		ImGui::TableHeadersRow();

		// Draw data type
		for (int i = 0; i < group_cnt; ++i) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%d", i);

			ImGui::TableSetColumnIndex(1);
			ImGui::Text(type_list[i].c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::Text(name_list[i].c_str());
		}
		ImGui::EndTable();
	}
	ImGui::End();
}

const char* GroupDataWindow::GetObjectName() {
	return group->GetName();
}
