#include "ArrayDataWindow.h"

const char* CKArrayTypeToString(CK_ARRAYTYPE type);

ArrayDataWindow::ArrayDataWindow(CKDataArray* array) {
	InitializeTitle("Array");
	this->array = array;
	this->col_cnt = array->GetColumnCount();
	_window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
}

void ArrayDataWindow::Process() {
	if (!_active) return;

	if (array == nullptr) {
		_active = false;
		return;
	}

	key_col = array->GetKeyColumn();
	row_cnt = array->GetRowCount();
}

void ArrayDataWindow::Render() {
	if (!_active) return;

	ImGui::Begin(title.c_str(), &_active, _window_flags);
	MenuBar();

	ImGui::Text(array->GetName());
	ImGui::SameLine();
	ImGui::Text("Key column: %d", key_col);
	if (ImGui::BeginTable("table", col_cnt + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
		// Draw title
		ImGui::TableSetupColumn("Index");
		for (int c = 0; c < col_cnt; ++c) {
			char buf[50];
			sprintf(buf, "%d: %s", c, array->GetColumnName(c));
			ImGui::TableSetupColumn(buf);
		}
		ImGui::TableHeadersRow();

		// Draw data type
		for (int r = 0; r < row_cnt; ++r) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%d", r);
			for (int c = 0; c < col_cnt; ++c) {
				ImGui::TableSetColumnIndex(c + 1);
				// Get string to display
				char buf[200] = "";
				array->GetStringValue(*(array->GetElement(r, c)), c, buf);
				ImGui::TextWrapped(buf);
				if (ImGui::IsItemHovered()) {
					ImGui::BeginTooltip();
					ImGui::Text("%s\n%s", CKArrayTypeToString(array->GetColumnType(c)), buf);
					ImGui::EndTooltip();
				}
			}
		}
		ImGui::EndTable();
	}
	ImGui::End();
}

const char* ArrayDataWindow::GetObjectName() {
	return array->GetName();
}

const char* CKArrayTypeToString(CK_ARRAYTYPE type) {
	switch (type) {
	case CKARRAYTYPE_FLOAT: return "float";
	case CKARRAYTYPE_INT: return "int";
	case CKARRAYTYPE_OBJECT: return "object";
	case CKARRAYTYPE_PARAMETER: return "parameter";
	case CKARRAYTYPE_STRING: return "string";
	}
}
