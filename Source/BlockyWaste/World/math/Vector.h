#include <array>
#include <functional>
#include <type_traits>

namespace World
{

//namespace VectorInternal
//{
//
//template <typename T, std::size_t>
//using always_type = T;
//
//template <typename NumType, typename Seq> class VectorImpl;
//
//template <typename NumType, std::size_t ... Is>
//class VectorImpl<NumType, std::index_sequence<Is...>>
//{
//public:
//  using CoordType = NumType;
//  
//
//  static constexpr const NumType ZeroCoordValue = {};
//  static constexpr const size_t CoordCount = sizeof...(Is);
//
//  using MySequence = std::make_index_sequence<CoordCount>;
//  //VectorImpl(const VectorImpl&) = default;
//  //VectorImpl(VectorImpl&&) = default;
//  //VectorImpl& operator=(const VectorImpl&) = default;
//  //VectorImpl& operator=(VectorImpl&&) = default;
//
//  constexpr VectorImpl(always_type<NumType, Is>... vals) : values{ std::forward<NumType>(vals)... }
//  {}
//
//  VectorImpl(const std::array<NumType, CoordCount>& values) : values(values) {}
//  VectorImpl(std::array<NumType, CoordCount>&& values) : values(std::move(values)) {}
//
//  const NumType& operator[](size_t offset) const { return values[offset]; }
//  NumType& operator[](size_t offset) { return values[offset]; }
//
//  bool operator==(const VectorImpl& other) const;
//  VectorImpl operator*(NumType multiplier) const;
//  VectorImpl operator+(NumType addition) const;
//  VectorImpl operator+(const VectorImpl& other) const;
//  VectorImpl operator-(const VectorImpl& other) const;
//  VectorImpl& operator=(const VectorImpl& other) { values = other.values; }
//  VectorImpl& operator=(VectorImpl&& other) { values = std::move(other.values); }
//
//  unsigned int GetTypeHash() const;
//  //! Converts all values to new given type
//  template <typename NewType>
//  constexpr VectorImpl<NewType, MySequence> Convert() const { return Convert<NewType>(std::make_index_sequence<CoordCount>{}); }
//  //! Converts all values via the conversion function
//  template <typename NewType>
//  VectorImpl<NewType, MySequence> Convert(const std::function<NewType(NumType)>& callback) const { return Convert<NewType>(std::make_index_sequence<CoordCount>{}, callback); }
//
//  std::array<NumType, CoordCount> values;
//
//private:
//  //! Converts all values to new given type
//  template <typename NewType, std::size_t ... Isv>
//  constexpr VectorImpl<NewType, MySequence> Convert(std::index_sequence<Isv...>) const { return { { static_cast<NewType>(values[Isv])...} }; }
//  //! Converts all values via the conversion function
//  template <typename NewType, std::size_t ... Isv>
//  VectorImpl<NewType, MySequence> Convert(std::index_sequence<Isv...>, const std::function<NewType(NumType)>& callback) const { return { { callback(values[Isv])...} }; }
//};
//
//
//template <typename NumType, std::size_t ... Is>
//bool VectorImpl<NumType, Is>::operator==(const VectorImpl& other) const
//{
//  // This may look slow, but unless the compiler is really dumb, it will get unrolled
//  for (unsigned char i = 0; i < Size; ++i)
//  {
//    if (values[i] != other[i])
//    {
//      return false;
//    }
//  }
//  return true;
//}
//
//template <typename NumType, std::size_t ... Is>
//VectorImpl<NumType, Is> VectorImpl<NumType, Is>::operator*(NumType multiplier) const
//{
//  VectorImpl<NumType, Is> result{ ZeroCoordValue, ZeroCoordValue, ZeroCoordValue };
//
//  for (unsigned char i = 0; i < Size; ++i)
//  {
//    result[i] = values[i] * multiplier;
//  }
//  return result;
//}
//
//template <typename NumType, std::size_t ... Is>
//VectorImpl<NumType, Is> VectorImpl<NumType, Is>::operator+(NumType addition) const
//{
//  VectorImpl<NumType, Is> result{ ZeroCoordValue, ZeroCoordValue, ZeroCoordValue };
//
//  for (unsigned char i = 0; i < Size; ++i)
//  {
//    result[i] = values[i] + addition;
//  }
//  return result;
//}
//
//template <typename NumType, std::size_t ... Is>
//VectorImpl<NumType, Is> VectorImpl<NumType, Is>::operator+(const VectorImpl& other) const
//{
//  VectorImpl<NumType, Is> result{ ZeroCoordValue, ZeroCoordValue, ZeroCoordValue };
//
//  for (unsigned char i = 0; i < Size; ++i)
//  {
//    result[i] = values[i] + other[i];
//  }
//  return result;
//}
//
//template <typename NumType, std::size_t ... Is>
//VectorImpl<NumType, Is> VectorImpl<NumType, Is>::operator-(const VectorImpl& other) const
//{
//  VectorImpl<NumType, Is> result{ ZeroCoordValue, ZeroCoordValue, ZeroCoordValue };
//
//  for (unsigned char i = 0; i < Size; ++i)
//  {
//    result[i] = values[i] - other[i];
//  }
//  return result;
//}
//
//template <typename NumType, std::size_t ... Is>
//unsigned int VectorImpl<NumType, Is>::GetTypeHash() const
//{
//  static constexpr unsigned int fnv1a_base_32 = 0x811c9dc5;
//  static constexpr unsigned int fnv1a_mult_32 = 0x01000193;
//
//
//  unsigned char* data = (unsigned char*)&values[0];
//
//  unsigned int result = fnv1a_base_32;
//  for (unsigned char i = 0; i < Size * sizeof(NumType); ++i)
//  {
//    result = result ^ data[i];
//    result *= fnv1a_mult_32;
//  }
//  return result;
//}
//
//}
//
//template <typename T, std::size_t N>
//using Vector = VectorInternal::VectorImpl<T, std::make_index_sequence<N>>;



template <typename NumType, unsigned char Size>
class Vector
{
public:
  using CoordType = NumType;

