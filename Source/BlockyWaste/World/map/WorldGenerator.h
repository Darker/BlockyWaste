#pragma once
#include "../types/numeric.h"

namespace World
{


class WorldGenerator
{
public:
  WorldGenerator(uint64 seed = 1) : _seed(seed) {}

  virtual ~WorldGenerator() {}
private:
  uint64 _seed;
};

}