# ============================================================================
# kron — CLI multifuncional de manipulación de archivos
# CMakeLists.txt principal
# ============================================================================
# Requiere: CMake 3.15+, C++20
# Build tiers: BASIC, STANDARD, COMPLETE (ver cmake/tiers.cmake)
# ============================================================================

cmake_minimum_required(VERSION 3.15)

# ----------------------------------------------------------------------------
# Configuración del proyecto
# ----------------------------------------------------------------------------
project(kron
    VERSION 0.1.0
    DESCRIPTION "CLI multifuncional de manipulación de archivos"
    LANGUAGES CXX
)

# ----------------------------------------------------------------------------
# Estándar C++20 y configuración del compilador
# ----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Genera compile_commands.json para clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------------------------------------------------------
# Configuración de build tier (BASIC por defecto)
# ----------------------------------------------------------------------------
# Uso: cmake -DKRON_TIER=STANDARD ..
#      cmake -DKRON_TIER=COMPLETE ..
set(KRON_TIER "BASIC" CACHE STRING "Build tier: BASIC, STANDARD, COMPLETE")
set_property(CACHE KRON_TIER PROPERTY STRINGS BASIC STANDARD COMPLETE)

# Cargar definiciones de tiers
include(cmake/tiers.cmake)

# Validar tier seleccionado
if(NOT KRON_TIER STREQUAL "BASIC" AND 
   NOT KRON_TIER STREQUAL "STANDARD" AND 
   NOT KRON_TIER STREQUAL "COMPLETE")
    message(FATAL_ERROR "KRON_TIER inválido: ${KRON_TIER}. Use BASIC, STANDARD o COMPLETE")
endif()

message(STATUS "===========================================")
message(STATUS "kron v${PROJECT_VERSION}")
message(STATUS "Build Tier: ${KRON_TIER}")
message(STATUS "Comandos activos: ver cmake/tiers.cmake")
message(STATUS "===========================================")

# ----------------------------------------------------------------------------
# Flags de compilación por plataforma y compilador
# ----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC y Clang
    add_compile_options(
        -Wall                  # Warnings básicos
        -Wextra                # Warnings extras
        -Wpedantic             # Conformidad estricta al estándar
        -Wconversion           # Conversiones implícitas peligrosas
        -Wsign-conversion      # Conversiones de signo
        -Wshadow               # Variable shadowing
        -Wnon-virtual-dtor     # Destructor no virtual en clase base
        -Wold-style-cast       # Cast estilo C
        -Wcast-align           # Alineamiento en casts
        -Wunused               # Variables/funciones no usadas
        -Woverloaded-virtual   # Virtual function hiding
        -Wformat=2             # Format string security
    )
    
    # Flags adicionales en modo Debug
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
        add_compile_definitions(KRON_DEBUG)
    endif()
    
    # Flags adicionales en modo Release
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3 -DNDEBUG)
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # MSVC
    add_compile_options(
        /W4                    # Warning level 4
        /permissive-          # Conformancia estricta
        /Zc:__cplusplus       # Define __cplusplus correctamente
    )
    
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Zi /Od)
    endif()
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2)
    endif()
endif()

# ----------------------------------------------------------------------------
# Directorios de include
# ----------------------------------------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/include)

# ----------------------------------------------------------------------------
# Recolección de archivos fuente
# ----------------------------------------------------------------------------
file(GLOB_RECURSE KRON_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# Excluir archivos de test del binario principal
list(FILTER KRON_SOURCES EXCLUDE REGEX ".*/tests/.*")

# Mostrar fuentes encontrados (útil para debug de CMake)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Archivos fuente encontrados:")
    foreach(src ${KRON_SOURCES})
        message(STATUS "  ${src}")
    endforeach()
endif()

# ----------------------------------------------------------------------------
# Target principal: ejecutable kron
# ----------------------------------------------------------------------------
add_executable(kron ${KRON_SOURCES})

# Propiedades del target
set_target_properties(kron PROPERTIES
    OUTPUT_NAME "kron-${KRON_TIER_LOWERCASE}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
)

# Definir macros de tier en tiempo de compilación
target_compile_definitions(kron PRIVATE
    KRON_VERSION="${PROJECT_VERSION}"
    KRON_TIER="${KRON_TIER}"
    ${KRON_TIER_DEFINES}  # Definido en tiers.cmake
)

# Enlazar con filesystem si es necesario (GCC < 9, Clang < 9)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(kron stdc++fs)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(kron c++fs)
endif()

# ----------------------------------------------------------------------------
# Tests (opcional, solo si se requiere explícitamente)
# ----------------------------------------------------------------------------
option(KRON_BUILD_TESTS "Compilar tests unitarios" OFF)

if(KRON_BUILD_TESTS)
    enable_testing()
    
    file(GLOB TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/*.cpp")
    
    foreach(test_src ${TEST_SOURCES})
        get_filename_component(test_name ${test_src} NAME_WE)
        
        add_executable(${test_name} ${test_src})
        
        target_include_directories(${test_name} PRIVATE ${PROJECT_SOURCE_DIR}/include)
        target_compile_definitions(${test_name} PRIVATE ${KRON_TIER_DEFINES})
        
        add_test(NAME ${test_name} COMMAND ${test_name})
        
        message(STATUS "Test registrado: ${test_name}")
    endforeach()
endif()

# ----------------------------------------------------------------------------
# Instalación (opcional)
# ----------------------------------------------------------------------------
install(TARGETS kron
    RUNTIME DESTINATION bin
)

# ----------------------------------------------------------------------------
# Resumen de configuración
# ----------------------------------------------------------------------------
message(STATUS "")
message(STATUS "Configuración completada:")
message(STATUS "  Compilador: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Tier: ${KRON_TIER}")
message(STATUS "  Binario: kron-${KRON_TIER_LOWERCASE}")
message(STATUS "  compile_commands.json: ${CMAKE_BINARY_DIR}/compile_commands.json")
message(STATUS "")
