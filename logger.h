// logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <string>

void logMessage(const std::string& message);
void openLogFile(const std::string& filename);
void closeLogFile();

#endif // LOGGER_H

