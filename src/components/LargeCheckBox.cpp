#include "LargeCheckBox.h"

BEGIN_EVENT_TABLE(LargeCheckBox, wxControl)
EVT_PAINT(LargeCheckBox::OnPaint)
EVT_LEFT_DOWN(LargeCheckBox::OnLeftDown)
EVT_LEFT_UP(LargeCheckBox::OnLeftUp)
EVT_KEY_DOWN(LargeCheckBox::OnKeyDown)
EVT_SET_FOCUS(LargeCheckBox::OnSetFocus)
EVT_KILL_FOCUS(LargeCheckBox::OnKillFocus)
END_EVENT_TABLE()

LargeCheckBox::LargeCheckBox(wxWindow *parent, wxWindowID id,
                             const wxString &label, const wxPoint &pos,
                             const wxSize &size, long style)
    : wxControl(parent, id, pos, size, wxBORDER_NONE | style), m_checked(false),
      m_hasFocus(false), m_indicatorSize(24) // Tamaño predeterminado más grande
{
  SetLabel(label);
  SetBackgroundStyle(wxBG_STYLE_PAINT);

  // Calcular tamaño ideal basado en el texto y el tamaño del indicador
  SetInitialSize(size);
}

LargeCheckBox::~LargeCheckBox() {}

void LargeCheckBox::SetValue(bool checked) {
  if (m_checked != checked) {
    m_checked = checked;
    Refresh();

    // Enviar un evento de cambio
    wxCommandEvent event(wxEVT_CHECKBOX, GetId());
    event.SetInt(m_checked ? 1 : 0);
    event.SetEventObject(this);
    GetEventHandler()->ProcessEvent(event);
  }
}

void LargeCheckBox::OnPaint(wxPaintEvent &WXUNUSED(event)) {
  wxPaintDC dc(this);

  // Obtener el tamaño y posición
  wxSize size = GetSize();
  wxRect rect = GetClientRect();

  // Dibujar el fondo
  dc.SetBackground(wxBrush(GetBackgroundColour()));
  dc.Clear();

  wxRect checkRect = GetIndicatorRect();

  // Si está marcado, usar un fondo verde para el cuadro del checkbox
  if (m_checked) {
    // Usar un borde más oscuro para el contraste
    dc.SetPen(wxPen(wxColour(0, 100, 0)));       // Verde oscuro para el borde
    dc.SetBrush(wxBrush(wxColour(50, 205, 50))); // Verde claro para el relleno
  } else {
    // Estilo normal para desmarcado
    dc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW)));
    dc.SetBrush(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE)));
  }

  // Dibujar el cuadro del checkbox
  dc.DrawRectangle(checkRect);

  // Dibujar el foco si lo tiene
  if (m_hasFocus) {
    wxRect focusRect = checkRect;
    focusRect.Inflate(2);
    dc.SetPen(wxPen(*wxBLACK, 1, wxPENSTYLE_DOT));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(focusRect);
  }

  // Dibujar el texto si hay alguno
  if (!GetLabel().empty()) {
    dc.SetFont(GetFont());
    dc.SetTextForeground(GetForegroundColour());

    // Espacio entre el indicador y el texto
    int textX = checkRect.GetRight() + 10;
    int textY = (size.y - dc.GetCharHeight()) / 2;

    dc.DrawText(GetLabel(), textX, textY);
  }
}

void LargeCheckBox::OnLeftDown(wxMouseEvent &event) {
  // Capturamos el mouse para asegurarnos de recibir el evento LEFT_UP
  CaptureMouse();

  // Solo establecemos foco pero no cambiamos estado todavía
  if (!HasFocus())
    SetFocus();

  event.Skip();
}

void LargeCheckBox::OnLeftUp(wxMouseEvent &event) {
  // Si habíamos capturado el mouse, lo liberamos
  if (HasCapture())
    ReleaseMouse();

  // Obtenemos la posición del clic
  wxPoint pos = event.GetPosition();
  wxRect checkRect = GetIndicatorRect();

  // Solo cambiamos el estado si el clic termina dentro del área del checkbox
  if (checkRect.Contains(pos)) {
    SetValue(!m_checked);
  }
}

void LargeCheckBox::OnKeyDown(wxKeyEvent &event) {
  if (event.GetKeyCode() == WXK_SPACE) {
    SetValue(!m_checked);
  } else {
    event.Skip();
  }
}

void LargeCheckBox::OnSetFocus(wxFocusEvent &WXUNUSED(event)) {
  m_hasFocus = true;
  Refresh();
}

void LargeCheckBox::OnKillFocus(wxFocusEvent &WXUNUSED(event)) {
  m_hasFocus = false;
  Refresh();
}

wxRect LargeCheckBox::GetIndicatorRect() const {
  wxSize size = GetSize();
  int y = (size.y - m_indicatorSize) / 2;
  return wxRect(5, y, m_indicatorSize, m_indicatorSize);
}
