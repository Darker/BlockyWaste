#pragma once
#include "../types/numeric.h"
#include "Block.h"

#include <array>

constexpr byte CHUNK_SIZE = 32;

class Chunk
{
public:

private:
  std::array<Block, CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE> _blocks;
};