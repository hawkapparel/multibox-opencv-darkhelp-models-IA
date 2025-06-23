// clang-format off
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/wx.h>
#include <sstream>
#include "MainFrame.h"
#include "../utils/Logger.h"
#include "../components/LargeCheckBox.h"
// clang-format on

MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                     const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
  // Inicializar el WindowManager
  m_windowManager = new WindowManager();

  // Establecer la referencia a este MainFrame en el WindowManager
  m_windowManager->SetUpdater(this);

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

  // Crear panel inferior y guardarlo como miembro de clase
  m_panel_bottom =
      new wxPanel(this, wxID_ANY, wxPoint(0, 100),
                  wxSize(800, 500)); // Tamaño inicial, se ajustará con EVT_SIZE

  // Ajustar el panel_bottom cuando cambie el tamaño de la ventana
  this->Bind(wxEVT_SIZE, [this, panel_top](wxSizeEvent &event) {
    wxSize size = event.GetSize();
    m_panel_bottom->SetSize(0, 100, size.x, size.y - 100);
    panel_top->SetSize(0, 0, size.x, 100);
    event.Skip(); // Importante: permite que el evento siga propagándose
  });
  // rgb(221, 230, 237)
  m_panel_bottom->SetBackgroundColour(wxColor(221, 230, 237));

  // Renderizado dinámico de la lista de ventanas
  wxSize panelBottomSize = m_panel_bottom->GetSize();

  wxStaticText *titlePanelBottom =
      new wxStaticText(m_panel_bottom, wxID_ANY, "Ventanas Activas",
                       wxDefaultPosition, wxDefaultSize);
  titlePanelBottom->SetPosition(
      wxPoint(panelBottomSize.x / 2 - titlePanelBottom->GetSize().x, 20));
  titlePanelBottom->SetFont(
      wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
}

MainFrame::~MainFrame() {
  // Liberar la memoria del WindowManager
  if (m_windowManager != nullptr) {
    delete m_windowManager;
    m_windowManager = nullptr;
  }
}

