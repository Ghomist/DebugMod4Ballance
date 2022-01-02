#include "DebugCmd.h"

void DebugCmd::Execute(IBML* bml, const std::vector<std::string>& args) {
	// D xxx
	if (args.size() == 2) {
		// D add
		if (args[1] == "add") {
			bml->SendIngameMessage("add what");
		}
		// D list
		else if (args[1] == "menu") {
			*DebugMod::GetActiveInstance()->MainMenuEnable() = true;
		}
		else if (args[1] == "list") {
			DebugMod::GetActiveInstance()->AddDataWindow(new ListDataWindow(bml->GetCKContext()));
		}
		// D ???
		else {
			bml->SendIngameMessage("Command unknown.");
		}
	}
	// D xxx xxx xxx
	else if (args.size() == 4) {
		// D add xxx xxx
		if (args[1] == "add") {
			std::string _name_string = args[3];
			std::replace(_name_string.begin(), _name_string.end(), '+', ' ');
			CKSTRING _name = _name_string.c_str();
			// D add entity xxx
			if (args[2] == "entity") {
				CK3dEntity* _obj = (CK3dEntity*)bml->Get3dObjectByName(_name);
				if (_obj == nullptr) {
					_obj = bml->Get3dEntityByName(_name);
				}
				if (_obj == nullptr) {
					bml->SendIngameMessage("Object not found.");
					return;
				}
				DebugMod::GetActiveInstance()->AddDataWindow(new EntityDataWindow(_obj));
			}
			// D add camera xxx
			else if (args[2] == "camera") {
				CKCamera* _cam = bml->GetCameraByName(_name);
				bool _has_target = false;
				if (_cam == nullptr) {
					_cam = (CKCamera*)bml->GetTargetCameraByName(_name);
					_has_target = true;
				}
				if (_cam == nullptr) {
					bml->SendIngameMessage("Camera not found.");
					return;
				}
				DebugMod::GetActiveInstance()->AddDataWindow(new CameraDataWindow(_cam, _has_target));
			}
			// D add array xxx
			else if (args[2] == "array") {
				CKDataArray* _array = bml->GetArrayByName(_name);
				if (_array == nullptr) {
					bml->SendIngameMessage("Array not found.");
					return;
				}
				DebugMod::GetActiveInstance()->AddDataWindow(new ArrayDataWindow(_array));
			}
			// D add group xxx
			else if (args[2] == "group") {
				CKGroup* _group = bml->GetGroupByName(_name);
				if (_group == nullptr) {
					bml->SendIngameMessage("Group not found.");
					return;
				}
				DebugMod::GetActiveInstance()->AddDataWindow(new GroupDataWindow(_group));
			}
			// D add light xxx
			else if (args[2] == "light") {
				CKLight* _light = bml->GetLightByName(_name);
				bool _has_target = false;
				if (_light == nullptr) {
					_light = (CKLight*)bml->GetTargetLightByName(_name);
					_has_target = true;
				}
				if (_light == nullptr) {
					bml->SendIngameMessage("Light not found.");
					return;
				}
				DebugMod::GetActiveInstance()->AddDataWindow(new LightDataWindow(_light, _has_target));
			}
			// D add keys xxx
			else if (args[2] == "keys") {
				if (args[3] == "ballance") {
					DebugMod::GetActiveInstance()->AddDataWindow(new KeyDataWindow(DEFAULT_KEY_INSPECT));
				}
				else if (args[3] == "arrow") {
					DebugMod::GetActiveInstance()->AddDataWindow(new KeyDataWindow(ARROW_KEY_INSPECT));
				}
				else {
					bml->SendIngameMessage("No suitable option.");
				}
			}
			else {
				bml->SendIngameMessage("Type unknown.");
			}
		}
		else {
			bml->SendIngameMessage("Command unknown.");
		}
	}
	else {
		// ???
	}
}

const std::vector<std::string> DebugCmd::GetTabCompletion(IBML* bml, const std::vector<std::string>& args) {
	if (args.size() == 2) return std::vector<std::string>{"menu", "list", "add"};
	if (args.size() == 3 && args[1] == "add") return std::vector<std::string>{"entity", "array", "camera", "group", "keys"};
	if (args.size() == 4 && args[2] == "keys") return std::vector<std::string>{"ballance", "arrow"/*, "alphabet", "function", "all"*/};
	return std::vector<std::string>{};
}
