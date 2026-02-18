#include "../../include/command/command-raw-metadata.hpp"
#include "../../include/command/command-implementation.hpp"

void CreatedCommandData(){
  // ============================================================================
  // TIER: kron-basic
  // ============================================================================
  
  // --------------------------------------------------------------------------
  // Comando: list
  // --------------------------------------------------------------------------
  /*
   * Lista el contenido de un directorio. Equivalente a `ls` pero con control 
   * explícito sobre el formato y los campos.
   * 
   * Uso: kron list [ruta]
   * Default: directorio actual
   */
  CommandMetaData list;
  list.default_name = "list";
  list.option_avaible = {
    "--all",          // -a: Incluye entradas ocultas
    "--long",         // -l: Formato largo con metadatos
    "--sort",         // Ordenar por: name, size, modified, type
    "--reverse",      // Invierte el orden
    "--dirs-first",   // Muestra directorios antes que archivos
    "--filter",       // Filtra por patrón glob: *.cpp, test_*
    "--no-header"     // Omite la cabecera de la tabla
  };
  // list.handler = ListHandler;
  
  GeneralCommandLog(list);

  // --------------------------------------------------------------------------
  // Comando: inspect
  // --------------------------------------------------------------------------
  /*
   * Muestra metadatos completos de uno o más archivos o directorios. 
   * Es el comando núcleo del sistema. Todo lo demás lo complementa.
   * 
   * Uso: kron inspect <ruta> [ruta2 ...]
   * Acepta múltiples rutas separadas por espacio
   */
  CommandMetaData inspect;
  inspect.default_name = "inspect";
  inspect.option_avaible = {
    "--all",             // -a: Incluye archivos ocultos
    "--recursive",       // -r: Inspecciona subdirectorios recursivamente
    "--depth",           // -d: Profundidad máxima de recursión
    "--follow-symlinks", // Sigue enlaces simbólicos
    "--fields",          // Selecciona campos: name,size,modified,permissions,type,inode
    "--sort",            // Ordena resultado: name, size, modified
    "--reverse"          // Invierte el orden del sort
  };
  // inspect.handler = InspectHandler;
  
  GeneralCommandLog(inspect);

  // --------------------------------------------------------------------------
  // Comando: copy
  // --------------------------------------------------------------------------
  /*
   * Copia un archivo o directorio. Con control explícito sobre 
   * comportamiento ante conflictos.
   * 
   * Uso: kron copy <origen> <destino>
   * Origen y destino pueden ser archivos o directorios
   */
  CommandMetaData copy;
  copy.default_name = "copy";
  copy.option_avaible = {
    "--recursive",      // -r: Copia directorios recursivamente
    "--force",          // -f: Sobreescribe destino sin preguntar
    "--no-overwrite",   // Falla si el destino existe
    "--skip-existing",  // Omite archivos que ya existen en destino
    "--preserve",       // -p: Preserva metadatos: fechas, permisos
    "--dry-run",        // -n: Muestra qué se copiaría sin ejecutar
    "--verbose"         // Muestra cada archivo copiado
  };
  // copy.handler = CopyHandler;
  
  GeneralCommandLog(copy);

  // --------------------------------------------------------------------------
  // Comando: move
  // --------------------------------------------------------------------------
  /*
   * Mueve o renombra un archivo o directorio.
   * 
   * Uso: kron move <origen> <destino>
   * Destino puede ser nueva ubicación o nuevo nombre
   */
  CommandMetaData move;
  move.default_name = "move";
  move.option_avaible = {
    "--force",        // -f: Sobreescribe destino sin preguntar
    "--no-overwrite", // Falla si el destino existe
    "--dry-run",      // -n: Simula la operación
    "--verbose"       // Muestra detalles de la operación
  };
  // move.handler = MoveHandler;
  
  GeneralCommandLog(move);

  // --------------------------------------------------------------------------
  // Comando: delete
  // --------------------------------------------------------------------------
  /*
   * Elimina archivos o directorios. Requiere confirmación explícita para 
   * directorios no vacíos salvo --force.
   * 
   * Uso: kron delete <ruta> [ruta2 ...]
   * Acepta múltiples rutas a eliminar
   */
  CommandMetaData delete_cmd;  // 'delete' es keyword en C++, usar delete_cmd
  delete_cmd.default_name = "delete";
  delete_cmd.option_avaible = {
    "--recursive",  // -r: Elimina directorios y su contenido
    "--force",      // -f: Sin confirmación interactiva
    "--dry-run",    // -n: Muestra qué se eliminaría
    "--verbose",    // Muestra cada archivo eliminado
    "--match"       // Elimina solo entradas que coincidan con el patrón
  };
  // delete_cmd.handler = DeleteHandler;
  
  GeneralCommandLog(delete_cmd);
  

  // --------------------------------------------------------------------------
  // Comando: find
  // --------------------------------------------------------------------------
  /*
   * Busca archivos en un directorio según criterios combinables.
   * Los criterios son acumulativos (AND lógico).
   * 
   * Uso: kron find <directorio> [criterios]
   * Raíz de búsqueda como primer argumento posicional
   */
  CommandMetaData find;
  find.default_name = "find";
  find.option_avaible = {
    "--name",            // Busca por nombre (glob): test_*.cpp, *config*
    "--ext",             // Busca por extensión: cpp, .cpp (ambos válidos)
    "--type",            // Tipo: file, dir, symlink
    "--size-gt",         // Archivos mayores a n bytes/KB/MB: 10MB
    "--size-lt",         // Archivos menores a n
    "--modified-after",  // Fecha ISO: 2024-01-01
    "--modified-before", // Fecha ISO: 2024-01-01
    "--recursive",       // -r: Búsqueda recursiva (default: true)
    "--depth",           // Profundidad máxima
    "--no-hidden"        // Excluye archivos ocultos
  };
  // find.handler = FindHandler;
  
  GeneralCommandLog(find);

  // ============================================================================
  // NOTAS DE IMPLEMENTACIÓN
  // ============================================================================
  /*
   * FLAGS GLOBALES (aplicables a todos los comandos):
   * --help, -h       : Muestra ayuda del comando
   * --version, -v    : Muestra versión del binario y tier
   * --no-color       : Desactiva colores ANSI
   * --quiet, -q      : Suprime output informativo, solo errores
   * --verbose        : Output extendido (algunos comandos específicos)
   * --dry-run, -n    : Simula sin ejecutar (comandos destructivos)
   * --output, -o     : Formato de salida: plain, json, csv
   * 
   * CONFLICTOS A VALIDAR EN EL PARSER:
   * - copy/move: --force ⊗ --no-overwrite
   * - copy: --force ⊗ --skip-existing
   * - inspect/find: --depth sin --recursive → warning
   * 
   * CONFIRMACIONES INTERACTIVAS:
   * - delete sin --force → pide confirmación en TTY
   * - move con --force → sobreescribe sin preguntar
   * - copy con conflictos → comportamiento según flags
   */
}
