#pragma once
#include "WorldGenerator.h"
#include "Chunk.h"

#include <vector>

#include "Math/GenericOctree.h"

namespace World
{

class MapManager
{
public:

  virtual ~MapManager() {}

  Chunk* GetChunk(const Chunk::Coords& coords);
  

private:
  std::vector<Chunk*> _map;
  
};

}
