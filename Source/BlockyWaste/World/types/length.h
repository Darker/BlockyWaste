#pragma once
#include <cmath>

namespace World
{

class Length
{
public:
  using LengthNumType = unsigned long long;


  constexpr Length(LengthNumType micrometres) : length(micrometres) {}

  static constexpr Length FromMillimetres(LengthNumType mm) { return { mm * MillimetreCount }; }
  static constexpr Length FromMetres(LengthNumType m) { return { m * MetreCount }; }
  static constexpr Length FromCentimetres(LengthNumType cm) { return { cm * CentimetreCount }; }

  Length constexpr operator*(double value) const { return Length(std::round(length * value)); }
  Length constexpr operator*(float value) const { return Length(std::round(length * value)); }
  Length constexpr operator*(LengthNumType value) const { return Length(value*length); }
  Length constexpr operator+(LengthNumType value) const { return Length(value+length); }
  Length constexpr operator+(Length value) const { return Length(value.length + length); }

  constexpr double GetMetres() const { return length / (double)MetreCount; }
  constexpr int GetMetresInt() const { return length / MetreCount; }

private:
  static constexpr LengthNumType MillimetreCount = 1000;
  static constexpr LengthNumType CentimetreCount = MillimetreCount * 10;
  static constexpr LengthNumType MetreCount = MillimetreCount * 1000;

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
