// clang-format off
#include <windows.h>
#include <winuser.h>
#include <string>
#include <vector>
#include "WindowManager.h"
#include "../utils/Logger.h"
// clang-format on

// Constructor con parámetros
WindowManager::WindowManager() : m_activeWindowIndex(-1), m_updater(nullptr) {

  LogDebug("Constructor WindowManager con parámetros");

  // Buscar y registrar ventanas
  // bool found = this->FindAndRegisterRoseWindows();

  // if (!found) {
  //   LogDebug("No se encontraron ventanas de ROSE");
  // }

  // La actualización de la interfaz se hará cuando se establezca el MainFrame
}

// Destructor
WindowManager::~WindowManager() {}

// Establecer el objeto que actualizará la interfaz
void WindowManager::SetUpdater(WindowListUpdater *updater) {
  m_updater = updater;

  // Si hay ventanas registradas, actualizar la interfaz
  if (!m_windows.empty() && m_updater != nullptr) {
    m_updater->UpdateWindowList();
  }
}

bool WindowManager::FindAndRegisterRoseWindows() {
  LogDebug("Buscando ventanas de ROSE...");
  std::vector<HWND> foundWindows;

  // Buscar ventanas de diferentes clases de ROSE
  // Similar al script original que busca ahk_class classPVP, classCLIENT, etc.
  std::vector<std::wstring> classNames = {L"classROSE", L"classCLIENT",
                                          L"classAMETHYST", L"classCodeZero",
                                          L"Arua"};

  // Para cada clase, buscar todas las ventanas
  for (const auto &className : classNames) {
    // Convertir wstring a string para logging
    std::string classNameLog(className.begin(), className.end());
    LogDebug("Buscando ventanas con clase: " + classNameLog);

    // Usar FindWindowExW para mantener consistencia con std::wstring
    HWND hwnd = FindWindowExW(NULL, NULL, className.c_str(), NULL);
    while (hwnd != NULL) {
      foundWindows.push_back(hwnd);
      hwnd = FindWindowExW(NULL, hwnd, className.c_str(), NULL);
    }
  }

  // Si no se encontraron ventanas, retornar falso
  if (foundWindows.empty()) {
    LogDebug("No se encontraron ventanas con las clases especificadas.");
    return false;
  }

  // Para cada ventana encontrada, añadirla a la lista si no está ya
  for (const auto &hwnd : foundWindows) {
    // Obtener título y clase de la ventana usando versiones Unicode (W)
    wchar_t title[256];
    wchar_t className[256];

    GetWindowTextW(hwnd, title, sizeof(title) / sizeof(wchar_t));
    GetClassNameW(hwnd, className, sizeof(className) / sizeof(wchar_t));

    std::wstring wTitle(title);
    std::wstring wClassName(className);

    // Si no está ya registrada, añadirla
    if (!IsWindowRegistered(hwnd)) {
      // Ya tenemos las cadenas en formato wstring, no necesitamos conversión
      AddWindow(hwnd, wClassName, wTitle);

      // Convertir a string solo para logging
      std::string titleStr(wTitle.begin(), wTitle.end());
      LogDebug("Registrada nueva ventana: " + titleStr);
    }
  }

  return true;
}

bool WindowManager::IsWindowRegistered(HWND hwnd) {
  return std::any_of(
      m_windows.begin(), m_windows.end(),
      [hwnd](const WindowInfo &info) { return info.winId == hwnd; });
}

void WindowManager::AddWindow(HWND hwnd, const std::wstring &className,
                              const std::wstring &title) {
  // Verificar si la ventana ya está registrada
  if (IsWindowRegistered(hwnd)) {
    // Convert std::wstring to std::string for logging
    std::string titleStr(title.begin(), title.end());
    LogDebug("Ventana ya registrada: " + titleStr);
    return;
  }

  // Crear nueva entrada para la ventana
  WindowInfo newWindow;
  newWindow.winId = hwnd;
  newWindow.winClass = className;
  newWindow.winTitle = title;
  newWindow.winOrder = static_cast<int>(m_windows.size()) + 1;

  // Añadir la ventana a la lista
  m_windows.push_back(newWindow);

  // Si es la primera ventana, establecerla como activa
  if (m_windows.size() == 1) {
    m_activeWindowIndex = 0;
  }

  // Convert std::wstring to std::string for logging
  std::string titleStr(title.begin(), title.end());
  LogDebug("Añadida ventana: " + titleStr);
}

void WindowManager::ResetWindows() {
  m_windows.clear();
  m_activeWindowIndex = -1;
  LogDebug("Lista de ventanas reiniciada");
}