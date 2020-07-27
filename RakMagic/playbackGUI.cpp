#include "main.h"

namespace Interface {
	namespace PlaybackGUI {
		bool bEnabled = false;
		char szInput[128];
		char szFile[128];
		char szFileSaveAs[128];
		char szInfo[128] = "Playback";
		int iRecordType = Playback::PLAYBACK_TYPE_ONFOOT;

		//bool bUseHotkey = false;

		static void LoadPlayback() {
			char errmsg[256];
			if (Playback::Load(szInput, errmsg)) {
				sprintf(szInfo, "{00ff00}Playback \"%s\" load successful", szInput);
				strcpy(szFile, szInput);
				if (gConfig.bPlaybackAutoPlay)
					Playback::StartFirstEver();
			}
			else {
				sprintf(szInfo, "{ff0000}Error: %s", errmsg);
			}
		}

		static void SavePlayback() {
			if (!strlen(szFileSaveAs)) {
				strcpy(szInfo, "{ff0000}Please give a name to output file");
				return;
			}

			char errmsg[256];
			if (PBRecorder::Finalize(szFileSaveAs, errmsg)) {
				sprintf(szInfo, "{00ff00}Playback \"%s\" saved successfully", szFileSaveAs);
			}
			else {
				sprintf(szInfo, "{ff0000}Error: %s", errmsg);
			}
		}

		static void StartRecording() {
			if (iRecordType == Playback::PLAYBACK_TYPE_NONE) {
				strcpy(szInfo, "{ff0000}Player is not connected");
			}
			else if (!gPlayer[PBRecorder::iGoalPlayer].bIsConnected) {
				strcpy(szInfo, "{ff0000}Player is not connected");
			}
			else {
				if (!gPlayer[PBRecorder::iGoalPlayer].bIsStreamedIn) {
					strcpy(szInfo, "{ff0000}Player is not streamed in");
				}
				else {
					PBRecorder::StartRecording(iRecordType);
					strcpy(szInfo, "{00ff00}Recording has been started");
				}
			}
		}

		static void PauseOrResume() {
			Playback::bPlaying = !Playback::bPlaying;
			if (Playback::GetCurrentIndex() == 0)
				Playback::StartFirstEver();
		}

		static void RenderPlaybackPlayer() {
			ImGuiStyle *style = &ImGui::GetStyle();

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
			if (ImGui::InputText("File##Playback", szInput, 128, ImGuiInputTextFlags_EnterReturnsTrue)) {
				LoadPlayback();
			}

			ImGui::SameLine();
			if (ImGui::Button(ICON_MD_OPEN_IN_NEW" Load")) {
				LoadPlayback();
			} ImGui::SameLine();
			if (ImGui::Button(ICON_MD_CLOSE" Close")) {
				if (Playback::Unload())
					strcpy(szInfo, "Close the playback");
				else
					strcpy(szInfo, "{ff0000}You haven't load any playback");
			}
			
			ImGui::BulletText("Playback: %s", (Playback::iPlaybackType == Playback::PLAYBACK_TYPE_NONE) ? "None" : szFile); ImGui::SameLine();
			ImGui::BulletText("Type: %s",
				(Playback::iPlaybackType == Playback::PLAYBACK_TYPE_NONE) ?
				"Unknown" : ((Playback::iPlaybackType == Playback::PLAYBACK_TYPE_DRIVER) ? "Driver" : "On foot"));

			if (Playback::iPlaybackType == Playback::PLAYBACK_TYPE_DRIVER) {
				if (!IsVehicleIndexValid(RakMagic::iVehicle)) {
					ImGui::SameLine();
					ImGui::TextWithColors("{FFAA00}(A vehicle is required)");
				}
			}

			ImGui::NewLine();
			ImGui::AlignTextToFramePadding();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, style->ItemSpacing.y));
			ImGui::Text("Progess:"); ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
			if (ImGui::Button(Playback::bPlaying ? ICON_MD_PAUSE : ICON_MD_PLAY_ARROW))
				if (RakMagic::bSpawned)
					if (Playback::iPlaybackType != Playback::PLAYBACK_TYPE_NONE)
						PauseOrResume();

