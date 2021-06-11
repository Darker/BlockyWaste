#pragma once
#include "../types/numeric.h"



class Block final
{
public:
  //! Block size in milimetres
  static constexpr uint32 BLOCK_SIZE = 500;

  uint32 id = 0;
  bool hasMetadata = false;

};