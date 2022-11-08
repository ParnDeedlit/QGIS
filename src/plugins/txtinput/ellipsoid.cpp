#include "ellipsoid.h"

#include <cmath>

Ellipsoid::Ellipsoid() {

}

Ellipsoid::Ellipsoid(
      QString shortName,
      double equatorRadius,
      double poleRadius,
      double reciprocalFlattening,
      QString name
){
    this->shortName = shortName;
    this->name = name;
    this->equatorRadius = equatorRadius;
    this->poleRadius = poleRadius;
    this->reciprocalFlattening = reciprocalFlattening;

    if (reciprocalFlattening != 0.0) {
      this->flattening = 1.0 / reciprocalFlattening;
      this->eccentricity2 = 2.0 * this->flattening - this->flattening * this->flattening;
      this->poleRadius = equatorRadius * sqrt(1.0 - this->eccentricity2);
    }

    initFlattening();
    initPoleCurvatureRadis();
    initEccentricity();
    initSecondEccentricity();
}

void Ellipsoid::initFlattening() {
    if (this->reciprocalFlattening != 0.0) {
      this->flattening = 1.0 / this->reciprocalFlattening;
    }
}

void Ellipsoid::initPoleCurvatureRadis() {
    this->poleCurvatureRadis =
      (this->equatorRadius * this->equatorRadius) / this->poleRadius;
}

  /**
   * 初始化第一偏心量
   */
void Ellipsoid::initEccentricity() {
    this->eccentricity2 =
      1.0 -
      (this->poleRadius * this->poleRadius) /
        (this->equatorRadius * this->equatorRadius);
    this->eccentricity = sqrt(this->eccentricity2);
}

/**
* 初始化第二偏心量
*/
void Ellipsoid::initSecondEccentricity() {
    this->secondeccentricity2 =
      ((this->equatorRadius * this->equatorRadius) /
        (this->poleRadius * this->poleRadius)) -
      1;
    this->secondeccentricity = sqrt(this->secondeccentricity2);
}


/**
* 第一辅助函数
* @param lat 纬度，弧度制
*/
double Ellipsoid::firstAuxiliary(double lat, bool isRadian) {
    if (!isRadian) {
      lat = (lat / 180) * M_PI;
    }
    double sin1 = sin(lat);
    double sin2 = sin1 * sin1;
    return sqrt(1 - this->eccentricity2 * sin2);
}

  /**
   * 第二辅助函数
   * @param lat 纬度，弧度制
   */
  double Ellipsoid::secondAuxiliary(double lat, bool isRadian ) {
    if (!isRadian) {
      lat = (lat / 180) * M_PI;
    }
    double cos1 = cos(lat);
    double cos2 = cos1 * cos1;
    return sqrt(1 + this->secondeccentricity2 * cos2);
  }

  /**
   * 指定纬度到赤道的子午线弧长
   * @param lat 纬度
   * @param presicion 迭代项数
   * @link https://blog.csdn.net/Hanford/article/details/54631522
   */
  double Ellipsoid::meridianArcLength(
          double lat,
          bool isRadian,
          int presicion
  ) {
    if (!isRadian) {
      lat = (lat / 180) * M_PI;
    }
    double cos1 = cos(lat);
    double sin1 = sin(lat);
    double sin2 = sin1 * sin1;
    double sincos = sin1 * cos1;
    double e = 1.0;
    double In = lat;
    double sum = lat;
    double k = 0.0;
    for (int n = 1; n <= presicion; n++) {
      k = 0.5 / n;
      e *= (1.0 + k) * eccentricity2;
      In -= k * (In + sincos);
      sum += e * In;
      sincos *= sin2;
    }
    return equatorRadius * (1 - eccentricity2) * sum;
  }

  /**
   * 指定子午线弧长反算该纬度到赤道的弧度差值,待完善
   * @param length 弧长
   * @param isRadian 返回结果是否弧度表示
   * @param presicion 迭代项数
   */
  double Ellipsoid::inverseMeridianArcLengthDefault(
          double length,
          bool isRadian,
          int presicion
  ) {
    double tolerence = pow(10, -presicion);
    double m0 = equatorRadius * (1 - eccentricity2);
    double m2 = (3 * eccentricity2 * m0) / 2;
    double m4 = (5 * eccentricity2 * m2) / 4;
    double m6 = (7 * eccentricity2 * m4) / 6;
    double m8 = (9 * eccentricity2 * m6) / 8;
    double a0 = m0 + m2 / 2 + (3 * m4) / 8 + (5 * m6) / 16 + (35 * m8) / 128;
    double a2 = m2 / 2 + m4 / 2 + (15 * m6) / 32 + (7 * m8) / 16;
    double a4 = m4 / 8 + (3 * m6) / 16 + (7 * m8) / 32;
    double a6 = m6 / 32 + m8 / 16;
    double a8 = m8 / 128;

    double result = 0;

    double b0 = length / a0;

    while (1) {
      double F =
        (-a2 * sin(2 * b0)) / 2 +
        (a4 * sin(4 * b0)) / 4 -
        (a6 * sin(6 * b0)) / 6 +
        (a8 * sin(8 * b0)) / 8;
      double B = (length - F) / a0;
      if (abs(b0 - B) < tolerence) {
        result = B;
        break;
      }
      b0 = B;
    }

    if (!isRadian) {
      result = (result * 180) / M_PI;
    }
    return result;
  }

  /**
   * 指定子午线弧长反算该纬度到赤道的弧度差值,待完善
   * @param length 弧长
   * @param isRadian 返回结果是否弧度表示
   * @param presicion 迭代项数
   */
  double Ellipsoid::inverseMeridianArcLength(
          double length,
          bool isRadian,
          int presicion
  ) {
    double B = 0;
    double a0 = equatorRadius * (1 - eccentricity2);

    double B0 = length / a0;

    for (int i = presicion; i != 0; i--) {
      double F = meridianArcLength(B0) - a0 * B0;
      B = (length - F) / a0;

      if (abs(B0 - B) < 1e-11) {
        break;
      }
      B0 = B;
    }
    if (!isRadian) {
      B = (B * 180) / M_PI;
    }
    return B;
  }


  static Ellipsoid BEIJING54 = Ellipsoid(
    "beijing54",
    6378245.0,
    0.0,
    298.3,
    "BeiJing 1954"
  );
  static Ellipsoid XIAN80 = Ellipsoid(
    "xian80",
    6378140,
    0.0,
    298.257,
    "XiAn 1980"
  );
  static Ellipsoid CGCS2000 = Ellipsoid(
    "china2000",
    6378137.0,
    0.0,
    298.257222101,
    "China 2000"
  );
