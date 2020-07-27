#include "main.h"

namespace Interface {

	namespace Dialog {

		bool bEnabled = true;
		bool bShowDialog;
		stSAMPDialog dialog;

		char szInput[256];
		int iSelectedItem;

		void ReplyDialog(bool positively) {
			BYTE respLen = (BYTE)strlen(szInput);
			RakNet::BitStream bsSend;
			bsSend.Write((WORD)dialog.wDialogID);
			bsSend.Write((BYTE)positively);
			if (iSelectedItem == -1)
				iSelectedItem = 0;
			bsSend.Write((WORD)iSelectedItem);
			bsSend.Write(respLen);
			bsSend.Write(szInput, respLen);
			RakMagic::pRakInterface->RPC(&RPC_DialogResponse, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);

			if (szInput[0] != 0x00)
				memset(szInput, 0, 256);

			bShowDialog = false;
		}

		void RenderButtons() {
			ImGuiStyle* style = &ImGui::GetStyle();
			ImGui::Separator();

			ImVec2 centered((ImGui::GetWindowSize().x - (style->WindowPadding.x * 2)) / 2.0f, 0.0f);
			if (ImGui::Button(dialog.szButton1, centered))
				ReplyDialog(true);

			ImGui::SameLine();

			if (ImGui::Button(dialog.szButton2, ImVec2(centered.x - style->WindowPadding.x, 0.0f)))
				ReplyDialog(false);
		}

		void Render() {
			SetSubWindowTransparent();

			char cache[260];
			strcpy(cache, dialog.szTitle);
			strcat(cache, "###Dialog");

			ImGui::Begin(cache, &bShowDialog);

			if (dialog.bDialogStyle == DIALOG_STYLE_MSGBOX ||
				dialog.bDialogStyle == DIALOG_STYLE_INPUT ||
				dialog.bDialogStyle == DIALOG_STYLE_PASSWORD) {

				ImGui::TextWithColors(dialog.szInfo);

				if (dialog.bDialogStyle == DIALOG_STYLE_INPUT ||
					dialog.bDialogStyle == DIALOG_STYLE_PASSWORD) {
					ImGui::PushItemWidth(-1.0f);
					ImGui::InputText("##DContent", szInput, IM_ARRAYSIZE(szInput), (dialog.bDialogStyle == DIALOG_STYLE_PASSWORD) ? ImGuiInputTextFlags_Password : NULL);
				}
			}
			else if (dialog.bDialogStyle == DIALOG_STYLE_LIST ||
				dialog.bDialogStyle == DIALOG_STYLE_TABLIST ||
				dialog.bDialogStyle == DIALOG_STYLE_TABLIST_HEADERS) {

				//hmmm... it's sort of complicated.

				int tab_cut = -1; //location where the first '\n' appears, for DIALOG_STYLE_TABLIST_HEADERS

				if (dialog.bDialogStyle == DIALOG_STYLE_TABLIST_HEADERS) {
					char clone[257];
					strcpy(clone, dialog.szInfo);
					char* token = strchr(clone, '\n');
					if (token) {
						*token = 0x00;
						tab_cut = strlen(clone);
						ImGui::Text(clone);
						ImGui::Separator();
					}
				}

				ImGui::BeginChild("##DLIST", ImVec2(ImGui::GetWindowContentRegionWidth(), 128.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

				int index = 0;
				char clone[257];
				if (tab_cut == -1)
					strcpy(clone, dialog.szInfo);
				else
					strcpy(clone, &dialog.szInfo[tab_cut]);

				char* token = strtok(clone, "\n");

				while (token != NULL) {
					if (ImGui::Selectable(token, iSelectedItem == index))
						iSelectedItem = index;
					token = strtok(NULL, "\n");
					index++;
				}

				ImGui::EndChild();
			}

			RenderButtons();
			ImGui::End();
		}
	}
}