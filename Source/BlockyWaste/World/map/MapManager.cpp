#include "MapManager.h"

namespace World
{



Chunk* MapManager::GetChunk(const Chunk::Coords& coords, LoadMode mode)
{

  ChunkContainer::iterator it;

  it = _map.find(coords);
  if (it != _map.end())
  {
    return it->second;
  }
  else
  {
    Chunk* newChunk = _generator.CreateChunk(coords);
    _map.insert(std::make_pair(coords, newChunk));
    return newChunk;
  }
}

}