#include "Chunk.h"

namespace World
{

const Block& Chunk::GetBlock(const Vector3LL& blockCoords) const
{
  const Vector3Int chunkBlockCoords = blockCoords.Convert<int>([](Vector3LL::CoordType v) { return static_cast<int>(v % CHUNK_SIZE); });
  const size_t offset = chunkBlockCoords.CalculateArrayOffset<CHUNK_SIZE>();
  return _blocks[offset];
}

Block& Chunk::GetBlock(const Vector3LL& blockCoords)
{
  return const_cast<Block&>(const_cast<const Chunk*>(this)->GetBlock(blockCoords));
}

}
