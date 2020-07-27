#include "main.h"

static FILE *flLog;

bool logOpen(const char *file) {
	if ((flLog = fopen(file, "a")) == nullptr)
		return false;
	return true;
}

void logPrintf(const char *str, ...) {
	time_t rawTime;
    struct tm * timeInfo;

    time ( &rawTime );
    timeInfo = localtime ( &rawTime );

    fprintf( flLog, "[%d-%d-%d %02d:%02d:%02d] ",1900 + timeInfo->tm_year, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  
    va_list ap;
	va_start( ap, str );
	char buffer[256];
	vsprintf(buffer, str, ap);

	Interface::Console::AddLog(buffer);

	va_end( ap );
	
	fwrite(buffer, strlen(buffer), 1, flLog);

	fputc('\n', flLog);
	putchar('\n');
	fflush( flLog );
}

void log(const char *str, ...) {
	time_t rawTime;
    struct tm * timeInfo;

    time ( &rawTime );
    timeInfo = localtime ( &rawTime );

    fprintf( flLog, "[%d-%d-%d %02d:%02d:%02d] ",1900 + timeInfo->tm_year, timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    
    va_list ap;
	va_start( ap, str );
	vfprintf( flLog, str, ap );
	va_end( ap );

	fputwc('\n', flLog);
	fflush( flLog );
}

void logClose() {
	if (flLog != nullptr) {
		fclose(flLog);
	}
}