			if (Playback::iPlaybackType != Playback::PLAYBACK_TYPE_NONE)
				if (RakMagic::bSpawned)
					if (ImGui::IsWindowFocused())
						if (IsKeyPressOnce(GLFW_KEY_SPACE))
							PauseOrResume();

			ImGui::SameLine();
			char cache[64];
			sprintf(cache, "%d / %d", Playback::GetCurrentIndex(), Playback::GetTotalPacket());
			ImGui::PopStyleVar();
			ImGui::ProgressBar(Playback::GetProgessPercent(), ImVec2(0, ImGui::GetItemRectSize().y), cache);
			ImGui::PopStyleColor(3);
		}

		static void RenderRecorder() {

			ImGui::InputText("Output file", szFileSaveAs, 128, ImGuiInputTextFlags_EnterReturnsTrue);

			if (ImGui::Combo("Record type", &iRecordType, "None\0Vehicle\0On foot")) {
				if (PBRecorder::iPlaybackType != Playback::PLAYBACK_TYPE_NONE) {
					iRecordType = PBRecorder::iPlaybackType;
					strcpy(szInfo, "{ff0000}Failed to change type, the recording has been on");
				}
			}

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
			if (ImGui::InputScalar("Goal player:", ImGuiDataType_U32, &PBRecorder::iGoalPlayer, NULL, NULL, "%u", ImGuiInputTextFlags_EnterReturnsTrue)) {
				if (PBRecorder::iGoalPlayer >= MAX_PLAYERS) {
					PBRecorder::iGoalPlayer = MAX_PLAYERS - 1;
				}
			} 
			ImGui::SameLine();
			if (gPlayer[PBRecorder::iGoalPlayer].bIsConnected) {
				if (gPlayer[PBRecorder::iGoalPlayer].bIsStreamedIn)
					ImGui::TextColored(ImVec4(0, 1, 0, 1), gPlayer[PBRecorder::iGoalPlayer].szPlayerName);
				else
					ImGui::TextColored(ImVec4(1, 0, 0, 1), "Out of stream");
			}
			else {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "Not connected");
			}

			if (PBRecorder::iPlaybackType == Playback::PLAYBACK_TYPE_NONE) {
				if (ImGui::Button("Start recording")) {
					StartRecording();
				}
			}
			else {
				if (ImGui::Button("Save")) {
					SavePlayback();
				}
				ImGui::SameLine();
				if (ImGui::Button("Reset")) {
					strcpy(szInfo, "Reset the record");
					PBRecorder::Reset();
				}
			}
			ImGui::SameLine();
			ImGui::BulletText("Packet recorded: %d", PBRecorder::iPacketRecorded);
	
			/*if (ImGui::TreeNode("Hotkey")) {
				ImGui::Checkbox("Enable hotkey", &bUseHotkey);
				ImGui::Text("You need to open \"Playback\" window in order to use hotkey");
				ImGui::BulletText("Left Ctrl + Left Alt + K: Start or save record"); ImGui::SameLine();
				ImGui::HelpMarker("You need to set the output name before saving");
				ImGui::BulletText("Left Ctrl + Left Alt + L: Reset record");
				ImGui::Text("This function is still developing, you can't change the hotkey :(");
				ImGui::TreePop();
			}*/
		}

		/*static void HotkeyRecorder() {
			if (Interface::IsKeyPressOnce(GLFW_KEY_LEFT_CONTROL) && Interface::IsKeyPressOnce(GLFW_KEY_LEFT_ALT)) {
				if (Interface::IsKeyPressOnce(GLFW_KEY_K)) {
					if (PBRecorder::iPlaybackType == Playback::PLAYBACK_TYPE_NONE)
						StartRecording();
				}
				else if (Interface::IsKeyPressOnce(GLFW_KEY_L)) {
					if (PBRecorder::iPlaybackType != Playback::PLAYBACK_TYPE_NONE)
						SavePlayback();
				}
			}
		}*/

		void Render() {
			SetSubWindowTransparent();
			ImGui::Begin("Playback", &bEnabled);
			ImGui::TextWithColors(szInfo);
			ImGui::Separator();
			if (ImGui::BeginTabBar("PlaybackTab", ImGuiTabBarFlags_None)) {
				if (ImGui::BeginTabItem("Playback")) {
					RenderPlaybackPlayer();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Recorder")) {
					RenderRecorder();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
			//HotkeyRecorder();
		}
	}
}