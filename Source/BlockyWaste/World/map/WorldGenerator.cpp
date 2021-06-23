#include "WorldGenerator.h"
#include <cmath>

namespace World
{
using namespace Units;

Chunk* WorldGenerator::CreateChunk(Chunk::Coords coords)
{
  Chunk* newChunk = new Chunk;
  Chunk& chunkRef = *newChunk;

  chunkRef.SetPosition(Chunk::Coords(coords));

  const Vector3LL chunkZeroPos = coords * (Chunk::Coords::CoordType)CHUNK_SIZE;
  for (size_t i = 0; i < BLOCKS_PER_CHUNK; ++i)
  {
    const Vector3LL blockRelativePos = Vector3LL::FromArrayOffset<CHUNK_SIZE>(i);
    const Vector3LL blockAbsolutePos = blockRelativePos + chunkZeroPos;

    const Length height = GetWorldHeight(blockAbsolutePos);
    const int heightM = height.GetMetresInt();
    if (blockAbsolutePos.Z() <= heightM)
    {
      switch (heightM - blockAbsolutePos.Z())
      {
      case 0: chunkRef[i].id = 1; break;
      case 1:[[fallthrough]]
      case 2:[[fallthrough]]
      case 3: chunkRef[i].id = 2; break;
      default: chunkRef[i].id = 3;
      }
      //chunkRef[i].id = heightM - blockAbsolutePos.Z()>3 ? 3: 1;
      chunkRef.MakeNonEmpty();
    }
  }
  return newChunk;
}

Length WorldGenerator::GetWorldHeight(Vector3LL worldBlockCoords) const
{
  const Length ms100 = 100_m;
  float noise = (1.0f+_simplex.fractal(7, worldBlockCoords.X()* 2.0f, worldBlockCoords.Y() * 2.0f))/2.0f;
  noise = std::powf(noise, 2.85); //std::cos(std::sin(worldBlockCoords.X()/1017.0) + 0.5- std::sin(worldBlockCoords.Y() / 1028.0));
  return ms100 * noise;
}

}
