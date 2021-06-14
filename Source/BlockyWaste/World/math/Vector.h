#include <array>
#include <functional>

namespace World
{

template <typename NumType, unsigned char Size>
class Vector
{
public:
  using CoordType = NumType;


  template<typename... NumTypes>
  constexpr Vector(NumTypes&&... vals) : values{ std::forward<NumTypes>(vals)... }
  {
    static_assert(sizeof...(NumTypes) == Size, "You must provide N arguments.");
  }

  Vector(const std::array<NumType, Size>& values) : values(values) {}
  Vector(std::array<NumType, Size>&& values) : values(std::move(values)) {}

  const NumType& operator[](size_t offset) const { return values[offset]; }
  NumType& operator[](size_t offset) { return values[offset]; }
  bool operator==(const Vector& other) const;

  //! Converts all values to new given type
  template <typename NewType>
  constexpr Vector<NewType, Size> Convert() const { return Convert<NewType>(std::make_index_sequence<Size>{}); }
  //! Converts all values via the conversion function
  template <typename NewType>
  Vector<NewType, Size> Convert(const std::function<NewType(NumType)>& callback) const { return Convert<NewType>(std::make_index_sequence<Size>{}, callback); }

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

}