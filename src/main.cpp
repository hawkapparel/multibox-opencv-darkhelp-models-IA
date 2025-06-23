// clang-format off
#include <wx/wx.h>
#include <wx/image.h>
#include "utils/Logger.h"
#include "mainframes/MainFrame.h"
// clang-format on

class RoseMultiboxApp : public wxApp {
public:
  virtual bool OnInit();
};

wxIMPLEMENT_APP(RoseMultiboxApp);

bool RoseMultiboxApp::OnInit() {
  // Inicializar los manejadores de formatos de imagen
  wxInitAllImageHandlers();

  LogDebug("=== ROSE Multibox Debug Console (UTF-8) ===");
  MainFrame *frame = new MainFrame("Multibox By HawkDev 3.0.0",
                                   wxDefaultPosition, wxDefaultSize);
  frame->SetSize(wxSize(800, 600));
  frame->SetMinSize(wxSize(800, 600));
  frame->SetMaxSize(wxSize(800, 600));
  frame->Center();
  frame->Show(true);
  return true;
}