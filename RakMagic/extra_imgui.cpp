#include "main.h"

namespace ImGui {

	const char ColorMarkerStart = '{';
	const char ColorMarkerEnd = '}';

	const char szDefaultStyleFile[] = "style.bin";

	bool processInlineHexColor(const char* start, const char* end, ImVec4& color) {
		const int hexCount = (int)(end - start);
		if (hexCount == 6 || hexCount == 8)
		{
			char hex[9];
			strncpy(hex, start, hexCount);
			hex[hexCount] = 0;

			unsigned int hexColor = 0;
			if (sscanf(hex, "%x", &hexColor) > 0)
			{
				color.x = static_cast<float>((hexColor & 0x00FF0000) >> 16) / 255.0f;
				color.y = static_cast<float>((hexColor & 0x0000FF00) >> 8) / 255.0f;
				color.z = static_cast<float>((hexColor & 0x000000FF)) / 255.0f;
				color.w = 1.0f;

				if (hexCount == 8)
				{
					color.w = static_cast<float>((hexColor & 0xFF000000) >> 24) / 255.0f;
				}

				return true;
			}
		}

		return false;
	}

	void TextWithColors(const char* fmt, ...)
	{
		char tempStr[4096];

		va_list argPtr;
		va_start(argPtr, fmt);
		_vsnprintf(tempStr, sizeof(tempStr), fmt, argPtr);
		va_end(argPtr);
		tempStr[sizeof(tempStr) - 1] = '\0';

		bool pushedColorStyle = false;
		const char* textStart = tempStr;
		const char* textCur = tempStr;
		while (textCur < (tempStr + sizeof(tempStr)) && *textCur != '\0')
		{
			if (*textCur == ColorMarkerStart)
			{
				// Print accumulated text
				if (textCur != textStart)
				{
					ImGui::TextUnformatted(textStart, textCur);
					ImGui::SameLine(0.0f, 0.0f);
				}

				// Process color code
				const char* colorStart = textCur + 1;
				do
				{
					++textCur;
				} while (*textCur != '\0' && *textCur != ColorMarkerEnd);

				// Change color
				if (pushedColorStyle)
				{
					ImGui::PopStyleColor();
					pushedColorStyle = false;
				}

				ImVec4 textColor;
				if (processInlineHexColor(colorStart, textCur, textColor))
				{
					ImGui::PushStyleColor(ImGuiCol_Text, textColor);
					pushedColorStyle = true;
				}

				textStart = textCur + 1;
			}
			else if (*textCur == '\n')
			{
				// Print accumulated text an go to next line
				ImGui::TextUnformatted(textStart, textCur);
				textStart = textCur + 1;
			}

			++textCur;
		}

		if (textCur != textStart)
		{
			ImGui::TextUnformatted(textStart, textCur);
		}
		else
		{
			ImGui::NewLine();
		}

		if (pushedColorStyle)
		{
			ImGui::PopStyleColor();
		}
	}

	bool LoadImGuiStyleFile(const char *szFile) {
		ImGuiStyle *style = &ImGui::GetStyle();

		FILE *pFile;
		if ((pFile = fopen(szFile, "rb")) == NULL) {
			//Apply the default style
			ImGui::StyleColorsDark();
			return false;
		}

		fread(style, sizeof(ImGuiStyle), 1, pFile);
		fclose(pFile);

		return true;
	}

	bool SaveImGuiStyle(const char *szFile, ImGuiStyle *pstyle) {
		FILE *pFile;
		if ((pFile = fopen(szFile, "wb")) == NULL)
			return false;

		fwrite(pstyle, sizeof(ImGuiStyle), 1, pFile);
		fclose(pFile);

		return true;
	}

	void HelpMarker(const char* desc) {
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}