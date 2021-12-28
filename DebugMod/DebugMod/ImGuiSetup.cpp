#include "ImGuiSetup.h"

void ImGuiSetup::Execute(IBML* bml, const std::vector<std::string>& args) {
	std::filesystem::copy("..\\ModLoader\\Cache\\Mods\\DebugMod.zip\\d3d8\\", "..\\bin");
	bml->SendIngameMessage("Set up dxd8.dll done! Please reboot Ballance to take effect.");
}

const std::vector<std::string> ImGuiSetup::GetTabCompletion(IBML* bml, const std::vector<std::string>& args) {
	return std::vector<std::string> {};
}
