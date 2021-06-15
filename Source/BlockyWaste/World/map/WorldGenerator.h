#pragma once
#include "../types/numeric.h"
#include "../types/length.h"
#include "Chunk.h"

namespace World
{


class WorldGenerator
{
public:
  WorldGenerator(uint64 seed = 1) : _seed(seed) {}

  virtual ~WorldGenerator() {}

  virtual Chunk* CreateChunk(Chunk::Coords coords);

  virtual Length GetWorldHeight(Vector3LL worldBlockCoords);

private:
  uint64 _seed;
};

}