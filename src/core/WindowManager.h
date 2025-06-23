#pragma once

#include <string>
#include <vector>
#include <windows.h>

// Interfaz para actualizar la interfaz gráfica
class WindowListUpdater {
public:
  virtual ~WindowListUpdater() {}
  virtual void UpdateWindowList() = 0;
};

// Estructura para almacenar información de ventanas
struct WindowInfo {
  HWND winId;            // Handle de la ventana
  std::wstring winClass; // Clase de la ventana
  std::wstring winTitle; // Título de la ventana
  int winOrder;          // Orden de la ventana

  WindowInfo() : winId(NULL), winOrder(0) {}
};

class WindowManager {
public:
  WindowManager();
  ~WindowManager();

  // Establecer el objeto que actualizará la interfaz
  void SetUpdater(WindowListUpdater *updater);

  // Busca y registra las ventanas del juego ROSE
  bool FindAndRegisterRoseWindows();

  // Obtiene la lista de ventanas registradas
  const std::vector<WindowInfo> &GetWindows() const { return m_windows; }

  // Resetea la lista de ventanas
  void ResetWindows();

  // Establecer orden de ventanas
  void SetWindowOrder(int windowIndex, int order);

  // Editar nombre de una ventana
  void EditWindowName(int windowIndex, const std::wstring &newName);

  // Ocultar/mostrar una ventana
  void ToggleWindowVisibility(int windowIndex, bool hide);

  // Eliminar una ventana
  void RemoveWindow(int windowIndex);

private:
  std::vector<WindowInfo> m_windows;
  int m_activeWindowIndex;
  WindowListUpdater *m_updater;

  // Verifica si una ventana ya está registrada
  bool IsWindowRegistered(HWND hwnd);

  // Añade una ventana a la lista
  void AddWindow(HWND hwnd, const std::wstring &className,
                 const std::wstring &title);
};
