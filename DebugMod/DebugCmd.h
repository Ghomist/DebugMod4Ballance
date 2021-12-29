#pragma once

#include "Include.h"
#include "DebugMod.h"

class DebugCmd : public ICommand {
public:
	virtual std::string GetName() override { return "debug"; };
	virtual std::string GetAlias() override { return "d"; };
	virtual std::string GetDescription() override { return "Do u want a debug window?"; };
	virtual bool IsCheat() override { return false; };

	virtual void Execute(IBML* bml, const std::vector<std::string>& args) override;
	virtual const std::vector<std::string> GetTabCompletion(IBML* bml, const std::vector<std::string>& args) override;;
};