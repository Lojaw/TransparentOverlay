#include "TransparentOverlay.h"
#include <iostream>
#include <fstream>
#include <string>
#include "logger.h"
//Main.cpp

int main() {
    openLogFile("log.txt");
    // Nachrichten loggen
    logMessage("Programm wird gestartet");

    TransparentOverlay overlay;
    overlay.Run();

    closeLogFile();
    return 0;
}
