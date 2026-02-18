#include <cstdint>
enum class TypeToken : std::uint8_t{
  COMMAND,
  OPTION_SHORT,
  OPTION_LONG,
  OPTION_NORMALIZED,

  LITERAL,
  SEPARATION,
  POSITIONAL,
};
