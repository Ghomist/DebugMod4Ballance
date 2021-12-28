#pragma once

#include <BML/BMLAll.h>

#include "imgui.h"

static ImVec4 font_color = ImVec4(255, 255, 255, 255);
static int windows_cnt = 1;

class DataWindow {
public:
	virtual void Process() = 0;
	virtual void Render() = 0;
	virtual const char* GetObjectName() = 0;

	void InitializeTitle(const char* type);
	const char* GetTitle();

	bool IsActive();
	void ChangeActiveState();

	void MenuBar();
protected:
	std::string title;
	bool _active = true;
	int _window_flags = 1 << 10; // Default open the menu-bar
};