  static constexpr NumType ZeroCoordValue = {};

  //! For use as a comparator in maps and the like
  struct SizeComparator
  {
    bool operator()(const Vector& a, const Vector& b) const
    {
      //return a.length() < b.length();
      return a.SizeSquared() < b.SizeSquared();
    }
  };

  //! For use as a comparator in maps and the like
  struct NaiveCoordComparator
  {
    bool operator()(const Vector& a, const Vector& b) const
    {
      for (unsigned char i = 0; i < Size; ++i)
      {
        if (a[i] < b[i])
          return true;
      }
      return false;
    }
  };

  //! For hashing in unordered map
  struct KeyHasher
  {
    std::size_t operator()(const Vector& k) const
    {
      return k.GetTypeHash();
    }
  };

  Vector(const Vector& v) : values(v.values) {}
  Vector(Vector&& v) : values(std::move(v.values)) {}

  template<typename... NumTypes,
    std::enable_if_t<sizeof...(NumTypes) == Size
    && std::conjunction_v<std::is_same<NumType, NumTypes>...>,
    int> = 0>
  constexpr Vector(NumTypes&&... vals) : values{ std::forward<NumTypes>(vals)... }
  {
    static_assert(sizeof...(NumTypes) == Size, "You must provide N arguments.");
  }

  Vector(const std::array<NumType, Size>& values) : values(values) {}
  Vector(std::array<NumType, Size>&& values) : values(std::move(values)) {}

  const NumType& operator[](size_t offset) const { return values[offset]; }
  NumType& operator[](size_t offset) { return values[offset]; }
  bool operator==(const Vector& other) const;
  Vector operator*(NumType multiplier) const;
  Vector operator+(NumType addition) const;
  Vector operator-(NumType subst) const;
  Vector operator+(const Vector& other) const;
  Vector operator-(const Vector& other) const;
  Vector& operator=(const Vector& other) { values = other.values; return *this; }
  Vector& operator=(Vector&& other) { values = std::move(other.values); return *this; }
  bool operator<(const Vector& other) { return SizeSquared() < other.SizeSquared(); }
  bool operator>(const Vector& other) { return SizeSquared() > other.SizeSquared(); }

  unsigned int GetTypeHash() const;
  NumType SizeSquared() const { NumType size = 0; for (unsigned char i = 0; i < Size; ++i) size += values[i] * values[i]; return size; }
  NumType DistanceTo(const Vector& other) const { return (other - *this).SizeSquared(); }

