#pragma once

#include <filesystem>

#include "DebugMod.h"

class ImGuiSetup : public ICommand {
public:
	virtual std::string GetName() override { return "setup_imgui_lib"; };
	virtual std::string GetAlias() override { return "imgui_lib_setup"; };
	virtual std::string GetDescription() override { return "Set up dxd8.dll for ImGui working correctly."; };
	virtual bool IsCheat() override { return false; };

	virtual void Execute(IBML* bml, const std::vector<std::string>& args) override;
	virtual const std::vector<std::string> GetTabCompletion(IBML* bml, const std::vector<std::string>& args) override;;
};