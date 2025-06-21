#pragma once

#include <wx/string.h>

// Forward declaration of wxString
class wxString;

// Función de logging principal (para textos wxString)
void LogDebug(const wxString &mensaje);

// Función para loggear wxString directamente (mejor manejo de caracteres
// españoles)
void LogDebugW(const wxString &mensaje);
