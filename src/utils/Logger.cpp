#include "Logger.h"
#include <Windows.h> // Para OutputDebugStringA
#include <ctime>
#include <fstream>
#include <iostream> // Para cout

// Variable global para controlar si se ha inicializado la consola
bool g_consoleInitialized = false;

// FunciÃ³n para inicializar la consola para debugging con soporte UTF-8
void InitConsole() {
  if (!g_consoleInitialized) {
    // Crear una consola
    if (AllocConsole()) {
      // Redireccionar stdout/stdin/stderr a la consola
      FILE *pFile = nullptr;
      freopen_s(&pFile, "CONOUT$", "w", stdout);
      freopen_s(&pFile, "CONIN$", "r", stdin);
      freopen_s(&pFile, "CONOUT$", "w", stderr);

      // Habilitar el procesamiento de comandos de consola virtual (VT)
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      DWORD dwMode = 0;
      GetConsoleMode(hOut, &dwMode);
      dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      SetConsoleMode(hOut, dwMode);

      // Configurar la codificaciÃ³n de la consola a UTF-8
      SetConsoleOutputCP(CP_UTF8);
      SetConsoleCP(CP_UTF8);

      // Configurar la fuente de la consola para mejor soporte de caracteres
      // especiales
      CONSOLE_FONT_INFOEX cfi = {sizeof(cfi)};
      GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
      wcscpy(cfi.FaceName, L"Consolas"); // Fuente con mejor soporte Unicode
      SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

      // Configurar el tÃ­tulo de la consola usando la versiÃ³n Unicode
      SetConsoleTitleW(L"ROSE Multibox - Debug Console \u2713");

      // Marcar la consola como inicializada
      g_consoleInitialized = true;

      // Mensaje inicial con caracteres de prueba en espaÃ±ol
      std::cout << "=== ROSE Multibox Debug Console (UTF-8) ===" << std::endl;
      std::cout << "Caracteres de prueba: "
                   "Ã¡Ã©Ã­Ã³ÃºÃ±ÃÃ‰ÃÃ“ÃšÃ‘"
                << std::endl;
      std::cout << "Los mensajes de depuraciÃ³n aparecerÃ¡n aquÃ­."
                << std::endl;
      std::cout << "================================" << std::endl << std::endl;
    }
  }
}

// ImplementaciÃ³n de la funciÃ³n de logging
void LogDebug(const wxString &mensaje) {
  // Asegurar que la consola estÃ© inicializada
  if (!g_consoleInitialized) {
    InitConsole();
  }

  // Obtener tiempo actual
  time_t now = time(0);
  struct tm timeinfo;
  char buffer[80];
  localtime_s(&timeinfo, &now);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

  // Formato del mensaje (asegurando que estÃ© en UTF-8)
  std::string logMessage = std::string(buffer) + " - " + mensaje.ToStdString();

  // Escribir al archivo de log en UTF-8
  std::ofstream logFile("roseMultibox_debug.log",
                        std::ios_base::app | std::ios_base::binary);
  logFile << logMessage << std::endl;
  logFile.close();

  // Escribir a la consola (ya configurada para UTF-8)
  std::cout << logMessage << std::endl;

  // Escribir a la ventana de salida del depurador de Visual Studio
  logMessage += "\n";
  OutputDebugStringA(logMessage.c_str());
}

// Nueva funciÃ³n para loggear texto wxString directamente (para manejar mejor
// caracteres espaÃ±oles)
void LogDebugW(const wxString &mensaje) {
  // Ya que LogDebug ahora acepta wxString directamente, simplemente lo llamamos
  LogDebug(mensaje);
}
