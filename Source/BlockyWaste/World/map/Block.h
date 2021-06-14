#pragma once
#include "../types/numeric.h"
#include "../types/length.h"

namespace World
{
using namespace Units;

class Block final
{
public:
  //! Block size
  static constexpr Length BLOCK_SIZE = 50_cm;

  uint32 id = 0;
  bool hasMetadata = false;

};

}

