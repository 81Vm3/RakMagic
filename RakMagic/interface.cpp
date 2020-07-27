#include "main.h"

namespace Interface {

	GLFWwindow *window;
	bool bTrue = true;

	int ScreenWidth = 800;
	int ScreenHeight = 480;

	bool bExit = false;

	int iNearbyObjectType;

	bool bAbout = false;

	void ApplyStyle() {

		int hue = 140;
		float col_main_sat = 180.f / 255.f;
		float col_main_val = 161.f / 255.f;
		float col_area_sat = 124.f / 255.f;
		float col_area_val = 100.f / 255.f;
		float col_back_sat = 59.f / 255.f;
		float col_back_val = 40.f / 255.f;

		ImGuiStyle& style = ImGui::GetStyle();

		ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
		ImVec4 col_main = ImColor::HSV(hue / 255.f, col_main_sat, col_main_val);
		ImVec4 col_back = ImColor::HSV(hue / 255.f, col_back_sat, col_back_val);
		ImVec4 col_area = ImColor::HSV(hue / 255.f, col_area_sat, col_area_val);

		style.Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(col_main.x, col_main.y, col_main.z, 0.80f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(col_main.x, col_main.y, col_main.z, 0.24f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	char keyState[GLFW_KEY_LAST + 1];
	char keyStateOnce[GLFW_KEY_LAST + 1];

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			keyState[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keyState[key] = false;
		}
	}

	bool Initialize() {
		if (!glfwInit())
			return false;

		window = glfwCreateWindow(ScreenWidth, ScreenHeight, "RakMagic " RAKMAGIC_VERSION, NULL, NULL);
		if (window == nullptr)
			return false;

		glfwSetWindowSizeCallback(window, OnWindowResized);

		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync
		glfwSetKeyCallback(window, key_callback);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		{
			char cache[256];
			sprintf(cache, ".\\fonts\\%s", gConfig.szFont);
			io.Fonts->AddFontFromFileTTF(cache, gConfig.fFontSize, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

			strcpy(cache, ".\\fonts\\" FONT_ICON_FILE_NAME_MD);
			static const ImWchar icons_ranges[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };
			ImFontConfig icons_config; 
			icons_config.MergeMode = true;
			icons_config.PixelSnapH = true;
			icons_config.GlyphOffset.y = (gConfig.fFontSize + 2.f) / 5.f;
			io.Fonts->AddFontFromFileTTF(cache, gConfig.fFontSize + 2.f, &icons_config, icons_ranges);

			io.Fonts->Build();
		}

		// Setup Dear ImGui style
		ImGui::LoadImGuiStyleFile(ImGui::szDefaultStyleFile);

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL2_Init();

		Console::Initialize();
		return true;
	}

	void Uninitialize() {
		if (window) {
			Console::Uninitialize();

			ImGui_ImplOpenGL2_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();

			glfwDestroyWindow(window);
			glfwTerminate();
		}
	}

	void OnWindowResized(GLFWwindow* window, int width, int height) {
		ScreenHeight = height;
		ScreenWidth = width;
	}

	bool ShouldClose() {
		return glfwWindowShouldClose(window) || bExit;
	}

	void SetSubWindowTransparent() {
		ImGui::SetNextWindowBgAlpha(min(Interface::fBoxAlpha * 1.5f, 255.0f));
	}

	//-----------------

	void Render() {
		glfwPollEvents();

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		RenderMenuBar();
		static ImGuiWindowFlags inflag = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus;
		//inflag |= ImGuiWindowFlags_MenuBar;
		ImGui::SetNextWindowBgAlpha(0.f);
		ImGui::SetNextWindowSize(ImVec2((float)ScreenWidth, (float)ScreenHeight));
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		ImGui::Begin("MAIN", &bTrue, inflag);
		{
			ImGuiStyle *style = &ImGui::GetStyle();
			float fx, fy, fw, fh;
			static const float fSeparate = 8.0f;
			{
				//left box, for displaying nearby things.
				fx = fSeparate; fy = fSeparate * 4 - 4.0f;
				ImGui::SetNextWindowPos(ImVec2(fx, fy));
				fw = 128.0f;
				fh = ScreenHeight - 200.0f;
				ImGui::SetNextWindowBgAlpha(fBoxAlpha);
				ImGui::BeginChild("leftbox1", ImVec2(fw, fh), &bTrue);
				
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::Combo("##OBJTYPE", &iNearbyObjectType, "Players\0Vehicles\0Pickups\0\0");

				ImGui::BeginChild("##OBJLIST", ImVec2(ImGui::GetWindowContentRegionWidth(), fh - ImGui::GetCursorPosY() - style->WindowPadding.y), false, ImGuiWindowFlags_HorizontalScrollbar);

				if (iNearbyObjectType == NEARBY_PLAYER) {
					for (int i = 0; i < MAX_PLAYERS; i++) {
						if (RakMagic::iPlayerID == i)
							continue;
						if (!gPlayer[i].bIsConnected)
							continue;
						if (!gPlayer[i].bIsStreamedIn)
							continue;
						
						if (i == RakMagic::iPlayerID)
							ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s(%d) %0.2fm", gPlayer[i].szPlayerName, i, vect3_dist(RakMagic::fPosition, gPlayer[i].GetPosition()));
						else 
							ImGui::Text("%s(%d) %0.2fm", gPlayer[i].szPlayerName, i, vect3_dist(RakMagic::fPosition, gPlayer[i].GetPosition()));
					}
				} else if (iNearbyObjectType == NEARBY_VEHICLE) {
					const struct vehicle_entry *pVehicle;
					for (int i = 0; i < MAX_VEHICLES; i++) {
						if (!gVehiclePool[i].bExist)
							continue;
						
						pVehicle = gta_vehicle_get_by_id(gVehiclePool[i].iModelID);
						if (!pVehicle)
							continue;
						
						if (i == RakMagic::iVehicle)
							ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s(%d) Driving", pVehicle->name, i, vect3_dist(RakMagic::fPosition, gVehiclePool[i].fPos));
						else 
							ImGui::Text("%s(%d) %0.2fm", pVehicle->name, i, vect3_dist(RakMagic::fPosition, gVehiclePool[i].fPos));
					}
				} else if (iNearbyObjectType == NEARBY_PICKUP) {
					for (int i = 0; i < MAX_PICKUPS; i++) {
						if (!gPickupPool[i].bExist)
							continue;
						ImGui::Text("%d(M:%d) %0.2fm", i, gPickupPool[i].iModelID, vect3_dist(RakMagic::fPosition, gPickupPool[i].fPos));
					}
				}
				
				ImGui::EndChild();
			}
			ImGui::EndChild();
			
			//small left box 
			{
				fy += fh + fSeparate;
				fh = ScreenHeight - fy - fSeparate;
				ImGui::SetNextWindowPos(ImVec2(fx, fy));
				ImGui::SetNextWindowBgAlpha(fBoxAlpha);
				ImGui::BeginChild("leftbox2", ImVec2(fw, fh), &bTrue);
				//render buttons
				ImVec2 centered(ImGui::GetWindowSize().x - (style->WindowPadding.x * 2), 0.0f);
				if (ImGui::Button("Request spawn", centered)) {
					if (RakMagic::bGameInited)
						RakMagic::RequestSpawn();
					else
						logPrintf(ERROR_GAMENOTINITED);
				}
				if (ImGui::Button("Spawn", centered)) {
					if (RakMagic::bGameInited)
						RakMagic::Spawn();
					else
						logPrintf(ERROR_GAMENOTINITED);
				}
				if (ImGui::Button("Fake spawn", centered)) {
					if (RakMagic::bGameInited && RakMagic::iState != RakMagic::RSTATE_DEAD)
						RakMagic::bSpawned = true;
					else
						logPrintf(ERROR_GAMENOTINITED);
				}
				if (ImGui::Button("Class", ImVec2(centered.x / 2.0f, 0.0f))) {
					if (RakMagic::bGameInited)
						RakMagic::RequestClass(gConfig.iDefaultClass);
					else
						logPrintf(ERROR_GAMENOTINITED);
				} ImGui::SameLine();
				ImGui::SetNextItemWidth(centered.x / 2.0f - style->WindowPadding.x);
				ImGui::InputScalar("##ClassInput", ImGuiDataType_S32, &gConfig.iDefaultClass, NULL, NULL, "%u");

				ImGui::SetNextItemWidth(centered.x);
				ImGui::Combo("##runmode", &RakMagic::iMode, "Normal\0Follow\0AFK\0");

				ImGui::EndChild();
			}

			fx = (fSeparate * 2.0f) + fw;
			fy = fSeparate * 4.0f - 4.0f;
			fw = ScreenWidth - fx - fSeparate;
			fh = ScreenHeight - fy - fSeparate;
			Console::Draw(fx, fy, fw, fh, fBoxAlpha);

			if (Dialog::bShowDialog && Dialog::bEnabled)
				Dialog::Render();

			if (Scoreboard::bEnabled)
				Scoreboard::Render();

			if (PlaybackGUI::bEnabled)
				PlaybackGUI::Render();

			if (bAbout) {
				SetSubWindowTransparent();
				ImGui::Begin("About", &bAbout);
				ImGui::TextWrapped(RAKMAGIC_ABOUT_MESSAGE);
				ImGui::End();
			}
		}
		ImGui::End();
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		glClearColor(gConfig.InterfaceClearColor.x, gConfig.InterfaceClearColor.y, gConfig.InterfaceClearColor.z, gConfig.InterfaceClearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	void RenderMenuBar() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem(ICON_MD_OPEN_IN_NEW " Open config")) {
					ShellExecute(NULL, NULL, gDefaultConfigPath, NULL, NULL, SW_SHOWNORMAL);
				}
				if (ImGui::MenuItem(ICON_MD_REFRESH " Reload config")) {
					if (!loadConfig(&gConfig, gDefaultConfigPath)) {
						logPrintf("[x] Failed to load XML configurations!!!");
					}
				}
				if (ImGui::MenuItem(ICON_MD_EXIT_TO_APP " Exit")) {
					bExit = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Client")) {
				ImGui::MenuItem(ICON_MD_LOCATION_ON " Keep position", NULL, &gConfig.bKeepPosition);
				ImGui::MenuItem(ICON_MD_TRANSFORM " Receive damage", NULL, &gConfig.bDamage);
				ImGui::MenuItem(ICON_MD_VISIBILITY_OFF " Spectator invisible", NULL, &gConfig.bInvisible);
				ImGui::MenuItem(ICON_MD_REPEAT " Playback", NULL, &PlaybackGUI::bEnabled);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Server")) {
				if (ImGui::MenuItem(ICON_MD_SIGNAL_WIFI_OFF " Reconnect"))
					RakMagic::Reconnect();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				ImGui::MenuItem(ICON_MD_LIST " Scoreboard", NULL, &Scoreboard::bEnabled);
				ImGui::MenuItem(ICON_MD_MENU " Dialog", NULL, &Dialog::bEnabled);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("About")) {
				ImGui::MenuItem(ICON_MD_NOTIFICATIONS " About", NULL, &bAbout);
				ImGui::EndMenu();
			}
			
			ImGui::EndMainMenuBar();
		}
	}

	bool IsKeyDown(int key) {
		return keyState[key];
	}

	bool IsKeyPressOnce(int key) {
		return (glfwGetKey(window, key) ? \
			(keyStateOnce[key] ? false : (keyStateOnce[key] = true)) : \
			(keyStateOnce[key] = false));
	}
}