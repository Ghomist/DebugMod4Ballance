#include "imgui_impl_virtools.h"
#include "DebugMod.h"

#define ENABLE_FOG_TOOLS true // If enable fog tools
#define ENABLE_RENDER_TOOLS true // If enable render tools

bool init = false;
EndScene oEndScene = NULL;

HWND window;
static int CKKey_ImGuiKey[256] = { 0 };

void InitImGui(LPDIRECT3DDEVICE9 pDevice) {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);

	// Default style
	// ImGui::StyleColorsClassic();

	// Key map
	InitializeKeyMap();
}

int kieroExampleThread() {
	bool hooked = false;
	do {
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success) {
			// define KIERO_USE_MINHOOK must be 1
			// the index of the required function can be found in the METHODSTABLE.txt
			if (kiero::bind(42, (void**)&oEndScene, hkEndScene) == kiero::Status::Success)
				hooked = true;

			//if (window != NULL) {
			//	oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			//	hooked = true;
			//}

			// If you just need to get the function address you can use the kiero::getMethodsTable function
			// oEndScene = (EndScene)kiero::getMethodsTable()[42];
		}
	} while (!hooked);

	return 0;
}

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
	if (!init) {
		InitImGui(pDevice);
		init = true;
	}

	// DebugMod::GetActiveInstance()->Logger()->Info("%s", init ? "yes" : "no");

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	if (DebugMod::GetActiveInstance()->IsActiveMainMenu())
		RenderMainMenuBar();
	// Update and render all
	for (DataWindow* data : DebugMod::GetActiveInstance()->GetAllDataWindows()) {
		data->Process();
		data->Render();
	}
	ImGui::EndFrame();

	// Rendering in hook
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice);
}

