#include "MapManager.h"
#include <cmath>

namespace World
{



Chunk* MapManager::GetChunk(const Chunk::Coords& coords, LoadMode mode)
{

  ChunkContainer::iterator it;

  it = _map.find(coords);
  if (it != _map.end())
  {
    return it->second.get();
  }
  else
  {
    Chunk* newChunk = _generator.CreateChunk(coords);
    _map.insert(std::make_pair(coords, newChunk));
    return newChunk;
  }
}

int MapManager::GetWorldHeight(Vector<float, 2> coordinatesXY) const
{
  const Length baseHeight = _generator.GetWorldHeight({ {(long long) coordinatesXY[0], (long long)coordinatesXY[1]} });
  return std::ceil(baseHeight.GetMetres());
}

}