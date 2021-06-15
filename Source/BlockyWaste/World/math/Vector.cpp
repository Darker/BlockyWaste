//#include "Vector.h"
//
//#include <functional>
//#include <iostream>
//
//using namespace World;
//using Vector3D = Vector<double, 3>;
//using Vector3Int = Vector<int, 3>;
//
//class Vector3DExtra : public Vector3D
//{
//public:
//  using Vector3D::Vector3D;
//
//  Vector3DExtra(const Vector3D& v) : Vector3D(v) {}
//
//  bool IsZero() const { return values[0] == 0.0 && values[1] == 0.0 && values[2] == 0.0; }
//};
//
//#include <cmath>
//
//int main()
//{
//  const Vector3D src{ 1.4, 2.5, 3.6 };
//  const Vector3Int target = src.Convert((const std::function<int(double)>)[](double val) { return (int)std::round(val); });
//  const Vector<double, 3> mult = src * 2;
//
//  const Vector3DExtra extra(mult + 1.0);
//  const Vector<double, 3> extraBack = extra - 1.0;;
//
//  std::cout << mult[0] << ", " << mult[1]<<'\n';
//
//  std::cout << target[0] << ", " << target[1]<<", " << target[2];
//  return 0;
//}