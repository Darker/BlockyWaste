#pragma once
#include "../types/numeric.h"
#include "Block.h"
#include "../math/Vector3.h"

#include <array>

namespace World
{

constexpr byte CHUNK_SIZE = 32;

class Chunk
{
public:
  class Coords : public Vector3LL
  {
  public:
    using Vector3LL::Vector3LL;

    constexpr static Coords FromBlockCoords(const Vector3LL& coords) { return { coords[0] / CHUNK_SIZE, coords[1] / CHUNK_SIZE, coords[2] / CHUNK_SIZE }; }
  };

  const Block& GetBlock(const Vector3LL& blockCoords) const;
  Block& GetBlock(const Vector3LL& blockCoords);


private:
  std::array<Block, CHUNK_SIZE* CHUNK_SIZE* CHUNK_SIZE> _blocks;
  bool _empty = true;
  // lowest xyz corner
  Coords _position{ 0ll,0ll,0ll };
};

}