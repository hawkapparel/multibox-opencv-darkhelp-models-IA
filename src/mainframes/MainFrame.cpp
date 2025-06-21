#include "MainFrame.h"
#include "../components/LargeCheckBox.h"
#include "../utils/Logger.h"
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/wx.h>

void MainFrame::OnLoadWindowsButtonClicked(wxCommandEvent &event) {
  LogDebug("Cargar Ventanas de ROSE");
}

MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                     const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {

  wxPanel *panel_top =
      new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(800, 100));
  // rgb(39, 55, 77)
  panel_top->SetBackgroundColour(wxColor(39, 55, 77));

  // Cargar la imagen para el icono del botón
  wxString iconPath = wxGetCwd() + "/icons/device-ipad-horizontal-bolt.png";
  wxImage image;

  if (image.LoadFile(iconPath, wxBITMAP_TYPE_PNG)) {
    // Escalar la imagen al tamaño adecuado para el botón (altura proporcional a
    // la del botón)
    int iconHeight = 24; // altura del icono
    int iconWidth = (image.GetWidth() * iconHeight) /
                    image.GetHeight(); // mantiene la proporción
    image.Rescale(iconWidth, iconHeight, wxIMAGE_QUALITY_HIGH);
  }

  wxBitmap iconBitmap(image);

  // Crear el botón temporalmente para calcular su tamaño basado en el texto
  wxButton *tempButton = new wxButton(panel_top, wxID_ANY);
  wxSize textSize = tempButton->GetTextExtent("Cargar Ventanas de ROSE");
  tempButton->Destroy(); // Eliminar el botón temporal

  // Calcular el ancho necesario para el texto + icono + márgenes
  int iconSpace = 0;
  if (iconBitmap.IsOk()) {
    iconSpace = iconBitmap.GetWidth() + 5; // 5px de margen entre icono y texto
  }

  // Crear el botón con tamaño ajustado para acomodar texto e icono
  wxButton *loadWindowsButton = new wxButton(
      panel_top, wxID_ANY, "Cargar Ventanas de ROSE", wxPoint(20, 20),
      wxSize(textSize.GetWidth() + iconSpace + 25,
             50)); // 20px adicionales de padding

  // Mapear el evento de click en el botón
  Connect(loadWindowsButton->GetId(), wxEVT_BUTTON,
          (wxObjectEventFunction)&MainFrame::OnLoadWindowsButtonClicked);

  // Establecer el bitmap a la izquierda del texto
  if (iconBitmap.IsOk()) {
    loadWindowsButton->SetBitmap(iconBitmap);
    loadWindowsButton->SetBitmapMargins(
        wxSize(5, 0)); // Margen entre el icono y el texto (5px)
    loadWindowsButton->SetBitmapPosition(
        wxLEFT); // Posición del icono a la izquierda del texto
  }

  // Calculamos la posición del segundo botón, para que esté a la derecha del
  // primero con una separación de 20 píxeles
  loadWindowsButton
      ->GetSize(); // Aseguramos que el botón ya tiene calculado su tamaño

  // rgb(26, 163, 99)
  // rgb(29, 201, 121)
  loadWindowsButton->SetBackgroundStyle(wxBG_STYLE_COLOUR);
  loadWindowsButton->SetBackgroundColour(wxColor(29, 201, 121));
  loadWindowsButton->SetCursor(wxCursor(wxCURSOR_HAND));

  wxSize loadButtonSize = loadWindowsButton->GetSize();
  int selectKeysX = 20 + loadButtonSize.GetWidth() +
                    20; // posX inicial + ancho del botón + separación

  // Cargar el icono para el segundo botón
  wxString keyboardIconPath = wxGetCwd() + "/icons/keyboard.png";
  wxImage keyboardImage;
  wxBitmap keyboardBitmap;

  if (keyboardImage.LoadFile(keyboardIconPath, wxBITMAP_TYPE_PNG)) {
    // Escalar la imagen con la misma altura que el icono anterior
    int iconHeight = 24;
    int iconWidth =
        (keyboardImage.GetWidth() * iconHeight) / keyboardImage.GetHeight();
    keyboardImage.Rescale(iconWidth, iconHeight, wxIMAGE_QUALITY_HIGH);
    keyboardBitmap = wxBitmap(keyboardImage);
  }

  // Crear el botón temporalmente para calcular su tamaño basado en el texto
  wxButton *tempButton2 = new wxButton(panel_top, wxID_ANY);
  wxSize textSize2 =
      tempButton2->GetTextExtent("Seleccionar Teclas para bucle infinito");
  tempButton2->Destroy();

  // Calcular espacio para el icono
  int iconSpace2 = 0;
  if (keyboardBitmap.IsOk()) {
    iconSpace2 =
        keyboardBitmap.GetWidth() + 5; // 5px de margen entre icono y texto
  }

  // Crear el botón con el tamaño adecuado
  wxButton *selectKeysButton = new wxButton(
      panel_top, wxID_ANY, "Seleccionar Teclas para bucle infinito",
      wxPoint(selectKeysX, 20),
      wxSize(textSize2.GetWidth() + iconSpace2 + 25, 50));

  // Establecer el bitmap, márgenes y posición
  if (keyboardBitmap.IsOk()) {
    selectKeysButton->SetBitmap(keyboardBitmap);
    selectKeysButton->SetBitmapMargins(
        wxSize(5, 0)); // Margen de 5px como el primer botón
    selectKeysButton->SetBitmapPosition(wxLEFT);
  }

  // Aplicar los mismos estilos de fondo y cursor que al primer botón
  selectKeysButton->SetBackgroundStyle(wxBG_STYLE_COLOUR);
  selectKeysButton->SetBackgroundColour(wxColor(18, 137, 248)); // Mismo color
  selectKeysButton->SetCursor(wxCursor(wxCURSOR_HAND));

  wxPanel *panel_bottom =
      new wxPanel(this, wxID_ANY, wxPoint(0, 100),
                  wxSize(800, 500)); // Tamaño inicial, se ajustará con EVT_SIZE

  // Ajustar el panel_bottom cuando cambie el tamaño de la ventana
  this->Bind(wxEVT_SIZE, [panel_bottom, panel_top](wxSizeEvent &event) {
    wxSize size = event.GetSize();
    panel_bottom->SetSize(0, 100, size.x, size.y - 100);
    panel_top->SetSize(0, 0, size.x, 100);
    event.Skip(); // Importante: permite que el evento siga propagándose
  });
  // rgb(221, 230, 237)
  panel_bottom->SetBackgroundColour(wxColor(221, 230, 237));

  wxSize panelBottomSize = panel_bottom->GetSize();
  wxStaticText *titlePanelBottom =
      new wxStaticText(panel_bottom, wxID_ANY, "Ventanas Activas",
                       wxDefaultPosition, wxDefaultSize);
  titlePanelBottom->SetPosition(
      wxPoint(panelBottomSize.x / 2 - titlePanelBottom->GetSize().x, 20));
  titlePanelBottom->SetFont(
      wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

  // Checkbox personalizado
  LargeCheckBox *largeCheckBox =
      new LargeCheckBox(panel_bottom, wxID_ANY, "Large Custom Checkbox",
                        wxPoint(50, 150), wxSize(-1, 40), wxBORDER_NONE);
  // Configurar un tamaño personalizado para el indicador
  largeCheckBox->SetIndicatorSize(20); // 30 pixeles de tamaño para el cuadrado
  largeCheckBox->SetBackgroundColour(wxColor(200, 200, 200));
  largeCheckBox->SetForegroundColour(wxColor(0, 0, 0));
  // end Checkbox

  /*
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

  // BEGIN PANEL LEFT

  // For panel_left, create a border sizer with specific dimensions
  wxBoxSizer *leftBorderSizer = new wxBoxSizer(wxVERTICAL);
  leftBorderSizer->Add(0, 10); // Top spacer: 10px

  wxBoxSizer *leftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
  leftMiddleSizer->Add(10, 0); // Left spacer: 5px
  leftMiddleSizer->Add(panel_top, 1, wxEXPAND);
  leftMiddleSizer->Add(5, 0); // Right spacer: 10px

  leftBorderSizer->Add(leftMiddleSizer, 1, wxEXPAND);
  leftBorderSizer->Add(0, 10); // Bottom spacer: 10px

  // Add the nested sizer to the main sizer
  sizer->Add(leftBorderSizer, 1, wxEXPAND);

  // END PANEL LEFT

  // BEGIN PANEL RIGHT
  wxBoxSizer *rightBorderSizer = new wxBoxSizer(wxVERTICAL);
  rightBorderSizer->Add(0, 10); // Top spacer: 10px

  wxBoxSizer *rightMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
  rightMiddleSizer->Add(5, 0); // Left spacer: 5px
  rightMiddleSizer->Add(panel_bottom, 1, wxEXPAND);
  rightMiddleSizer->Add(10, 0); // Right spacer: 10px

  rightBorderSizer->Add(rightMiddleSizer, 1, wxEXPAND);
  rightBorderSizer->Add(0, 10); // Bottom spacer: 10px

  sizer->Add(rightBorderSizer, 1, wxEXPAND);

  // END PANEL RIGHT

  this->SetSizerAndFit(sizer);
  */
}

MainFrame::~MainFrame() {
  // DESTRUCTOR
}