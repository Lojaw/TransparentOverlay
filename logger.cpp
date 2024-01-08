#include "logger.h"
#include <fstream>

std::ofstream logFile; // Globale Datei-Stream-Variable

void openLogFile(const std::string& filename) {
    logFile.open(filename, std::ios::out | std::ios::app);
}

void closeLogFile() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void logMessage(const std::string& message) {
    if (logFile.is_open()) {
        logFile << message << std::endl;
    }
}
