#pragma once
#include <wx/control.h>
#include <wx/wx.h>

class LargeCheckBox : public wxControl {
public:
  // Constructor
  LargeCheckBox(wxWindow *parent, wxWindowID id,
                const wxString &label = wxEmptyString,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize, long style = 0);

  // Destructor virtual
  virtual ~LargeCheckBox();

  // Obtener/Establecer el estado marcado
  bool GetValue() const { return m_checked; }
  void SetValue(bool checked);

  // Tamaño del indicador cuadrado personalizable
  void SetIndicatorSize(int size) {
    m_indicatorSize = size;
    Refresh();
  }
  int GetIndicatorSize() const { return m_indicatorSize; }

protected:
  // Eventos para manejar el comportamiento del control
  void OnPaint(wxPaintEvent &event);
  void OnLeftDown(wxMouseEvent &event);
  void OnLeftUp(wxMouseEvent &event);
  void OnKeyDown(wxKeyEvent &event);
  void OnSetFocus(wxFocusEvent &event);
  void OnKillFocus(wxFocusEvent &event);

  // Estado del control
  bool m_checked;
  bool m_hasFocus;

  // Tamaño personalizado del indicador
  int m_indicatorSize;

  // Calcular el rectángulo del indicador
  wxRect GetIndicatorRect() const;

private:
  DECLARE_EVENT_TABLE()
};
