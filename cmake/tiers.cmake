# ============================================================================
# kron — Definiciones de Build Tiers
# cmake/tiers.cmake
# ============================================================================
# Define feature flags y comandos disponibles por tier de compilación
# Incluido automáticamente desde CMakeLists.txt principal
# ============================================================================

# ----------------------------------------------------------------------------
# Conversión de tier a minúsculas para nombre de binario
# ----------------------------------------------------------------------------
string(TOLOWER "${KRON_TIER}" KRON_TIER_LOWERCASE)

# ----------------------------------------------------------------------------
# Tier 1: BASIC
# ----------------------------------------------------------------------------
# Usuario que reemplaza herramientas UNIX estándar
# Comandos: inspect, list, copy, move, delete, find
# ----------------------------------------------------------------------------
set(KRON_TIER_BASIC_COMMANDS
    "inspect"
    "list"
    "copy"
    "move"
    "delete"
    "find"
)

if(KRON_TIER STREQUAL "BASIC")
    set(KRON_TIER_DEFINES
        KRON_TIER_BASIC
        KRON_CMD_INSPECT
        KRON_CMD_LIST
        KRON_CMD_COPY
        KRON_CMD_MOVE
        KRON_CMD_DELETE
        KRON_CMD_FIND
    )
    
    message(STATUS "Tier BASIC activado")
    message(STATUS "  → inspect: Inspección de metadatos")
    message(STATUS "  → list:    Listado de directorios")
    message(STATUS "  → copy:    Copia de archivos")
    message(STATUS "  → move:    Movimiento/renombrado")
    message(STATUS "  → delete:  Eliminación")
    message(STATUS "  → find:    Búsqueda de archivos")
endif()

# ----------------------------------------------------------------------------
# Tier 2: STANDARD
# ----------------------------------------------------------------------------
# Desarrollador en flujo de trabajo real
# Comandos: Todo BASIC + hash, diff, dupes, sync, tree, stats
# ----------------------------------------------------------------------------
set(KRON_TIER_STANDARD_COMMANDS
    ${KRON_TIER_BASIC_COMMANDS}
    "hash"
    "diff"
    "dupes"
    "sync"
    "tree"
    "stats"
)

if(KRON_TIER STREQUAL "STANDARD")
    set(KRON_TIER_DEFINES
        KRON_TIER_STANDARD
        KRON_CMD_INSPECT
        KRON_CMD_LIST
        KRON_CMD_COPY
        KRON_CMD_MOVE
        KRON_CMD_DELETE
        KRON_CMD_FIND
        KRON_CMD_HASH
        KRON_CMD_DIFF
        KRON_CMD_DUPES
        KRON_CMD_SYNC
        KRON_CMD_TREE
        KRON_CMD_STATS
    )
    
    message(STATUS "Tier STANDARD activado")
    message(STATUS "  → Todos los comandos BASIC")
    message(STATUS "  → hash:    Checksums y verificación")
    message(STATUS "  → diff:    Comparación de archivos/directorios")
    message(STATUS "  → dupes:   Detección de duplicados")
    message(STATUS "  → sync:    Sincronización diferencial")
    message(STATUS "  → tree:    Visualización de estructura")
    message(STATUS "  → stats:   Análisis agregado")
endif()

# ----------------------------------------------------------------------------
# Tier 3: COMPLETE
# ----------------------------------------------------------------------------
# Power user, sysadmin, exploración profunda
# Comandos: Todo STANDARD + compress, version, watch, audit, profile, export
# ----------------------------------------------------------------------------
set(KRON_TIER_COMPLETE_COMMANDS
    ${KRON_TIER_STANDARD_COMMANDS}
    "compress"
    "version"
    "watch"
    "audit"
    "profile"
    "export"
)

if(KRON_TIER STREQUAL "COMPLETE")
    set(KRON_TIER_DEFINES
        KRON_TIER_COMPLETE
        KRON_CMD_INSPECT
        KRON_CMD_LIST
        KRON_CMD_COPY
        KRON_CMD_MOVE
        KRON_CMD_DELETE
        KRON_CMD_FIND
        KRON_CMD_HASH
        KRON_CMD_DIFF
        KRON_CMD_DUPES
        KRON_CMD_SYNC
        KRON_CMD_TREE
        KRON_CMD_STATS
        KRON_CMD_COMPRESS
        KRON_CMD_VERSION
        KRON_CMD_WATCH
        KRON_CMD_AUDIT
        KRON_CMD_PROFILE
        KRON_CMD_EXPORT
    )
    
    message(STATUS "Tier COMPLETE activado")
    message(STATUS "  → Todos los comandos STANDARD")
    message(STATUS "  → compress: Compresión/descompresión")
    message(STATUS "  → version:  Versionado ligero")
    message(STATUS "  → watch:    Monitoreo en tiempo real")
    message(STATUS "  → audit:    Historial de operaciones")
    message(STATUS "  → profile:  Análisis forense completo")
    message(STATUS "  → export:   Exportación estructurada")
endif()

# ----------------------------------------------------------------------------
# Uso en código C++
# ----------------------------------------------------------------------------
# Estos defines permiten compilación condicional:
#
# #ifdef KRON_CMD_HASH
#     // Registrar comando hash
#     register_command("hash", hash_execute);
# #endif
#
# #ifdef KRON_TIER_COMPLETE
#     // Funcionalidad solo en tier complete
# #endif
#
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# Helper: Verificar si un comando está disponible en el tier actual
# ----------------------------------------------------------------------------
# Uso en CMake (si fuera necesario en el futuro):
#   if("hash" IN_LIST KRON_TIER_${KRON_TIER}_COMMANDS)
#       # hacer algo específico para hash
#   endif()
# ----------------------------------------------------------------------------