  //! Converts all values to new given type
  template <typename NewType>
  constexpr Vector<NewType, Size> Convert() const { return Convert<NewType>(std::make_index_sequence<Size>{}); }
  template <typename NewType>
  using NumericConversionFunc = std::function<NewType(NumType)>;
  //! Converts all values via the conversion function
  template <typename NewType>
  Vector<NewType, Size> Convert(const NumericConversionFunc<NewType>& callback) const { return Convert<NewType>(std::make_index_sequence<Size>{}, callback); }

  std::array<NumType, Size> values;

private:
  //! Converts all values to new given type
  template <typename NewType, std::size_t ... Is>
  constexpr Vector<NewType, Size> Convert(std::index_sequence<Is...>) const { return { { static_cast<NewType>(values[Is])...} }; }
  //! Converts all values via the conversion function
  template <typename NewType, std::size_t ... Is>
  Vector<NewType, Size> Convert(std::index_sequence<Is...>, const std::function<NewType(NumType)>& callback) const { return { { callback(values[Is])...} }; }
};

template <typename NumType, unsigned char Size>
bool Vector<NumType, Size>::operator==(const Vector<NumType, Size>& other) const
{
  // This may look slow, but unless the compiler is really dumb, it will get unrolled
  for (unsigned char i = 0; i < Size; ++i)
  {
    if (values[i] != other[i])
    {
      return false;
    }
  }
  return true;
}

template <typename NumType, unsigned char Size>
Vector<NumType, Size> Vector<NumType, Size>::operator*(NumType multiplier) const
{
  Vector<NumType, Size> result{ { ZeroCoordValue, ZeroCoordValue, ZeroCoordValue } };

  for (unsigned char i = 0; i < Size; ++i)
  {
    result[i] = values[i] * multiplier;
  }
  return result;
}

template <typename NumType, unsigned char Size>
Vector<NumType, Size> Vector<NumType, Size>::operator+(NumType addition) const
{
  Vector<NumType, Size> result{ { ZeroCoordValue, ZeroCoordValue, ZeroCoordValue } };

  for (unsigned char i = 0; i < Size; ++i)
  {
    result[i] = values[i] + addition;
  }
  return result;
}

template <typename NumType, unsigned char Size>
Vector<NumType, Size> Vector<NumType, Size>::operator-(NumType subst) const
{
  Vector<NumType, Size> result{ { ZeroCoordValue, ZeroCoordValue, ZeroCoordValue } };

  for (unsigned char i = 0; i < Size; ++i)
  {
    result[i] = values[i] - subst;
  }
  return result;
}

template <typename NumType, unsigned char Size>
Vector<NumType, Size> Vector<NumType, Size>::operator+(const Vector& other) const
{
  Vector<NumType, Size> result{ { ZeroCoordValue, ZeroCoordValue, ZeroCoordValue } };

  for (unsigned char i = 0; i < Size; ++i)
  {
    result[i] = values[i] + other[i];
  }
  return result;
}

template <typename NumType, unsigned char Size>
Vector<NumType, Size> Vector<NumType, Size>::operator-(const Vector& other) const
{
  Vector<NumType, Size> result{ { ZeroCoordValue, ZeroCoordValue, ZeroCoordValue } };

  for (unsigned char i = 0; i < Size; ++i)
  {
    result[i] = values[i] - other[i];
  }
  return result;
}

template <typename NumType, unsigned char Size>
unsigned int Vector<NumType, Size>::GetTypeHash() const
{
  static constexpr unsigned int fnv1a_base_32 = 0x811c9dc5;
  static constexpr unsigned int fnv1a_mult_32 = 0x01000193;


  unsigned char* data = (unsigned char*)&values[0];
  
  unsigned int result = fnv1a_base_32;
  for (unsigned char i = 0; i < Size*sizeof(NumType); ++i)
  {
    result = result ^ data[i];
    result *= fnv1a_mult_32;
  }
  return result;
}

}
//namespace std
//{
//template <>
//struct less
//{
//  template <typename NumType, unsigned char Size>
//  constexpr bool operator()(const Vector<NumType, Size>& _Left, const Vector<NumType, Size>& _Right) const
//  {
//    return true;
//  }
//};
//}