void Tip(const char* txt);
void ShowInfoWindow(bool* p_open);
void RenderMainMenuBar() {
	static bool show_info = true;

	static int shade_type = -1; // No external shade type
	static bool texture = true;
	static bool wireframe = false;

	static VXFOG_MODE fog_mode = DebugMod::GetActiveInstance()->GetCKRenderContext()->GetFogMode();
	static float fog_start = DebugMod::GetActiveInstance()->GetCKRenderContext()->GetFogStart();
	static float fog_end = DebugMod::GetActiveInstance()->GetCKRenderContext()->GetFogEnd();
	static float fog_density = DebugMod::GetActiveInstance()->GetCKRenderContext()->GetFogDensity();
	static float fog_color[4];

	// static float ambient_light[4];

	// static CKDWORD render_options = DebugMod::GetActiveInstance()->GetCurrentRenderOptions();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Options")) {
			ImGui::Text("Info      "); Tip("Debug Mod\nAuthor: Ghomist\n");
			ImGui::MenuItem("Tips Window", "", &show_info);

			ImGui::Separator();
			if (ImGui::BeginMenu("GUI Style")) {
				if (ImGui::MenuItem("Classic")) ImGui::StyleColorsClassic();
				if (ImGui::MenuItem("Light")) ImGui::StyleColorsLight();
				if (ImGui::MenuItem("Dark")) ImGui::StyleColorsDark();
				ImGui::EndMenu();
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Hide Tool Bar"))
				DebugMod::GetActiveInstance()->HideMainMenu();
			Tip("Use \"/debug menu\" to reopen");

			ImGui::Separator();
			if (ImGui::MenuItem("Exit Ballance"))
				std::exit(EXIT_SUCCESS);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) {

			if (ImGui::BeginMenu("Render Type", ENABLE_RENDER_TOOLS)) {
				if (ImGui::MenuItem("Default", "", shade_type == -1)) {
					shade_type = -1;
					DebugMod::GetActiveInstance()->SetGlobalShadeMode(GouraudShading, true, false);
				}
				Tip("Gouraud shading with\ndefault global shade settings");

				ImGui::Separator();
				if (ImGui::MenuItem("Gouraud Shading", "", shade_type == GouraudShading)) shade_type = GouraudShading;
				if (ImGui::MenuItem("WireFrame", "", shade_type == WireFrame)) shade_type = WireFrame;
				Tip("Rendering will use settings\nof materials for rendering");
				if (ImGui::MenuItem("Flat Shading", "", shade_type == FlatShading)) shade_type = FlatShading;
				if (ImGui::MenuItem("Phong Shading", "", shade_type == PhongShading)) shade_type = PhongShading;
				if (ImGui::MenuItem("Material Default", "", shade_type == MaterialDefault)) shade_type = MaterialDefault;
				Tip("All Objects will be rendered\nusing their own shade mode");
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Use texture", "", texture, shade_type != -1)) texture ^= true;
			if (ImGui::MenuItem("Wireframe Layer", "", wireframe, shade_type != -1)) wireframe ^= true;
			//if (ImGui::BeginMenu("Ambient Light", shade_type != -1)) {
			//	ImGui::ColorPicker3("", ambient_light, ImGuiColorEditFlags_RGB);
			//	ImGui::EndMenu();
			//}
			ImGui::Separator();

			if (ImGui::BeginMenu("Fog Mode", ENABLE_FOG_TOOLS)) {
				if (ImGui::MenuItem("None", "", fog_mode == VXFOG_NONE)) {
					fog_mode = VXFOG_NONE;
					DebugMod::GetActiveInstance()->SetFog(VXFOG_NONE, 0, 0, 0, 0, 0, 0, 0);
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Linear", "", fog_mode == VXFOG_LINEAR)) fog_mode = VXFOG_LINEAR;
				if (ImGui::MenuItem("Exp", "", fog_mode == VXFOG_EXP)) fog_mode = VXFOG_EXP; Tip("Exponential mode");
				if (ImGui::MenuItem("Exp2", "", fog_mode == VXFOG_EXP2)) fog_mode = VXFOG_EXP2; Tip("Exponential squared mode");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Fog options", fog_mode != VXFOG_NONE)) {
				ImGui::DragFloat("Start", &fog_start, 0.05f, 0.0f, FLT_MAX, "%.2f");
				fog_end = fog_start > fog_end ? fog_start : fog_end;
				ImGui::DragFloat("End", &fog_end, 0.05f, 0.0f, FLT_MAX, "%.2f");
				fog_start = fog_end < fog_start ? fog_end : fog_start;
				ImGui::DragFloat("Density", &fog_density, 0.05f, 0.0f, FLT_MAX, "%.2f");
				ImGui::ColorEdit4("Color", fog_color);
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("New Window")) {
			if (ImGui::MenuItem("List"))
				DebugMod::GetActiveInstance()->AddDataWindow(new ListDataWindow(DebugMod::GetActiveInstance()->GetCKContext()));
			Tip("A list containing most of\nthe objects in current game");

			ImGui::Separator();
			if (ImGui::BeginMenu("Keyboard")) {
				if (ImGui::MenuItem("Default"))
					DebugMod::GetActiveInstance()->AddDataWindow(new KeyDataWindow(DEFAULT_KEY_INSPECT));
				if (ImGui::MenuItem("Arrows-only"))
					DebugMod::GetActiveInstance()->AddDataWindow(new KeyDataWindow(ARROW_KEY_INSPECT));
				ImGui::EndMenu();
			}
			//ImGui::Separator();
			//if (ImGui::BeginMenu("3D Entity")) {
			//	char buf[50] = "";
			//	if (ImGui::InputText("Name", buf, 50, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::Button("Submit")) {
			//		DebugMod::GetActiveInstance()->AddDataWindow(new EntityDataWindow(
			//			DebugMod::GetActiveInstance()->GetEntity(buf)
			//		));
			//	}
			//	ImGui::EndMenu();
			//}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows List")) {
			std::vector<DataWindow*> data_list = DebugMod::GetActiveInstance()->GetAllDataWindows();
			if (data_list.size() == 0) {
				ImGui::Text("No Window");
			}
			else {
				for (int i = 0; i < data_list.size(); ++i) {
					//if (ImGui::Selectable(data->GetTitle(), data->IsActive())) 
					if (ImGui::MenuItem(data_list[i]->GetTitle(), "", data_list[i]->IsActive())) {
						data_list[i]->ChangeActiveState();
					}
					if (ImGui::BeginPopupContextItem()) {
						if (ImGui::Button("Kill Window"))
							DebugMod::GetActiveInstance()->PopDataWindow(i);
						ImGui::EndPopup();
					}
					else Tip(data_list[i]->GetObjectName());
				}
				ImGui::Separator();
				if (ImGui::Button("Kill All")) ImGui::OpenPopup("kill all window");
				if (ImGui::BeginPopup("kill all window")) {
					if (ImGui::Button("Confirm?"))
						DebugMod::GetActiveInstance()->PopAllDataWindows();
					ImGui::EndPopup();
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (shade_type != -1) {
		DebugMod::GetActiveInstance()->SetGlobalShadeMode((VxShadeType)shade_type, texture, wireframe);
		// DebugMod::GetActiveInstance()->SetAmbientLight(ambient_light[0], ambient_light[1], ambient_light[2]);
	}
	if (fog_mode != VXFOG_NONE) DebugMod::GetActiveInstance()->SetFog(fog_mode, fog_start, fog_end, fog_density, fog_color[3] * 255, fog_color[0] * 255, fog_color[1] * 255, fog_color[2] * 255);

	ShowInfoWindow(&show_info);
}

void ShowInfoWindow(bool* p_open) {
	if (!*p_open) return;

	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (corner != -1) {
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Example: Simple overlay", p_open, window_flags)) {
		ImGui::Text(
			"Hello, Ballancer!\n"
			"This is the debug mod for Ballance!\n"
			"------------------Tool-Bar-----------------\n"
			"1) New Window: Open debug/inspect windows.\n"
			"2) Windows List: Where you can check all\n"
			"   Your windows, includes which was closed.\n"
			"   Right click to kill one.\n"
			"3) Right click to close this window.\n"
			"------------------Commands-----------------\n"
			"/debug menu\n"
			"/debug list\n"
			"/debug add <type> <name>\n"
			"-------------------Hoocha!-----------------\n"
			"Made by Ghomist (or Zzq_203 anyway)\n"
			"(Check your mouse position below)\n"
			"(If your imgui works successfully)"
		);
		ImGui::Separator();
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		if (ImGui::BeginPopupContextWindow()) {
			if (p_open && ImGui::MenuItem("Close")) *p_open = false;
			ImGui::Separator();
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

void ProcessGuiInput() {
	if (!init) return;

	// Cursor
	ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);

	ImGuiIO& io = ImGui::GetIO();
	CKInputManager* vt_in = DebugMod::GetActiveInstance()->GetInputManager();
	// Mouse click
	for (int i = 0; i < 5; i++) io.MouseDown[i] = false;
	CKBYTE oStates[4];
	vt_in->GetMouseButtonsState(oStates);
	for (int i = CK_MOUSEBUTTON_LEFT; i <= CK_MOUSEBUTTON_4; ++i) {
		if (oStates[i] & KS_PRESSED)
			io.MouseDown[i] = true;
	}
	// Mouse wheel
	VxVector mouse_pos(0, 0, 0);
	vt_in->GetMouseRelativePosition(mouse_pos);
	io.MouseWheel = mouse_pos.z * DebugMod::GetActiveInstance()->GetLastDeltaTime() / 1000; // per 1000 ms

	// Keyboard
	bool shift = (vt_in->IsKeyDown(CKKEY_LSHIFT) | vt_in->IsKeyDown(CKKEY_RSHIFT)) ^ GetKeyState(VK_CAPITAL);
	// InitializeKeyMap();
	for (int i = 0; i < vt_in->GetNumberOfKeyInBuffer(); ++i) {
		CKDWORD key;
		int state = vt_in->GetKeyFromBuffer(i, key);
		// No related key
		if (CKKey_ImGuiKey[key] == 0)
			continue;
		// Characters
		if (CKKey_ImGuiKey[key] >= 32 && CKKey_ImGuiKey[key] <= 126 && state == KEY_PRESSED) {
			char buf[2];
			sprintf(buf, "%c", CKKey_ImGuiKey[shift ? key + 128 : key]);
			io.AddInputCharactersUTF8(buf);
			continue;
		}
		// Functional key
		io.KeysDown[CKKey_ImGuiKey[key]] = state == KEY_PRESSED;
	}
}

void InitializeKeyMap() {
	ImGuiIO& io = ImGui::GetIO();
	// Set key map
	io.KeyMap[ImGuiKey_Tab] = CKKEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = CKKEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = CKKEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = CKKEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = CKKEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = CKKEY_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = CKKEY_NEXT;
	io.KeyMap[ImGuiKey_Home] = CKKEY_HOME;
	io.KeyMap[ImGuiKey_End] = CKKEY_END;
	io.KeyMap[ImGuiKey_Insert] = CKKEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = CKKEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = CKKEY_BACK;
	io.KeyMap[ImGuiKey_Space] = CKKEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = CKKEY_RETURN;
	io.KeyMap[ImGuiKey_Escape] = CKKEY_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = CKKEY_NUMPADENTER;
	// io.KeyMap[ImGuiKey_A] = 'A';
	// io.KeyMap[ImGuiKey_C] = 'C';
	// io.KeyMap[ImGuiKey_V] = 'V';
	// io.KeyMap[ImGuiKey_X] = 'X';
	// io.KeyMap[ImGuiKey_Y] = 'Y';
	// io.KeyMap[ImGuiKey_Z] = 'Z';

	// Functional key
	CKKey_ImGuiKey[CKKEY_TAB] = CKKEY_TAB;
	CKKey_ImGuiKey[CKKEY_LEFT] = CKKEY_LEFT;
	CKKey_ImGuiKey[CKKEY_RIGHT] = CKKEY_RIGHT;
	CKKey_ImGuiKey[CKKEY_UP] = CKKEY_UP;
	CKKey_ImGuiKey[CKKEY_DOWN] = CKKEY_DOWN;
	CKKey_ImGuiKey[CKKEY_PRIOR] = CKKEY_PRIOR;
	CKKey_ImGuiKey[CKKEY_NEXT] = CKKEY_NEXT;
	CKKey_ImGuiKey[CKKEY_HOME] = CKKEY_HOME;
	CKKey_ImGuiKey[CKKEY_END] = CKKEY_END;
	CKKey_ImGuiKey[CKKEY_INSERT] = CKKEY_INSERT;
	CKKey_ImGuiKey[CKKEY_DELETE] = CKKEY_DELETE;
	CKKey_ImGuiKey[CKKEY_BACK] = CKKEY_BACK;
	CKKey_ImGuiKey[CKKEY_SPACE] = CKKEY_SPACE;
	CKKey_ImGuiKey[CKKEY_RETURN] = CKKEY_RETURN;
	CKKey_ImGuiKey[CKKEY_ESCAPE] = CKKEY_ESCAPE;
	CKKey_ImGuiKey[CKKEY_NUMPADENTER] = CKKEY_NUMPADENTER;
	// Numbers
	CKKey_ImGuiKey[CKKEY_1] = '1'; CKKey_ImGuiKey[CKKEY_1 + 128] = '!';
	CKKey_ImGuiKey[CKKEY_2] = '2'; CKKey_ImGuiKey[CKKEY_2 + 128] = '@';
	CKKey_ImGuiKey[CKKEY_3] = '3'; CKKey_ImGuiKey[CKKEY_3 + 128] = '#';
	CKKey_ImGuiKey[CKKEY_4] = '4'; CKKey_ImGuiKey[CKKEY_4 + 128] = '$';
	CKKey_ImGuiKey[CKKEY_5] = '5'; CKKey_ImGuiKey[CKKEY_5 + 128] = '%';
	CKKey_ImGuiKey[CKKEY_6] = '6'; CKKey_ImGuiKey[CKKEY_6 + 128] = '^';
	CKKey_ImGuiKey[CKKEY_7] = '7'; CKKey_ImGuiKey[CKKEY_7 + 128] = '&';
	CKKey_ImGuiKey[CKKEY_8] = '8'; CKKey_ImGuiKey[CKKEY_8 + 128] = '*';
	CKKey_ImGuiKey[CKKEY_9] = '9'; CKKey_ImGuiKey[CKKEY_9 + 128] = '(';
	CKKey_ImGuiKey[CKKEY_0] = '0'; CKKey_ImGuiKey[CKKEY_0 + 128] = ')';
	// Characters
	CKKey_ImGuiKey[CKKEY_Q] = 'q'; CKKey_ImGuiKey[CKKEY_Q + 128] = 'Q';
	CKKey_ImGuiKey[CKKEY_W] = 'w'; CKKey_ImGuiKey[CKKEY_W + 128] = 'W';
	CKKey_ImGuiKey[CKKEY_E] = 'e'; CKKey_ImGuiKey[CKKEY_E + 128] = 'E';
	CKKey_ImGuiKey[CKKEY_R] = 'r'; CKKey_ImGuiKey[CKKEY_R + 128] = 'R';
	CKKey_ImGuiKey[CKKEY_T] = 't'; CKKey_ImGuiKey[CKKEY_T + 128] = 'T';
	CKKey_ImGuiKey[CKKEY_Y] = 'y'; CKKey_ImGuiKey[CKKEY_Y + 128] = 'Y';
	CKKey_ImGuiKey[CKKEY_U] = 'u'; CKKey_ImGuiKey[CKKEY_U + 128] = 'U';
	CKKey_ImGuiKey[CKKEY_I] = 'i'; CKKey_ImGuiKey[CKKEY_I + 128] = 'I';
	CKKey_ImGuiKey[CKKEY_O] = 'o'; CKKey_ImGuiKey[CKKEY_O + 128] = 'O';
	CKKey_ImGuiKey[CKKEY_P] = 'p'; CKKey_ImGuiKey[CKKEY_P + 128] = 'P';
	CKKey_ImGuiKey[CKKEY_A] = 'a'; CKKey_ImGuiKey[CKKEY_A + 128] = 'A';
	CKKey_ImGuiKey[CKKEY_S] = 's'; CKKey_ImGuiKey[CKKEY_S + 128] = 'S';
	CKKey_ImGuiKey[CKKEY_D] = 'd'; CKKey_ImGuiKey[CKKEY_D + 128] = 'D';
	CKKey_ImGuiKey[CKKEY_F] = 'f'; CKKey_ImGuiKey[CKKEY_F + 128] = 'F';
	CKKey_ImGuiKey[CKKEY_G] = 'g'; CKKey_ImGuiKey[CKKEY_G + 128] = 'G';
	CKKey_ImGuiKey[CKKEY_H] = 'h'; CKKey_ImGuiKey[CKKEY_H + 128] = 'H';
	CKKey_ImGuiKey[CKKEY_J] = 'j'; CKKey_ImGuiKey[CKKEY_J + 128] = 'J';
	CKKey_ImGuiKey[CKKEY_K] = 'k'; CKKey_ImGuiKey[CKKEY_K + 128] = 'K';
	CKKey_ImGuiKey[CKKEY_L] = 'l'; CKKey_ImGuiKey[CKKEY_L + 128] = 'L';
	CKKey_ImGuiKey[CKKEY_Z] = 'z'; CKKey_ImGuiKey[CKKEY_Z + 128] = 'Z';
	CKKey_ImGuiKey[CKKEY_X] = 'x'; CKKey_ImGuiKey[CKKEY_X + 128] = 'X';
	CKKey_ImGuiKey[CKKEY_C] = 'c'; CKKey_ImGuiKey[CKKEY_C + 128] = 'C';
	CKKey_ImGuiKey[CKKEY_V] = 'v'; CKKey_ImGuiKey[CKKEY_V + 128] = 'V';
	CKKey_ImGuiKey[CKKEY_B] = 'b'; CKKey_ImGuiKey[CKKEY_B + 128] = 'B';
	CKKey_ImGuiKey[CKKEY_N] = 'n'; CKKey_ImGuiKey[CKKEY_N + 128] = 'N';
	CKKey_ImGuiKey[CKKEY_M] = 'm'; CKKey_ImGuiKey[CKKEY_M + 128] = 'M';
	// Special char
	CKKey_ImGuiKey[CKKEY_GRAVE] = '`'; CKKey_ImGuiKey[CKKEY_GRAVE + 128] = '~';
	CKKey_ImGuiKey[CKKEY_MINUS] = '-'; CKKey_ImGuiKey[CKKEY_MINUS + 128] = '_';
	CKKey_ImGuiKey[CKKEY_EQUALS] = '='; CKKey_ImGuiKey[CKKEY_EQUALS + 128] = '+';
	CKKey_ImGuiKey[CKKEY_LBRACKET] = '['; CKKey_ImGuiKey[CKKEY_LBRACKET + 128] = '{';
	CKKey_ImGuiKey[CKKEY_RBRACKET] = ']'; CKKey_ImGuiKey[CKKEY_RBRACKET + 128] = '}';
	CKKey_ImGuiKey[CKKEY_SEMICOLON] = ';'; CKKey_ImGuiKey[CKKEY_SEMICOLON + 128] = ':';
	CKKey_ImGuiKey[CKKEY_APOSTROPHE] = '\''; CKKey_ImGuiKey[CKKEY_APOSTROPHE + 128] = '"';
	CKKey_ImGuiKey[CKKEY_BACKSLASH] = '\\'; CKKey_ImGuiKey[CKKEY_BACKSLASH + 128] = '|';
	CKKey_ImGuiKey[CKKEY_SLASH] = '/'; CKKey_ImGuiKey[CKKEY_SLASH + 128] = '?';
	CKKey_ImGuiKey[CKKEY_COMMA] = ','; CKKey_ImGuiKey[CKKEY_COMMA + 128] = '<';
	CKKey_ImGuiKey[CKKEY_PERIOD] = '.'; CKKey_ImGuiKey[CKKEY_PERIOD + 128] = '>';
	CKKey_ImGuiKey[CKKEY_SPACE] = ' '; CKKey_ImGuiKey[CKKEY_SPACE + 128] = ' ';
}

void Tip(const char* txt) {
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(txt);
}