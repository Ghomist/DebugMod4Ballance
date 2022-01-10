#include "../DebugMod.h"

#include "ListDataWindow.h"

ListDataWindow::ListDataWindow(CKContext* context) {
	InitializeTitle("List");
	this->_context = context;
	Fresh();
}

void ListDataWindow::Process() {
	if (!_active || !auto_fresh) return;
	Fresh();
}

void ListDataWindow::Render() {
	if (!_active) return;

	ImGui::Begin(title.c_str(), &_active, _window_flags);
	MenuBar();
	if (ImGui::BeginMenuBar()) {
		ImGui::Selectable("Auto-fresh", &auto_fresh);
		ImGui::EndMenuBar();
	}

	// Left
	// ImVec2(200, 0), true
	ImGui::BeginGroup();
	{
		ImGui::BeginChild("left pane", ImVec2(150, -ImGui::GetFrameHeightWithSpacing()), true);
		if (ImGui::BeginListBox("##type list", ImVec2(125, 150))) {
			for (int i = 0; i < all_type_cnt; ++i) {
				if (ImGui::Selectable(all_type[i], selected_type == i))
					selected_type = i;
			}
			ImGui::EndListBox();
		}
		ImGui::EndChild();
		if (!auto_fresh && ImGui::Button("Refresh")) {
			Fresh();
		}
		ImGui::EndGroup();
	}

	// Right
	ImGui::SameLine();
	ImGui::BeginGroup();
	{
		ImGui::Text("Filter");
		ImGui::SameLine();
		filter.Draw("Filter");
		ImGui::Separator();
		// Leave room for 1 line below us
		ImGui::BeginChild("item list", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
		{
			for (int i = 0; i < name_list.size(); ++i) {
				// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
				if (filter.PassFilter(name_list.at(i).c_str())) {
					if (ImGui::Selectable(name_list.at(i).c_str(), selected_obj == i, ImGuiSelectableFlags_AllowDoubleClick)) {
						selected_obj = i;
						if (ImGui::IsMouseDoubleClicked(0))
							OpenWindow();
					}
				}
			}
			ImGui::EndChild();
		}
		if (ImGui::Button("Inspect")) {
			OpenWindow();
		}
		ImGui::SameLine();
		if (ImGui::Button("Duplicate")) {
			// TODO
		}
		ImGui::SameLine();
		if (ImGui::Button("Destroy")) {
			_context->DestroyObject(
				_context->GetObjectByNameAndClass(
					name_list[selected_obj].c_str(),
					CKStringToClassID(all_type[selected_type])
				)
			);
		}
		ImGui::EndGroup();
	}

	ImGui::End();
}

const char* ListDataWindow::GetObjectName() {
	return all_type[selected_type];
}

void ListDataWindow::OpenWindow() {
	switch (selected_type) {
	case 0:
		DebugMod::GetActiveInstance()->AddDataWindow(new EntityDataWindow(
			(CK3dEntity*)_context->GetObjectByNameAndClass(
				name_list[selected_obj].c_str(),
				CKStringToClassID(all_type[selected_type])
			)
		));
		break;
	case 1:
		DebugMod::GetActiveInstance()->AddDataWindow(new ArrayDataWindow(
			(CKDataArray*)_context->GetObjectByNameAndClass(
				name_list[selected_obj].c_str(),
				CKStringToClassID(all_type[selected_type])
			)
		));
		break;
	case 2:
	case 3:
		DebugMod::GetActiveInstance()->AddDataWindow(new CameraDataWindow(
			(CKCamera*)_context->GetObjectByNameAndClass(
				name_list[selected_obj].c_str(),
				CKStringToClassID(all_type[selected_type])
			),
			selected_type == 3
		));
		break;
	case 4:
		DebugMod::GetActiveInstance()->AddDataWindow(new GroupDataWindow(
			(CKGroup*)_context->GetObjectByNameAndClass(
				name_list[selected_obj].c_str(),
				CKStringToClassID(all_type[selected_type])
			)
		));
		break;
	case 5:
	case 6:
		DebugMod::GetActiveInstance()->AddDataWindow(new LightDataWindow(
			(CKLight*)_context->GetObjectByNameAndClass(
				name_list[selected_obj].c_str(),
				CKStringToClassID(all_type[selected_type])
			),
			selected_type == 6
		));
		break;
	}
}

void ListDataWindow::Fresh() {
	CK_CLASSID c_id = CKStringToClassID(all_type[selected_type]);
	int cnt = _context->GetObjectsCountByClassID(c_id);
	CK_ID* IDs = _context->GetObjectsListByClassID(c_id);
	name_list.clear();
	for (int i = 0; i < cnt; ++i) {
		CKObject* obj = _context->GetObject(IDs[i]);
		if (obj != nullptr)
			name_list.emplace_back(obj->GetName());
	}
}