#pragma once
#include "WorldGenerator.h"
#include "Chunk.h"

#include <unordered_map>
#include <memory>

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
  //! Returns height in metres
  int GetWorldHeight(Vector<float, 2> coordinatesXY) const;

private:
  using ChunkContainer = std::unordered_map<Chunk::Coords, std::unique_ptr<Chunk>, Chunk::Coords::KeyHasher>;
  ChunkContainer _map;
  WorldGenerator _generator;
};

}