void MainFrame::UpdateWindowList() {
  // Actualizar la lista de ventanas en la interfaz
  // Primero, limpiar todos los controles antiguos del panel
  m_panel_bottom->DestroyChildren();

  // Crear un título para la sección
  wxStaticText *titleSection =
      new wxStaticText(m_panel_bottom, wxID_ANY, "Ventanas Detectadas",
                       wxDefaultPosition, wxDefaultSize);
  titleSection->SetPosition(wxPoint(50, 20));
  titleSection->SetFont(
      wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

  // Obtener lista de ventanas
  const std::vector<WindowInfo> &windows = m_windowManager->GetWindows();

  if (windows.empty()) {
    wxStaticText *noWindowsText = new wxStaticText(
        m_panel_bottom, wxID_ANY, "No hay ventanas registradas",
        wxPoint(50, 50), wxDefaultSize);
    noWindowsText->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC,
                                  wxFONTWEIGHT_NORMAL));
    LogDebug("No hay ventanas registradas");
    return;
  }

  // Solo llegamos aquí si hay ventanas registradas
  for (int i = 0; i < windows.size(); i++) {
    // Convertir wstring a string para el logging
    const WindowInfo &window = windows[i];

    // Obtener el PID real de la ventana usando el HWND (winId)
    DWORD processId = 0;
    GetWindowThreadProcessId(window.winId, &processId);

    // Formatear el HWND como hexadecimal
    std::stringstream ss;
    ss << "0x" << std::hex << (uintptr_t)window.winId;
    std::string hwndStr = ss.str();

    // Convertir datos para logging
    std::string fullTitleStr(window.winTitle.begin(), window.winTitle.end());
    std::string classNameStr(window.winClass.begin(), window.winClass.end());
    std::string winOrderStr = std::to_string(window.winOrder);

    // Extraer solo el texto entre corchetes []
    std::string titleStr;
    size_t startPos = fullTitleStr.find("[");
    size_t endPos = fullTitleStr.find("]");

    if (startPos != std::string::npos && endPos != std::string::npos &&
        startPos < endPos) {
      // Extraer texto entre corchetes, incluyendo los corchetes
      titleStr = fullTitleStr.substr(startPos, endPos - startPos + 1);

      // Eliminar espacios en blanco al inicio y final del texto extraído
      titleStr.erase(0, titleStr.find_first_not_of(" "));
      titleStr.erase(titleStr.find_last_not_of(" ") + 1);
    } else {
      // Si no se encuentran corchetes, usar "Ventana X" donde X es el orden
      titleStr = "Ventana " + winOrderStr;
    }

    LogDebug("Ventana HWND " + hwndStr);
    LogDebug("Ventana PID " + std::to_string(processId));
    LogDebug("Ventana Title " + titleStr);
    LogDebug("Ventana Class " + classNameStr);
    LogDebug("Ventana Order " + winOrderStr);
    LogDebug("-------------------------");

    // Añadir un debug para verificar el ancho real del panel padre
    int parentWidth = m_panel_bottom->GetSize().x;

    // Aumentamos el margen visual a 20px por lado para hacerlo más evidente
    int horizontalMargin = 20; // pixels por lado

    // Crear un panel para encapsular cada fila de ventana
    wxPanel *rowPanel = new wxPanel(
        m_panel_bottom, wxID_ANY,
        wxPoint(horizontalMargin,
                50 + i * 60), // Posición vertical con más espacio entre filas
        wxSize(parentWidth - (horizontalMargin * 2) - 16,
               50),       // Margen a cada lado (izquierdo y derecho)
        wxBORDER_SIMPLE); // Borde simple (línea sólida fina)

    // Color de fondo ligeramente diferente para mejorar la visualización
    if (i % 2 == 0) {
      rowPanel->SetBackgroundColour(wxColor(240, 240, 240)); // Gris muy claro
    } else {
      rowPanel->SetBackgroundColour(wxColor(250, 250, 250)); // Casi blanco
    }

    // Cargar el ícono de escritorio
    wxString desktopIconPath = "icons/device-desktop-check.png";
    wxImage desktopImage;
    wxBitmap desktopBitman;
    wxButton *windowButton = nullptr;
    int buttonWidth = 0;
    int buttonXPosition = 10; // Posición inicial del botón

    // Calcular la posición Y para centrar el botón verticalmente en el rowPanel
    int rowPanelHeight = rowPanel->GetSize().GetHeight();
    int buttonHeight = 30; // Altura fija de 30 píxeles como solicita el usuario
    int buttonYPos = (rowPanelHeight - buttonHeight) / 2;
    if (buttonYPos < 0)
      buttonYPos = 0; // Asegurar que no sea negativo

    // Calcular el tamaño del texto del título usando un botón temporal
    wxButton *tempButton = new wxButton(rowPanel, wxID_ANY);
    wxSize textSize = tempButton->GetTextExtent(titleStr);
    tempButton->Destroy(); // Eliminar el botón temporal

    // Intentar cargar el ícono antes de crear el botón principal
    bool iconLoaded = desktopImage.LoadFile(desktopIconPath, wxBITMAP_TYPE_PNG);

    // Calcular el ancho necesario para el texto + icono + márgenes
    int iconSpace = 0;
    if (iconLoaded) {
      // Escalar la imagen a un tamaño adecuado para el botón
      int iconHeight = 20;
      int iconWidth =
          (desktopImage.GetWidth() * iconHeight) / desktopImage.GetHeight();
      desktopImage.Rescale(iconWidth, iconHeight, wxIMAGE_QUALITY_HIGH);
      desktopBitman = wxBitmap(desktopImage);

      iconSpace = iconWidth + 5; // 5px de margen entre icono y texto
    }

    // Crear un botón que contendrá tanto el icono como el título
    // Usando el tamaño y posición calculados para centrarlo verticalmente
    windowButton = new wxButton(
        rowPanel, wxID_ANY, titleStr, wxPoint(buttonXPosition, buttonYPos),
        wxSize(textSize.GetWidth() + iconSpace + 25,
               buttonHeight)); // 25px adicionales de padding

    // rgb(218, 209, 87)
    windowButton->SetBackgroundStyle(wxBG_STYLE_COLOUR);
    windowButton->SetBackgroundColour(wxColor(218, 209, 87));
    windowButton->SetCursor(wxCursor(wxCURSOR_HAND));

    // Almacenar el objeto window en el botón para usarlo al hacer clic
    // Creamos una copia del objeto WindowInfo para evitar problemas de vida
    // útil
    WindowInfo *windowCopy = new WindowInfo(window);
    windowButton->SetClientData(windowCopy);

    // Conectamos el evento de clic al manejador OnActiveWindow
    windowButton->Bind(wxEVT_BUTTON, &MainFrame::OnActiveWindow, this);

    // Configurar la fuente del botón similar a loadWindowsButton
    windowButton->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                                 wxFONTWEIGHT_NORMAL));

    // Establecer el ícono en el botón si se cargó correctamente
    if (iconLoaded) {
      windowButton->SetBitmap(desktopBitman);
      windowButton->SetBitmapMargins(
          wxSize(5, 0)); // Espacio entre el ícono y el texto
      windowButton->SetBitmapPosition(wxLEFT); // Ícono a la izquierda del texto
    } else {
      LogDebug("No se pudo cargar el ícono 'icons/device-desktop-check.png'");
    }

    // Ajustar tamaño del botón si es necesario
    windowButton->SetMinSize(wxSize(200, 35)); // Garantizar un tamaño mínimo

    // Obtener el tamaño real del botón para cálculos posteriores
    wxSize buttonSize = windowButton->GetSize();
    buttonWidth = buttonSize.GetWidth();
    LogDebug("Ancho del botón de ventana: " + std::to_string(buttonWidth));

    // Calcular la posición del checkbox (30px después del final del botón)
    int checkboxXPos = buttonXPosition + buttonWidth + 30;

    // Crear los checkboxes dentro del panel de fila
    // 1. Checkbox de Bucle
    LargeCheckBox *bucleCheckBox =
        new LargeCheckBox(rowPanel, wxID_ANY, "Bucle",
                          wxPoint(checkboxXPos, 10), // Vertically centered
                          wxSize(70, 30), wxBORDER_NONE);

    bucleCheckBox->SetIndicatorSize(20);
    bucleCheckBox->SetBackgroundColour(
        rowPanel->GetBackgroundColour()); // Mismo color de fondo que el panel
    bucleCheckBox->SetForegroundColour(wxColor(0, 0, 0));

    // Obtener el ancho del primer checkbox para calcular la posición del
    // siguiente
    wxSize bucleCheckSize = bucleCheckBox->GetSize();
    LogDebug("Ancho del checkbox de Bucle: " +
             std::to_string(bucleCheckSize.GetWidth()));
    int currentX =
        checkboxXPos + bucleCheckSize.GetWidth() + 10; // 10px de padding
    LogDebug("Posición del checkbox de SyncKeys: " + std::to_string(currentX));

    // 2. Checkbox de SyncKeys
    LargeCheckBox *syncKeysCheckBox =
        new LargeCheckBox(rowPanel, wxID_ANY, "SyncKeys",
                          wxPoint(currentX, 10), // Misma altura que el anterior
                          wxSize(90, 30), wxBORDER_NONE);

    syncKeysCheckBox->SetIndicatorSize(20);
    syncKeysCheckBox->SetBackgroundColour(rowPanel->GetBackgroundColour());
    syncKeysCheckBox->SetForegroundColour(wxColor(0, 0, 0));

    // Actualizar posición X para el siguiente checkbox
    wxSize syncKeysSize = syncKeysCheckBox->GetSize();
    LogDebug("Ancho del checkbox de SyncKeys: " +
             std::to_string(syncKeysSize.GetWidth()));
    currentX += syncKeysSize.GetWidth() + 10; // 10px de padding
    LogDebug("Posición del checkbox de AutoAttackMobs: " +
             std::to_string(currentX));

    // 3. Checkbox de AutoAttackMobs
    LargeCheckBox *autoAttackMobsCheckBox = new LargeCheckBox(
        rowPanel, wxID_ANY, "AutoAttackMobs",
        wxPoint(currentX, 10), // Misma altura que los anteriores
        wxSize(130, 30), wxBORDER_NONE);

    wxSize autoAttackMobsSize = autoAttackMobsCheckBox->GetSize();
    LogDebug("Ancho del checkbox de AutoAttackMobs: " +
             std::to_string(autoAttackMobsSize.GetWidth()));

    autoAttackMobsCheckBox->SetIndicatorSize(20);
    autoAttackMobsCheckBox->SetBackgroundColour(
        rowPanel->GetBackgroundColour());
    autoAttackMobsCheckBox->SetForegroundColour(wxColor(0, 0, 0));
  }
}

