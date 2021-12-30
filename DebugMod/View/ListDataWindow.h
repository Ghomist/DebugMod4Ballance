#pragma once

#include "DataWindow.h"

class ListDataWindow : public DataWindow {
public:
	ListDataWindow(CKContext* context);
	void Process() override;
	void Render() override;
	const char* GetObjectName() override;
	void OpenWindow();
	void Fresh();
private:
	CKContext* _context;
	std::vector<std::string> name_list;
	bool auto_fresh = true;
	const char all_type[8][20] = { "3D Object", "Array", "Camera", "Target Camera", "Group", "Light", "Target Light", "Curve" };
	const int all_type_cnt = 8;
	int selected_type = 0;
	int selected_obj = 0;

	ImGuiTextFilter filter;
};