#include <cstdint>
enum class TypeToken : std::uint8_t{
  COMMAND,
  OPTION_NOT_NORMALIZED,
  OPTION_NORMALIZED,

  LITERAL,
  SEPARATION,
  POSITIONAL,
};
