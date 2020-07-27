#ifndef LOG_H
#define LOG_H

bool logOpen(const char *file);
void logPrintf(const char *str, ...);
void log(const char *str, ...);
void logClose();

#define ERROR_GAMENOTINITED "[x] The game is not initialized"
#define ERROR_INCORRECTPID "[x] Incorrect player index"
#endif