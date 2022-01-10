#include "imgui_impl_virtools.h"

bool init = false;
EndScene oEndScene = NULL;
HWND window;
static int CKKey_ImGuiKey[256] = { 0 };

void InitImGui(LPDIRECT3DDEVICE9 pDevice) {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
	ImGui::StyleColorsClassic(); // Default style
	InitializeKeyMap(); // Key map
}

// Hook thread
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

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	// Remember to process input every frame
	ProcessGuiInput();
	// Render all data windows
	DebugMod::GetActiveInstance()->RenderAll();

	// Rendering in hook
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(pDevice);
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

bool IsWindowTopMost() {
	return (GetWindowLong(window, GWL_EXSTYLE) & WS_EX_TOPMOST);
}

void SetWindowTopMost(bool top_most) {
	SetWindowPos(window, top_most ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
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