#ifndef EXTRA_IMGUI_H
#define EXTRA_IMGUI_H

namespace ImGui {
	extern const char szDefaultStyleFile[];
	void TextWithColors(const char* fmt, ...);
	
	bool LoadImGuiStyleFile(const char *szFile);
	bool SaveImGuiStyle(const char *szFile, ImGuiStyle *pstyle);

	void HelpMarker(const char* desc);
}

#endif