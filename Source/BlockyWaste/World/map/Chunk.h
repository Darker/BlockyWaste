#pragma once
#include "../types/numeric.h"
#include "Block.h"
#include "../math/Vector3.h"
#include "BlockCoords.h"

#include <array>

namespace World
{

constexpr byte CHUNK_SIZE = 8;
constexpr size_t BLOCKS_PER_CHUNK = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
constexpr double CHUNK_SIDE_WORLD_WIDTH = CHUNK_SIZE * 100.0;
constexpr float CHUNK_SIDE_WORLD_WIDTH_F = CHUNK_SIZE * 100.0f;

class Chunk
{
public:
  class Coords : public Vector3LL
  {
  public:
    using Vector3LL::Vector3LL;

    Coords(const Vector3LL& v) : Vector3LL(v) {}
    Coords(Vector3LL&& v) : Vector3LL(v) {}
    Vector3F ToWorldCoords() const { return Convert<float>((NumericConversionFunc<float>)[](long long c) { return c * CHUNK_SIDE_WORLD_WIDTH_F; }); }
    //! Provides the coordinates of the midpoint of this chunk
    Vector3F ToWorldCenterPoint() const { return Convert<float>((NumericConversionFunc<float>)[](long long c) { return c * CHUNK_SIDE_WORLD_WIDTH_F + CHUNK_SIDE_WORLD_WIDTH_F/2.0f; }); }

    static Coords FromBlockCoords(const BlockCoords& coords) { return { coords[0] / CHUNK_SIZE, coords[1] / CHUNK_SIZE, coords[2] / CHUNK_SIZE }; }
    static Coords FromWorldCoords(const Vector3F& worldCoords) { return Coords((worldCoords * (1.0/(CHUNK_SIZE*100.0))).Convert<long long>()); }
  };

  const Block& GetBlock(const Vector3LL& blockCoords) const;
  Block& GetBlock(const Vector3LL& blockCoords);

  Block& operator[](size_t index) { return _blocks[index]; }
  const Block& operator[](size_t index) const { return _blocks[index]; }

  void MakeNonEmpty() { _empty = false; }
  void MakeEmpty() { _empty = true; }
  bool IsEmpty() const { return _empty; }
  void SetPosition(Coords pos) { _position = pos; }
  Coords GetPosition() const { return _position; }
  Vector3F GetWorldMidpoint() const { return _position.ToWorldCenterPoint(); }

private:
  std::array<Block, BLOCKS_PER_CHUNK> _blocks;
  bool _empty = true;
  // lowest xyz corner
  Coords _position{ 0ll,0ll,0ll };
};

}