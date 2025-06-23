#pragma once
// clang-format off
#include <wx/wx.h> 
#include "../core/WindowManager.h"
// clang-format on
class MainFrame : public wxFrame, public WindowListUpdater {
public:
  MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
  ~MainFrame();

  // Actualiza la interfaz con las ventanas actuales
  void UpdateWindowList();

  // Evento del botón de cargar ventanas
  void OnLoadWindowsButtonClicked(wxCommandEvent &event);

  // Evento para activar una ventana específica
  void OnActiveWindow(wxCommandEvent &event);

private:
  WindowManager *m_windowManager;
  wxPanel *m_panel_bottom; // Panel para la sección inferior (ventanas)
};