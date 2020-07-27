#include "main.h"

stCommand::stCommand(const char *pname, void(*pcallback)(const char *args)) {
	name = pname;
	callback = pcallback;
}

namespace Interface {
	namespace Console {

		char                  InputBuf[256];
		ImVector<char*>       Items;
		ImVector<stCommand*>  Commands;
		ImVector<char*>       History;
		int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
		bool                  AutoScroll;
		bool                  ScrollToBottom;

		void Initialize() {
			ClearLog();
			memset(InputBuf, 0, sizeof(InputBuf));
			HistoryPos = -1;
			registerAllCommands();

			AutoScroll = true;
			ScrollToBottom = false;
			AddLog("RakMagic " RAKMAGIC_VERSION"     Author: " RAKMAGIC_AUTHOR"");
		}

		void Uninitialize() {
			ClearLog();
			for (int i = 0; i < History.Size; i++)
				free(History[i]);
			History.clear();

			UnregisterCommmands();
		}

		// Portable helpers
		static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
		static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
		static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
		static void  Strtrim(char* str) { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

		void ClearLog() {
			for (int i = 0; i < Items.Size; i++)
				free(Items[i]);
			Items.clear();
		}

		void AddCommand(const char *name, void(*callback)(const char *args)) {
			Commands.push_back(new stCommand(name, callback));
		}

		void UnregisterCommmands() {
			for (int i = 0; i < Commands.Size; i++)
				free(Commands[i]);
			Commands.clear();
		}

		void AddLog(const char* fmt, ...) IM_FMTARGS(2) {
			// FIXME-OPT
			char buf[1024];
			va_list args;
			va_start(args, fmt);
			vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
			buf[IM_ARRAYSIZE(buf) - 1] = 0;
			va_end(args);
			Items.push_back(Strdup(buf));
			if (Items.Size > 256) {
				free(Items[0]);
				Items.erase(Items.begin());
			}
		}

		void Draw(float x, float y, float w, float h, float a) {

			ImGui::SetNextWindowBgAlpha(a);
			ImGui::SetNextWindowPos(ImVec2(x, y));
			ImGui::SetNextWindowSize(ImVec2(w, h));

			static bool open = true;
			ImGui::BeginChild("CONSOLE", ImVec2(w, h), &open);

			if (RakMagic::bGameInited) {

				char buffer[64];

				ImGui::Columns(5, "InfoBar", false);
				ImGui::Text("Client:%s", gConfig.szNickname); ImGui::NextColumn();
				ImGui::Text("Index:%d", RakMagic::iPlayerID); ImGui::NextColumn();
				ImGui::Text("Ping:%d", gPlayer[RakMagic::iPlayerID].dwPing); ImGui::NextColumn();
				ImGui::Text("Score:%d", gPlayer[RakMagic::iPlayerID].iScore); ImGui::NextColumn();

				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(118, 118, 118, 255));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6);

				if (RakMagic::fArmor > 100.f || RakMagic::fArmor < 0.f) //isinfinite function seems not work at all 
					strcpy(buffer, "N/A");
				else
					sprintf(buffer, "%0.2f", RakMagic::fArmor);
				ImGui::ProgressBar(RakMagic::fArmor / 100.f, ImVec2(0.f, gConfig.fFontSize), buffer); ImGui::NextColumn(); //5
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();

				ImGui::Text("X:%0.3f", RakMagic::fPosition[0]); ImGui::NextColumn();
				ImGui::Text("Y:%0.3f", RakMagic::fPosition[1]); ImGui::NextColumn();
				ImGui::Text("Z:%0.3f", RakMagic::fPosition[2]); ImGui::NextColumn();
				
				if (IsVehicleIndexValid(RakMagic::iVehicle)) {
					const struct vehicle_entry *pVehicle = gta_vehicle_get_by_id(gVehiclePool[RakMagic::iVehicle].iModelID);
					if (pVehicle)
						sprintf(buffer, "Veh:%d (%s)", RakMagic::iVehicle, pVehicle->name);
					else
						strcpy(buffer, "Veh:Err");
				}
				else
					strcpy(buffer, "Veh:None");
				ImGui::Text(buffer); ImGui::NextColumn();

				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(200, 50, 50, 255));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6);
				if (RakMagic::fHealth > 100.f || RakMagic::fHealth < 0.f)
					strcpy(buffer, "N/A");
				else
					sprintf(buffer, "%0.2f", RakMagic::fHealth);
				ImGui::ProgressBar(RakMagic::fHealth / 100.f, ImVec2(0.f, gConfig.fFontSize), buffer); ImGui::NextColumn(); //5
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();

