#pragma once
#include "../math/Vector3.h"

namespace World
{

class BlockCoords : public Vector3LL
{
public:
  using Vector3LL::Vector3LL;
  BlockCoords(const Vector3LL& v) : Vector3LL(v) {}
  BlockCoords(Vector3LL&& v) : Vector3LL(v) {}

  Vector3F ToWorldCoords() const { return Convert<float>((NumericConversionFunc<float>)[](long long c) { return c * 100.0f; }); }

  static BlockCoords FromWorldCoords(const Vector3D& worldCoords) { return BlockCoords((worldCoords * (1.0 /100.0)).Convert<long long>()); }
};

}