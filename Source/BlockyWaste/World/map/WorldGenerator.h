#pragma once
#include "../types/numeric.h"
#include "../types/length.h"
#include "Chunk.h"

#include "../../extern/SimplexNoise/SimplexNoise.h"

namespace World
{


class WorldGenerator
{
public:
  WorldGenerator(uint64 seed = 1) : _seed(seed) {}

  virtual ~WorldGenerator() {}

  virtual Chunk* CreateChunk(Chunk::Coords coords);

  virtual Length GetWorldHeight(Vector3LL worldBlockCoords) const;

private:
  uint64 _seed;
  SimplexNoise _simplex = SimplexNoise(0.1f / 400.f, 0.5f, 1.99f, 0.5f);
};

}