				ImGui::Columns(1);
			}
			/*if (ImGui::SmallButton("Add Dummy Text")) { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ImGui::SameLine();
			if (ImGui::SmallButton("Add Dummy Error")) { AddLog("[error] something went wrong"); } ImGui::SameLine();
			if (ImGui::SmallButton("Clear")) { ClearLog(); } ImGui::SameLine();
			bool copy_to_clipboard = ImGui::SmallButton("Copy");*/
			//static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

			ImGui::Separator();


			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Clear")) ClearLog();
				ImGui::EndPopup();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			//if (copy_to_clipboard)
			//	ImGui::LogToClipboard();
			for (int i = 0; i < Items.Size; i++) {
				const char* item = Items[i];

				// Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
				bool pop_color = false;
				if (strstr(item, "[x]")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true; }
				else if (strncmp(item, "[-]", 2) == 0) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.25f, 1.0f, 0.25f, 1.0f)); pop_color = true; }
				else if (strncmp(item, "# ", 2) == 0) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f)); pop_color = true; }

				if (pop_color) {
					ImGui::TextUnformatted(item);
					ImGui::PopStyleColor();
				}
				else {
					ImGui::TextWithColors(item);
				}
			}
			//if (copy_to_clipboard)
			//	ImGui::LogFinish();

			if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
				ImGui::SetScrollHereY(1.0f);
			ScrollToBottom = false;

			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();

			// Command-line
			bool reclaim_focus = false;
			if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &TextEditCallback)) {
				char* s = InputBuf;
				Strtrim(s);
				if (s[0] == '!') {
					ExecCommand(s);
				}
				else if (s[0] == '/') {
					if (RakMagic::bGameInited) {
						if (strlen(s)) {
							if (gConfig.bEncodeGB2312) {
								char *unicoded = u2g(s);
								RakMagic::SendServerCommand(unicoded);
								AddHistory(unicoded);
								delete[] unicoded;
							}
							else {
								RakMagic::SendServerCommand(s);
								AddHistory(s);
							}
						}
					}
				}
				else {
					if (RakMagic::bGameInited) {
						if (strlen(s)) {
							if (gConfig.bEncodeGB2312) {
								char *unicoded = u2g(s);
								RakMagic::SendChat(unicoded);
								AddHistory(unicoded);
								delete[] unicoded;
							}
							else {
								RakMagic::SendChat(s);
								AddHistory(s);
							}
						}
					}
				}
				memset(s, 0, sizeof(*s));
				reclaim_focus = true;
			}

			// Auto-focus on window apparition
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

			ImGui::EndChild();
		}

		void AddHistory(const char *content) {
			// Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
			HistoryPos = -1;
			for (int i = History.Size - 1; i >= 0; i--)
				if (Stricmp(History[i], content) == 0)
				{
					free(History[i]);
					History.erase(History.begin() + i);
					break;
				}
			History.push_back(Strdup(content));
		}

		void ExecCommand(const char* command_line) {
			AddLog("# %s", command_line);

			AddHistory(command_line);

			//copy
			char buffer[256];
			memset(buffer, 0, 256); //Always remember to empty this char array, or there will be a argument problem.
			strcpy(buffer, &command_line[1]);

			//separate command and arguments
			int length = strlen(buffer);
			int cmdlen = 0;
			for (int i = 0; i < length; i++) {
				if (buffer[cmdlen] == ' ' || buffer[cmdlen] == '\0') break;
				cmdlen++;
			}
			const char *arguments = &buffer[cmdlen + 1];
			buffer[cmdlen] = 0x00;

			// Process command
			bool found = false;
			for (int i = 0; i < Commands.Size; i++) {
				if (Stricmp(buffer, Commands[i]->name) == 0) {
					Commands[i]->callback(arguments);
					found = true;
					break;
				}
			}
			if (!found)
				AddLog("Unknown command: '%s'\n", command_line);

			// On commad input, we scroll to bottom even if AutoScroll==false
			ScrollToBottom = true;
		}

		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
		{
			//stConsole* console = (stConsole*)data->UserData;
			//return console->TextEditCallback(data);
		}

		int     TextEditCallback(ImGuiInputTextCallbackData* data)
		{
			//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
			switch (data->EventFlag)
			{
			case ImGuiInputTextFlags_CallbackCompletion:
			{

				if (data->Buf[0] != '!')
					break;

				// Locate beginning of current word
				const char* word_end = (data->Buf + data->CursorPos);
				const char* word_start = word_end;
				while (word_start > data->Buf + 1)
				{
					const char c = word_start[-1];
					if (c == ' ' || c == '\t' || c == ',' || c == ';')
						break;
					word_start--;
				}

				// Build a list of candidates
				ImVector<const char*> candidates;
				for (int i = 0; i < Commands.Size; i++)
					if (Strnicmp(Commands[i]->name, word_start, (int)(word_end - word_start)) == 0)
						candidates.push_back(Commands[i]->name);

				if (candidates.Size == 0)
				{
					// No match
					AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
				}
				else if (candidates.Size == 1)
				{
					// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
					data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
					data->InsertChars(data->CursorPos, candidates[0]);
					data->InsertChars(data->CursorPos, " ");
				}
				else
				{
					// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
					int match_len = (int)(word_end - word_start);
					for (;;)
					{
						int c = 0;
						bool all_candidates_matches = true;
						for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
							if (i == 0)
								c = toupper(candidates[i][match_len]);
							else if (c == 0 || c != toupper(candidates[i][match_len]))
								all_candidates_matches = false;
						if (!all_candidates_matches)
							break;
						match_len++;
					}

					if (match_len > 0)
					{
						data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
						data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
					}

					// List matches
					AddLog("Possible matches:");
					for (int i = 0; i < candidates.Size; i++)
						AddLog("- %s", candidates[i]);
				}

				break;
			}
			case ImGuiInputTextFlags_CallbackHistory:
			{
				// Example of HISTORY
				const int prev_history_pos = HistoryPos;
				if (data->EventKey == ImGuiKey_UpArrow)
				{
					if (HistoryPos == -1)
						HistoryPos = History.Size - 1;
					else if (HistoryPos > 0)
						HistoryPos--;
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					if (HistoryPos != -1)
						if (++HistoryPos >= History.Size)
							HistoryPos = -1;
				}

				// A better implementation would preserve the data on the current input line along with cursor position.
				if (prev_history_pos != HistoryPos)
				{
					const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, history_str);
				}
			}
			}
			return 0;
		}
	}
}