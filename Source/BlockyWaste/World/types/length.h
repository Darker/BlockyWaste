#pragma once
namespace World
{

class Length
{
public:
  using LengthNumType = unsigned long long;


  constexpr Length(LengthNumType micrometres) : length(micrometres) {}

  static constexpr Length FromMillimetres(LengthNumType mm) { return { mm * 1000 }; }
  static constexpr Length FromMetres(LengthNumType mm) { return { mm * 1000 * 1000 }; }
  static constexpr Length FromCentimetres(LengthNumType cm) { return { cm * 1000 * 10 }; }

private:
  //! Length in micrometres
  LengthNumType length;

};

namespace Units
{

constexpr Length operator"" _mm(Length::LengthNumType millimetres) { return Length::FromMillimetres(millimetres); }
constexpr Length operator"" _m(Length::LengthNumType metres) { return Length::FromMetres(metres); }
constexpr Length operator"" _cm(Length::LengthNumType centimetres) { return Length::FromCentimetres(centimetres); }

}

}
