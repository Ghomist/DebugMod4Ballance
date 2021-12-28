#include "DebugMod.h"
#include "KeyDataWindow.h"

const char* GetKeyName(CKKEYBOARD key);

KeyDataWindow::KeyDataWindow(int _keys) {
	InitializeTitle("Keyboard");
	keys_type = _keys;
	switch (_keys) {
	case DEFAULT_KEY_INSPECT:
		inspect_keys = std::vector<CKKEYBOARD>{
			CKKEY_UP,
			CKKEY_DOWN,
			CKKEY_LEFT,
			CKKEY_RIGHT,
			CKKEY_SPACE,
			CKKEY_LSHIFT,
			CKKEY_Q
		};
		row_count = 7;
		break;
	case ARROW_KEY_INSPECT:
		inspect_keys = std::vector<CKKEYBOARD>{
			CKKEY_UP,
			CKKEY_DOWN,
			CKKEY_LEFT,
			CKKEY_RIGHT
		};
		row_count = 4;
		break;
		//case ALPHABET_KEY_INSPECT:
		//	break;
		//case FUNCTION_KEY_INSPECT:
		//	break;
		//case ALL_KEY_INSPECT:
		//	break;
	}
	_window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
}

void KeyDataWindow::Process() {
	if (!_active) return;

	key_state = DebugMod::GetActiveInstance()->GetKeyboardState();
}

void KeyDataWindow::Render() {
	if (!_active) return;

	ImGuiIO& io = ImGui::GetIO();

	ImGui::Begin(title.c_str(), &_active, _window_flags);
	MenuBar();
	if (ImGui::BeginTable("key-inspect", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders)) {
		ImGui::TableSetupColumn("Key");
		ImGui::TableSetupColumn("Hold?");
		ImGui::TableHeadersRow();
		for (int row = 0; row < row_count; row++) {
			bool _is_down = key_state[inspect_keys[row]];
			ImGui::TableNextRow();
			// Key
			ImGui::TableSetColumnIndex(0);
			ImGui::Button(GetKeyName(inspect_keys[row]), ImVec2(40, 0)); // ImVec2(-FLT_MIN, 0)
			// Hold?
			ImGui::TableSetColumnIndex(1);
			if (_is_down)
				ImGui::Text("Get!");
		}
		ImGui::EndTable();
	}
	ImGui::End();
}

const char* KeyDataWindow::GetObjectName() {
	switch (keys_type) {
	case DEFAULT_KEY_INSPECT: return "Default";
	case ARROW_KEY_INSPECT: return "Arrows";
		//case ALPHABET_KEY_INSPECT: return "";
		//case FUNCTION_KEY_INSPECT: return "";
		//case ALL_KEY_INSPECT: return "";
	}
	return "Unknown type";
}

const char* GetKeyName(CKKEYBOARD key) {
	switch (key) {
	case CKKEY_UP:
		return "up";
	case CKKEY_DOWN:
		return "down";
	case CKKEY_LEFT:
		return "left";
	case CKKEY_RIGHT:
		return "right";
	case CKKEY_SPACE:
		return "space";
	case CKKEY_LSHIFT:
	case CKKEY_RSHIFT:
		return "shift";
	case CKKEY_Q:
		return "Q";
	}
}