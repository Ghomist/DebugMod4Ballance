#pragma once

#include "DataWindow.h"

class GroupDataWindow : public DataWindow {
public:
	GroupDataWindow(CKGroup* group);
	void Process() override;
	void Render() override;
	const char* GetObjectName() override;
private:
	CKGroup* group;
	int group_cnt;
	std::vector<std::string> name_list;
	std::vector<std::string> type_list;
};