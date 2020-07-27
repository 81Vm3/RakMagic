#ifndef CONSOLE_H
#define CONSOLE_H

struct stCommand {
	const char *name;
	void (*callback)(const char *args);

	stCommand(const char *pname, void(*pcallback)(const char *args));
};

namespace Interface {
	namespace Console {
		void	Initialize();
		void	Uninitialize();
		void	AddCommand(const char *name, void(*callback)(const char *args));
		void	UnregisterCommmands();
		void    AddLog(const char* fmt, ...) IM_FMTARGS(2);
		void    ClearLog();
		void	AddHistory(const char *content);
		void    ExecCommand(const char* command_line);
		int     TextEditCallback(ImGuiInputTextCallbackData* data);
		void	Draw(float x, float y, float w, float h, float a);
	}
}
#endif