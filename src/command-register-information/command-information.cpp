#include "../../include/command/command-raw-metadata.hpp"
#include "../../include/command/command-implementation.hpp"
#include "command-handler-include/command-hanlder-include.hpp"

void CreatedCommandData(){

  CommandMetaData list;
  list.default_name       = "list";
  list.minimun_positional = 0;
  list.maximun_positional = 1;
  list.option_avaible.insert(list.option_avaible.end(),{
    "--all",          // -a: Incluye entradas ocultas
    "--long",         // -l: Formato largo con metadatos
    "--sort",         // Ordenar por: name, size, modified, type
    "--reverse",      // Invierte el orden
    "--dirs-first",   // Muestra directorios antes que archivos
    "--filter",       // Filtra por patrón glob: *.cpp, test_*
    "--no-header",
  });
  list.handler = LIST_HANDLER;
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
  inspect.default_name       = "inspect";
  inspect.minimun_positional = 0;
  inspect.maximun_positional = 100;
  inspect.option_avaible.insert(inspect.option_avaible.end(),{
    "--all",             // -a: Incluye archivos ocultos
    "--recursive",       // -r: Inspecciona subdirectorios recursivamente
    "--depth",           // -d: Profundidad máxima de recursión
    "--follow-symlinks", // Sigue enlaces simbólicos
    "--fields",          // Selecciona campos: name,size,modified,permissions,type,inode
    "--sort",            // Ordena resultado: name, size, modified
    "--reverse",         // Invierte el orden del sort
    "--filter",
    "--stats",
    "--stats-only",
  });
  inspect.handler = INSPECT_HANDLER;
  GeneralCommandLog(inspect);

  CommandMetaData move;
  move.default_name       = "move";
  move.minimun_positional = 2;
  move.maximun_positional = 2;
  move.option_avaible.insert(move.option_avaible.end(),{
    "--force",        // -f: Sobreescribe destino sin preguntar
    "--no-overwrite", // Falla si el destino existe
    "--dry-run",      // -n: Simula la operación
    "--verbose"       // Muestra detalles de la operación
  });
  // move.handler = MoveHandler;
  GeneralCommandLog(move);


  CommandMetaData delete_cmd;  // 'delete' es keyword en C++
  delete_cmd.default_name       = "delete";
  delete_cmd.minimun_positional = 1;
  delete_cmd.maximun_positional = 100;
  delete_cmd.option_avaible.insert(delete_cmd.option_avaible.end(),{
    "--recursive",  // -r: Elimina directorios y su contenido
    "--force",      // -f: Sin confirmación interactiva
    "--dry-run",    // -n: Muestra qué se eliminaría
    "--verbose",    // Muestra cada archivo eliminado
    "--match"       // Elimina solo entradas que coincidan con el patrón
  });
  // delete_cmd.handler = DeleteHandler;
  GeneralCommandLog(delete_cmd);

  CommandMetaData find;
  find.default_name       = "find";
  find.minimun_positional = 1;
  find.maximun_positional = 1;
  find.option_avaible.insert(find.option_avaible.end(),{
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
  });
  // find.handler = FindHandler;
  GeneralCommandLog(find);

}
