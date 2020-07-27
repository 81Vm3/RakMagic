#include "main.h"

namespace Interface {
	namespace Scoreboard {
		bool bEnabled = false;
		void Render() {
			ImGuiStyle *style = &ImGui::GetStyle();
			SetSubWindowTransparent();
			ImGui::Begin("Scoreboard", &bEnabled);

			ImGui::Text("Players in total: %d", gPlayer->GetTotalPlayer());
			ImGui::Separator();

			ImGui::BeginChild("SBScroll", ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionMax().y - ImGui::GetCursorPosY() - style->WindowPadding.y), false, ImGuiWindowFlags_HorizontalScrollbar);

			ImGui::Columns(3);
			ImGui::Text("Player"); ImGui::NextColumn();
			ImGui::Text("Score"); ImGui::NextColumn();
			ImGui::Text("Ping"); ImGui::NextColumn();

			char buffer[80];
			ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.f, 0.f));
			for (int i = 0; i < MAX_PLAYERS; i++) {
				if (!gPlayer[i].bIsConnected)
					continue;
				sprintf(buffer, "%d %s", i, gPlayer[i].szPlayerName);
				if (ImGui::Button(buffer, ImVec2(-1.f, 0.f))) {
					//Like SAMP scoreboard clicking
					RakMagic::SendScoreboardClick(i);
				}
				ImGui::NextColumn();
				ImGui::Text("%d", gPlayer[i].iScore); ImGui::NextColumn();
				ImGui::Text("%d", gPlayer[i].dwPing); ImGui::NextColumn();
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::Columns(1);
			ImGui::EndChild();

			ImGui::Separator();
			ImGui::End();
		}
	}
}