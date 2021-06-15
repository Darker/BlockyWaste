#pragma once
#include "WorldGenerator.h"
#include "Chunk.h"

#include <map>

#include "Math/GenericOctree.h"

namespace World
{

class MapManager
{
public:

  enum class LoadMode
  {
    //! Load function blocks and always returns value
    Load,
    //! Load function returns immediatelly, if data is not preload it returns empty result (for example nullptr);
    Preload
  };

  virtual ~MapManager() {}

  Chunk* GetChunk(const Chunk::Coords& coords, LoadMode mode = LoadMode::Load);
  

private:
  using ChunkContainer = std::map<Chunk::Coords, Chunk*, Chunk::Coords::NaiveCoordComparator>;
  ChunkContainer _map;
  WorldGenerator _generator;
};

}
