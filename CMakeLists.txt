cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(rose_multibox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuración específica para compilar con Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Compilando con Clang - ajustando configuración...")
    # Añadir flags específicos para Clang si es necesario
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
    
    # Asegurar que usamos la versión correcta de MSVC runtime cuando usamos Clang con wxWidgets
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Important for using DLLs on Windows
if(WIN32)
    add_definitions(-DWXUSINGDLL -D_CRT_SECURE_NO_WARNINGS)
endif()

# Configuración wxWidgets
set(wxWidgets_USE_STATIC OFF)
set(wxWidgets_USE_UNICODE ON)

# Usar variable de entorno WXWIN o intentar varias rutas comunes
set(wxWidgets_ROOT_DIR "$ENV{WXWIN}")
message(STATUS "Using wxWidgets from ENV: ${wxWidgets_ROOT_DIR}")

# Configuraciones específicas para Windows
if(WIN32)
    # Buscar las bibliotecas de Debug primero para la configuración Debug
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
        # En VS, el tipo de compilación no siempre está definido, pero el modo Debug es el predeterminado
        set(wxWidgets_CONFIGURATION mswud)
        set(WX_LIB_DIR_SUFFIX "_dll")
    else()
        set(wxWidgets_CONFIGURATION mswu)
        set(WX_LIB_DIR_SUFFIX "_dll")
    endif()

    # Rutas comunes para las DLLs de wxWidgets
    set(POSSIBLE_DLL_PATHS
        "${wxWidgets_ROOT_DIR}/lib/vc_x64${WX_LIB_DIR_SUFFIX}"
        "${wxWidgets_ROOT_DIR}/lib/vc14x_x64${WX_LIB_DIR_SUFFIX}"
        "${wxWidgets_ROOT_DIR}/lib/vc14x_x64_dll"
    )
    
    # Encontrar la primera ruta válida que contenga DLLs
    foreach(DLL_PATH ${POSSIBLE_DLL_PATHS})
        if(EXISTS "${DLL_PATH}")
            set(wxWidgets_LIB_DIR "${DLL_PATH}")
            message(STATUS "Usando bibliotecas wxWidgets de: ${wxWidgets_LIB_DIR}")
            break()
        endif()
    endforeach()
    
    # Añadir rutas de búsqueda para bibliotecas
    list(APPEND CMAKE_PREFIX_PATH "${wxWidgets_LIB_DIR}")
endif()

# Buscar wxWidgets con los componentes necesarios
find_package(wxWidgets COMPONENTS core base net REQUIRED)

# Verificar que se encontró wxWidgets
if(wxWidgets_FOUND)
    message(STATUS "wxWidgets encontrado: ${wxWidgets_VERSION}")
    message(STATUS "wxWidgets_INCLUDE_DIRS: ${wxWidgets_INCLUDE_DIRS}")
    message(STATUS "wxWidgets_LIBRARIES: ${wxWidgets_LIBRARIES}")
    include(${wxWidgets_USE_FILE})
else()
    message(FATAL_ERROR "wxWidgets no encontrado")
endif()

# Archivos fuente
set(SRCS 
    src/main.cpp
    src/utils/Logger.cpp
    src/components/LargeCheckBox.cpp
    src/mainframes/MainFrame.cpp
)

# Archivos de cabecera
set(HEADERS
    src/utils/Logger.h
    src/components/LargeCheckBox.h
    src/mainframes/MainFrame.h
)

# Crear ejecutable para Windows
if(APPLE)
    add_executable(RoseMultibox MACOSX_BUNDLE ${SRCS})
    set_target_properties(RoseMultibox PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
    # WIN32 es necesario para que busque WinMain en lugar de main
    add_executable(RoseMultibox WIN32 ${SRCS} RoseMultibox.exe.manifest)
endif()

# Añadir los directorios de include explícitamente para mejorar la compatibilidad con Clang
target_include_directories(RoseMultibox PRIVATE ${wxWidgets_INCLUDE_DIRS})

# Vincular con las bibliotecas de wxWidgets
target_link_libraries(RoseMultibox PRIVATE ${wxWidgets_LIBRARIES})

# Copiar las DLLs de wxWidgets al directorio de salida en Windows
if(WIN32)
    # Configuramos la ruta de las DLLs de wxWidgets
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
        file(GLOB WX_DLLS "${wxWidgets_LIB_DIR}/wxbase32ud_*.dll" "${wxWidgets_LIB_DIR}/wxmsw32ud_*.dll")
        add_custom_command(TARGET RoseMultibox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${WX_DLLS}
                $<TARGET_FILE_DIR:RoseMultibox>)
    else()
        file(GLOB WX_DLLS "${wxWidgets_LIB_DIR}/wxbase32u_*.dll" "${wxWidgets_LIB_DIR}/wxmsw32u_*.dll")
        add_custom_command(TARGET RoseMultibox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${WX_DLLS}
                $<TARGET_FILE_DIR:RoseMultibox>)
    endif()
    message(STATUS "Las DLLs de wxWidgets se copiarán a: $<TARGET_FILE_DIR:RoseMultibox>")
endif()
