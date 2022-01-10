#include "MainMenu.h"

void RenderMainMenuBar() {
	static bool show_info = DEFAULT_SHOW_TIP;
	static bool* always_top_most = DebugMod::GetActiveInstance()->AlwaysTopMostEnable();

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
			if (ImGui::MenuItem("Top Most", "", always_top_most))
				~(*always_top_most);
			if (ImGui::MenuItem("Hide Tool Bar"))
				*DebugMod::GetActiveInstance()->MainMenuEnable() = false;
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
				Tip("Render wireframe only\nRendering will use settings\nof materials for rendering");
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
			auto data_list = DebugMod::GetActiveInstance()->GetAllDataWindows();
			if (data_list->size() == 0) {
				ImGui::Text("No Window");
			}
			else {
				for (int i = 0; i < data_list->size(); ++i) {
					//if (ImGui::Selectable(data->GetTitle(), data->IsActive())) 
					if (ImGui::MenuItem(data_list->at(i)->GetTitle(), "", data_list->at(i)->IsActive())) {
						data_list->at(i)->ChangeActiveState();
					}
					if (ImGui::BeginPopupContextItem()) {
						if (ImGui::Button("Kill Window"))
							DebugMod::GetActiveInstance()->PopDataWindow(i);
						ImGui::EndPopup();
					}
					else Tip(data_list->at(i)->GetObjectName());
				}
				ImGui::Separator();
				if (ImGui::Button("Kill All"))
					ImGui::OpenPopup("kill all window");
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

void Tip(const char* txt) {
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(txt);
}