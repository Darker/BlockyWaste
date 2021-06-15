#pragma once
#include "Vector.h"

#include <type_traits>

namespace World
{

template <typename NumType>
class Vector3 : public Vector<NumType, 3>
{
public:
  //! Inherit parent constructor
  using Vector<NumType, 3>::Vector;

  Vector3(const Vector<NumType, 3>& v) : Vector<NumType, 3>(v) {}
  Vector3(Vector<NumType, 3>&& v) : Vector<NumType, 3>(v) {}

  constexpr NumType X() const { return values[0]; }
  constexpr NumType Y() const { return values[1]; }
  constexpr NumType Z() const { return values[2]; }

  //! Calculate index of coordinates in a flat array that contains all coordinates within given range //, std::enable_if_t<std::is_integral<NumType>::value, bool> = true
  template <size_t xMax, size_t yMax = xMax, size_t zMax = yMax>
  constexpr size_t CalculateArrayOffset() const
  {
    return values[0] + values[1] * xMax + values[2] * yMax * xMax;
  }
  //! Calculate index of coordinates in a flat array that contains all coordinates within given range //, std::enable_if_t<std::is_integral<NumType>::value, bool> = true
  template <size_t xMax, size_t yMax = xMax, size_t zMax = yMax>
  static constexpr Vector3 FromArrayOffset(size_t offset)
  {
    size_t z = offset / (xMax * yMax);
    size_t noz = offset - (z * xMax * yMax);

    return { static_cast<NumType>(noz % xMax), static_cast<NumType>(noz / yMax), static_cast<NumType>(z) };
  }
};

using Vector3D = Vector3<double>;
using Vector3F = Vector3<float>;
using Vector3LL = Vector3<long long>;
using Vector3Int = Vector3<int>;
using Vector3SizeT = Vector3<size_t>;


}