void MainFrame::OnActiveWindow(wxCommandEvent &event) {
  // Obtener el botón que generó el evento
  wxButton *button = wxDynamicCast(event.GetEventObject(), wxButton);
  if (button) {
    // Recuperar el objeto WindowInfo asociado al botón
    WindowInfo *windowInfo = static_cast<WindowInfo *>(button->GetClientData());
    if (windowInfo) {
      // Activar la ventana usando las API de Windows
      HWND hwnd = windowInfo->winId;
      if (IsWindow(hwnd)) {
        // Registrar información de la ventana a activar
        std::string windowTitle(windowInfo->winTitle.begin(),
                                windowInfo->winTitle.end());
        LogDebug("Activando ventana: " + windowTitle);

        // Verificar si la ventana está minimizada o no es la ventana activa
        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwnd, &wp);

        // Secuencia completa para garantizar que la ventana se traiga al frente
        // y se active

        // 1. Si la ventana está minimizada, restaurarla
        if (wp.showCmd == SW_SHOWMINIMIZED) {
          LogDebug("Restaurando ventana minimizada: " + windowTitle);
          ShowWindow(hwnd, SW_RESTORE);
          Sleep(50); // Dar tiempo para que la ventana se restaure antes de
                     // activarla
        }

        // 2. Traer al frente y activar usando varios métodos para mayor éxito

        // Intento 1: Usar SwitchToThisWindow (raro, pero eficaz en algunos
        // casos)
        SwitchToThisWindow(hwnd, TRUE);

        // Intento 2: El proceso actual debe estar en primer plano para poder
        // dar primer plano a otra ventana
        DWORD thisPid = GetCurrentProcessId();
        AllowSetForegroundWindow(
            ASFW_ANY); // Permitir que cualquier proceso establezca el foco

        // Intento 3: Intentar que Windows enfoque y active la ventana
        BringWindowToTop(hwnd);
        SetForegroundWindow(hwnd);

        // Intento 4: Enviar eventos de foco
        HWND fgWnd = GetForegroundWindow();
        if (fgWnd != hwnd) {
          // Si todo lo anterior falló, intentar enviar mensajes de activación
          SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
          SendMessage(hwnd, WM_ACTIVATE, WA_ACTIVE, 0);
        }

        // Dar un poco de tiempo para que Windows procese
        Sleep(100);

        // Comprobar si logramos activar la ventana
        fgWnd = GetForegroundWindow();
        if (fgWnd == hwnd) {
          LogDebug("Ventana activada correctamente: " + windowTitle);
        } else {
          LogDebug("Es posible que no se haya podido activar completamente la "
                   "ventana");
        }
      } else {
        LogDebug("La ventana no es válida o ya no existe");
      }
    }
  }
}

void MainFrame::OnLoadWindowsButtonClicked(wxCommandEvent &event) {
  LogDebug("Cargar Ventanas de ROSE");

  if (m_windowManager == nullptr) {
    LogDebug("Error: WindowManager no inicializado");
    return;
  }

  // Restablecer ventanas si ya hay algunas
  if (!m_windowManager->GetWindows().empty()) {
    m_windowManager->ResetWindows();
  }

  // Buscar y registrar ventanas
  bool found = m_windowManager->FindAndRegisterRoseWindows();

  if (!found) {
    LogDebug("No se encontraron ventanas de ROSE");
  }

  // Actualizar interfaz siempre
  UpdateWindowList();
}