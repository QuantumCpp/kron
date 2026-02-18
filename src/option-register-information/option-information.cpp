#include "../../include/option/option-raw-metadata.hpp"
#include "../../include/option/option-implementation.hpp"

void CreatedOptionData(){
  // ============================================================================
  // FLAGS GLOBALES (aplicables a todos los comandos de kron-basic)
  // ============================================================================
  
  // --------------------------------------------------------------------------
  // --help, -h
  // Muestra ayuda del comando activo
  // --------------------------------------------------------------------------
  OptionMetaData help;
  help.normalized_name = "--help";
  help.alias_name = "-h";
  help.conflict_name = {};
  help.data_type = TypeDataReceived::NONE;
  // help.handler = HelpHandler;
  GeneralOptionLog(help);
  
  // --------------------------------------------------------------------------
  // --version, -v
  // Muestra versión del binario y tier
  // --------------------------------------------------------------------------
  OptionMetaData version;
  version.normalized_name = "--version";
  version.alias_name = "-v";
  version.conflict_name = {};
  version.data_type = TypeDataReceived::NONE;
  // version.handler = VersionHandler;
  GeneralOptionLog(version);
  
  // --------------------------------------------------------------------------
  // --no-color
  // Desactiva colores ANSI en output
  // --------------------------------------------------------------------------
  OptionMetaData no_color;
  no_color.normalized_name = "--no-color";
  no_color.alias_name = "";
  no_color.conflict_name = {};
  no_color.data_type = TypeDataReceived::NONE;
  // no_color.handler = std::monostate{};  // No requiere procesamiento especial
  GeneralOptionLog(no_color);
  
  // --------------------------------------------------------------------------
  // --quiet, -q
  // Suprime output informativo, solo errores
  // --------------------------------------------------------------------------
  OptionMetaData quiet;
  quiet.normalized_name = "--quiet";
  quiet.alias_name = "-q";
  quiet.conflict_name = {
    "--verbose"  // Mutuamente excluyente con verbose
  };
  quiet.data_type = TypeDataReceived::NONE;
  // quiet.handler = std::monostate{};
  GeneralOptionLog(quiet);
  
  // --------------------------------------------------------------------------
  // --verbose
  // Output extendido con detalles internos
  // --------------------------------------------------------------------------
  OptionMetaData verbose;
  verbose.normalized_name = "--verbose";
  verbose.alias_name = "";
  verbose.conflict_name = {
    "--quiet"  // Mutuamente excluyente con quiet
  };
  verbose.data_type = TypeDataReceived::NONE;
  // verbose.handler = std::monostate{};
  GeneralOptionLog(verbose);
  
  // --------------------------------------------------------------------------
  // --dry-run, -n
  // Simula la operación sin ejecutarla
  // --------------------------------------------------------------------------
  OptionMetaData dry_run;
  dry_run.normalized_name = "--dry-run";
  dry_run.alias_name = "-n";
  dry_run.conflict_name = {};
  dry_run.data_type = TypeDataReceived::NONE;
  // dry_run.handler = std::monostate{};
  GeneralOptionLog(dry_run);
  
  // --------------------------------------------------------------------------
  // --output, -o
  // Formato de salida: plain, json, csv
  // --------------------------------------------------------------------------
  OptionMetaData output;
  output.normalized_name = "--output";
  output.alias_name = "-o";
  output.conflict_name = {};
  output.data_type = TypeDataReceived::EXTENSION;  // Recibe string: plain/json/csv
  // output.handler = OutputFormatHandler;
  GeneralOptionLog(output);
  
  // ============================================================================
  // FLAGS DE VISUALIZACIÓN Y CONSULTA
  // ============================================================================
  
  // --------------------------------------------------------------------------
  // --all, -a
  // Incluye archivos/directorios ocultos
  // Usado en: list, inspect
  // --------------------------------------------------------------------------
  OptionMetaData all;
  all.normalized_name = "--all";
  all.alias_name = "-a";
  all.conflict_name = {
    "--no-hidden"  // Semánticamente opuesto (usado en find)
  };
  all.data_type = TypeDataReceived::NONE;
  // all.handler = FilteringProcess que incluye entradas con '.' al inicio
  GeneralOptionLog(all);
  
  // --------------------------------------------------------------------------
  // --long, -l
  // Formato largo con metadatos completos
  // Usado en: list
  // --------------------------------------------------------------------------
  OptionMetaData long_format;
  long_format.normalized_name = "--long";
  long_format.alias_name = "-l";
  long_format.conflict_name = {};
  long_format.data_type = TypeDataReceived::NONE;
  // long_format.handler = std::monostate{};  // Afecta formato de output
  GeneralOptionLog(long_format);
  
  // --------------------------------------------------------------------------
  // --recursive, -r
  // Procesa subdirectorios recursivamente
  // Usado en: inspect, copy, delete, find
  // --------------------------------------------------------------------------
  OptionMetaData recursive;
  recursive.normalized_name = "--recursive";
  recursive.alias_name = "-r";
  recursive.conflict_name = {};
  recursive.data_type = TypeDataReceived::NONE;
  // recursive.handler = FilteringProcess que expande directorios
  GeneralOptionLog(recursive);
  
  // --------------------------------------------------------------------------
  // --depth, -d
  // Profundidad máxima de recursión
  // Usado en: inspect, find
  // Requiere: --recursive (o da warning)
  // --------------------------------------------------------------------------
  OptionMetaData depth;
  depth.normalized_name = "--depth";
  depth.alias_name = "-d";
  depth.conflict_name = {};
  depth.data_type = TypeDataReceived::EXTENSION;  // Recibe int como string
  // depth.handler = FilteringProcess que limita profundidad
  GeneralOptionLog(depth);
  
  // --------------------------------------------------------------------------
  // --sort
  // Ordena resultado por: name, size, modified, type
  // Usado en: list, inspect
  // --------------------------------------------------------------------------
  OptionMetaData sort;
  sort.normalized_name = "--sort";
  sort.alias_name = "";
  sort.conflict_name = {};
  sort.data_type = TypeDataReceived::EXTENSION;  // Recibe: name/size/modified/type
  // sort.handler = FilteringProcess que ordena entries
  GeneralOptionLog(sort);
  
  // --------------------------------------------------------------------------
  // --reverse
  // Invierte el orden del sort
  // Usado en: list, inspect
  // --------------------------------------------------------------------------
  OptionMetaData reverse;
  reverse.normalized_name = "--reverse";
  reverse.alias_name = "";
  reverse.conflict_name = {};
  reverse.data_type = TypeDataReceived::NONE;
  // reverse.handler = FilteringProcess que invierte vector
  GeneralOptionLog(reverse);
  
  // --------------------------------------------------------------------------
  // --dirs-first
  // Muestra directorios antes que archivos
  // Usado en: list
  // --------------------------------------------------------------------------
  OptionMetaData dirs_first;
  dirs_first.normalized_name = "--dirs-first";
  dirs_first.alias_name = "";
  dirs_first.conflict_name = {};
  dirs_first.data_type = TypeDataReceived::NONE;
  // dirs_first.handler = FilteringProcess que particiona por tipo
  GeneralOptionLog(dirs_first);
  
  // --------------------------------------------------------------------------
  // --filter
  // Filtra por patrón glob: *.cpp, test_*
  // Usado en: list
  // --------------------------------------------------------------------------
  OptionMetaData filter;
  filter.normalized_name = "--filter";
  filter.alias_name = "";
  filter.conflict_name = {};
  filter.data_type = TypeDataReceived::EXTENSION;  // Recibe glob pattern
  // filter.handler = FilteringProcess que aplica glob matching
  GeneralOptionLog(filter);
  
  // --------------------------------------------------------------------------
  // --no-header
  // Omite la cabecera de la tabla
  // Usado en: list
  // --------------------------------------------------------------------------
  OptionMetaData no_header;
  no_header.normalized_name = "--no-header";
  no_header.alias_name = "";
  no_header.conflict_name = {};
  no_header.data_type = TypeDataReceived::NONE;
  // no_header.handler = std::monostate{};  // Afecta formato de output
  GeneralOptionLog(no_header);
  
  // --------------------------------------------------------------------------
  // --follow-symlinks
  // Sigue enlaces simbólicos
  // Usado en: inspect
  // --------------------------------------------------------------------------
  OptionMetaData follow_symlinks;
  follow_symlinks.normalized_name = "--follow-symlinks";
  follow_symlinks.alias_name = "";
  follow_symlinks.conflict_name = {};
  follow_symlinks.data_type = TypeDataReceived::NONE;
  // follow_symlinks.handler = FilteringProcess que resuelve symlinks
  GeneralOptionLog(follow_symlinks);
  
  // --------------------------------------------------------------------------
  // --fields
  // Selecciona campos a mostrar: name,size,modified,permissions,type,inode
  // Usado en: inspect
  // --------------------------------------------------------------------------
  OptionMetaData fields;
  fields.normalized_name = "--fields";
  fields.alias_name = "";
  fields.conflict_name = {};
  fields.data_type = TypeDataReceived::EXTENSION;  // Recibe lista CSV
  // fields.handler = std::monostate{};  // Afecta formato de output
  GeneralOptionLog(fields);
  
  // ============================================================================
  // FLAGS DE MANIPULACIÓN (copy, move, delete)
  // ============================================================================
  
  // --------------------------------------------------------------------------
  // --force, -f
  // Sobreescribe destino sin preguntar / Sin confirmación interactiva
  // Usado en: copy, move, delete
  // --------------------------------------------------------------------------
  OptionMetaData force;
  force.normalized_name = "--force";
  force.alias_name = "-f";
  force.conflict_name = {
    "--no-overwrite",   // Mutuamente excluyente
    "--skip-existing"   // Mutuamente excluyente
  };
  force.data_type = TypeDataReceived::NONE;
  // force.handler = std::monostate{};  // Afecta lógica de confirmación
  GeneralOptionLog(force);
  
  // --------------------------------------------------------------------------
  // --no-overwrite
  // Falla si el destino existe
  // Usado en: copy, move
  // --------------------------------------------------------------------------
  OptionMetaData no_overwrite;
  no_overwrite.normalized_name = "--no-overwrite";
  no_overwrite.alias_name = "";
  no_overwrite.conflict_name = {
    "--force",          // Mutuamente excluyente
    "--skip-existing"   // Mutuamente excluyente
  };
  no_overwrite.data_type = TypeDataReceived::NONE;
  // no_overwrite.handler = std::monostate{};
  GeneralOptionLog(no_overwrite);
  
  // --------------------------------------------------------------------------
  // --skip-existing
  // Omite archivos que ya existen en destino
  // Usado en: copy
  // --------------------------------------------------------------------------
  OptionMetaData skip_existing;
  skip_existing.normalized_name = "--skip-existing";
  skip_existing.alias_name = "";
  skip_existing.conflict_name = {
    "--force",         // Mutuamente excluyente
    "--no-overwrite"   // Mutuamente excluyente
  };
  skip_existing.data_type = TypeDataReceived::NONE;
  // skip_existing.handler = std::monostate{};
  GeneralOptionLog(skip_existing);
  
  // --------------------------------------------------------------------------
  // --preserve, -p
  // Preserva metadatos: fechas, permisos
  // Usado en: copy
  // --------------------------------------------------------------------------
  OptionMetaData preserve;
  preserve.normalized_name = "--preserve";
  preserve.alias_name = "-p";
  preserve.conflict_name = {};
  preserve.data_type = TypeDataReceived::NONE;
  // preserve.handler = std::monostate{};  // Afecta operación de copia
  GeneralOptionLog(preserve);
  
  // --------------------------------------------------------------------------
  // --match
  // Elimina solo entradas que coincidan con el patrón
  // Usado en: delete
  // --------------------------------------------------------------------------
  OptionMetaData match;
  match.normalized_name = "--match";
  match.alias_name = "";
  match.conflict_name = {};
  match.data_type = TypeDataReceived::EXTENSION;  // Recibe glob pattern
  // match.handler = FilteringProcess que aplica glob matching
  GeneralOptionLog(match);
  
  // ============================================================================
  // FLAGS DE BÚSQUEDA (find)
  // ============================================================================
  
  // --------------------------------------------------------------------------
  // --name
  // Busca por nombre (glob): test_*.cpp, *config*
  // Usado en: find
  // --------------------------------------------------------------------------
  OptionMetaData name;
  name.normalized_name = "--name";
  name.alias_name = "";
  name.conflict_name = {};
  name.data_type = TypeDataReceived::EXTENSION;  // Recibe glob pattern
  // name.handler = FilteringProcess que filtra por nombre
  GeneralOptionLog(name);
  
  // --------------------------------------------------------------------------
  // --ext
  // Busca por extensión: cpp, .cpp (ambos válidos)
  // Usado en: find
  // --------------------------------------------------------------------------
  OptionMetaData ext;
  ext.normalized_name = "--ext";
  ext.alias_name = "";
  ext.conflict_name = {};
  ext.data_type = TypeDataReceived::EXTENSION;  // Recibe extensión
  // ext.handler = FilteringProcess que filtra por extensión
  GeneralOptionLog(ext);
  
  // --------------------------------------------------------------------------
  // --type
  // Filtra por tipo: file, dir, symlink
  // Usado en: find
  // --------------------------------------------------------------------------
  OptionMetaData type;
  type.normalized_name = "--type";
  type.alias_name = "";
  type.conflict_name = {};
  type.data_type = TypeDataReceived::EXTENSION;  // Recibe: file/dir/symlink
  // type.handler = FilteringProcess que filtra por tipo
  GeneralOptionLog(type);
  
  // --------------------------------------------------------------------------
  // --size-gt
  // Archivos mayores a n bytes/KB/MB/GB: 10MB, 5KB
  // Usado en: find
  // --------------------------------------------------------------------------
  OptionMetaData size_gt;
  size_gt.normalized_name = "--size-gt";
  size_gt.alias_name = "";
  size_gt.conflict_name = {};
  size_gt.data_type = TypeDataReceived::SIZE;  // Recibe tamaño con unidad
  // size_gt.handler = FilteringProcess que filtra por tamaño mínimo
  GeneralOptionLog(size_gt);
  
  // --------------------------------------------------------------------------
  // --size-lt
  // Archivos menores a n bytes/KB/MB/GB
  // Usado en: find
  // --------------------------------------------------------------------------
  OptionMetaData size_lt;
  size_lt.normalized_name = "--size-lt";
  size_lt.alias_name = "";
  size_lt.conflict_name = {};
  size_lt.data_type = TypeDataReceived::SIZE;  // Recibe tamaño con unidad
  // size_lt.handler = FilteringProcess que filtra por tamaño máximo
  GeneralOptionLog(size_lt);
  
  // --------------------------------------------------------------------------
  // --modified-after
  // Archivos modificados después de fecha ISO: 2024-01-01
  // Usado en: find
  // --------------------------------------------------------------------------
  OptionMetaData modified_after;
  modified_after.normalized_name = "--modified-after";
  modified_after.alias_name = "";
  modified_after.conflict_name = {};
  modified_after.data_type = TypeDataReceived::DATE;  // Recibe fecha ISO
  // modified_after.handler = FilteringProcess que filtra por fecha mínima
  GeneralOptionLog(modified_after);
  
  // --------------------------------------------------------------------------
  // --modified-before
  // Archivos modificados antes de fecha ISO: 2024-01-01
  // Usado en: find
  // --------------------------------------------------------------------------
  OptionMetaData modified_before;
  modified_before.normalized_name = "--modified-before";
  modified_before.alias_name = "";
  modified_before.conflict_name = {};
  modified_before.data_type = TypeDataReceived::DATE;  // Recibe fecha ISO
  // modified_before.handler = FilteringProcess que filtra por fecha máxima
  GeneralOptionLog(modified_before);
  
  // --------------------------------------------------------------------------
  // --no-hidden
  // Excluye archivos ocultos (opuesto a --all)
  // Usado en: find
  // --------------------------------------------------------------------------
  OptionMetaData no_hidden;
  no_hidden.normalized_name = "--no-hidden";
  no_hidden.alias_name = "";
  no_hidden.conflict_name = {
    "--all"  // Semánticamente opuesto
  };
  no_hidden.data_type = TypeDataReceived::NONE;
  // no_hidden.handler = FilteringProcess que excluye archivos con '.'
  GeneralOptionLog(no_hidden);
  
  // ============================================================================
  // RESUMEN DE OPCIONES REGISTRADAS
  // ============================================================================
  /*
   * Total de opciones en kron-basic: 31 flags
   * 
   * CATEGORÍAS:
   * - Globales: 7 (help, version, no-color, quiet, verbose, dry-run, output)
   * - Visualización: 8 (all, long, recursive, depth, sort, reverse, dirs-first, no-header)
   * - Filtrado: 3 (filter, follow-symlinks, fields)
   * - Manipulación: 4 (force, no-overwrite, skip-existing, preserve)
   * - Búsqueda básica: 2 (name, match)
   * - Búsqueda avanzada: 7 (ext, type, size-gt, size-lt, modified-after, modified-before, no-hidden)
   * 
   * TIPOS DE DATOS:
   * - NONE (bool): 21 flags
   * - EXTENSION (string/pattern): 8 flags (output, filter, fields, match, name, ext, type, depth)
   * - SIZE: 2 flags (size-gt, size-lt)
   * - DATE: 2 flags (modified-after, modified-before)
   * 
   * CONFLICTOS A VALIDAR:
   * - quiet ⊗ verbose
   * - all ⊗ no-hidden
   * - force ⊗ no-overwrite ⊗ skip-existing
   * 
   * DEPENDENCIAS:
   * - depth requiere recursive (o da warning)
   */
}
