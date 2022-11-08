#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include <QString>

/**
 * 椭球体Elipsoid
 */
class Ellipsoid {
public:
  QString name;
  QString shortName;

  /**
   * 赤道半径 长半轴a
   */
  double equatorRadius = 1.0;

  /**
   * 南/北极半径 短半轴b
   */
  double poleRadius = 1.0;

  /**
   * 极曲率半径 c = a^2 / b
   */
  double poleCurvatureRadis = 1.0;

  /**
   * 扁率倒数 1/ ((a - b) / a)
   */
  double reciprocalFlattening = 1.0;

  /**
   * 扁率 (a - b) / a
   */
  double flattening = 1.0;

  /**
   * 第一偏心率 sqart(a^2 - b^2) / a
   */
  double eccentricity = 1.0;

  /**
   * 第一偏心率平方 (a^2 - b^2) / a^2
   */
  double eccentricity2 = 1.0;

  /**
   * 第二偏心率 sqart(a^2 - b^2) / b
   */
   double secondeccentricity = 0;

  /**
   * 第二偏心率平方 (a^2 - b^2) / b^2
   */
   double secondeccentricity2 = 0;

  Ellipsoid();

  Ellipsoid(
    QString shortName,
    double equatorRadius,
    double poleRadius,
    double reciprocalFlattening,
    QString name
  );

   void initFlattening();
   void initPoleCurvatureRadis();

  /**
   * 初始化第一偏心量
   */
  void initEccentricity();

  /**
   * 初始化第二偏心量
   */
  void initSecondEccentricity();


  /**
   * 第一辅助函数
   * @param lat 纬度，弧度制
   */
  double firstAuxiliary(double lat, bool isRadian = true);

  /**
   * 第二辅助函数
   * @param lat 纬度，弧度制
   */
  double secondAuxiliary(double lat, bool isRadian = true);

  /**
   * 指定纬度到赤道的子午线弧长
   * @param lat 纬度
   * @param presicion 迭代项数
   * @link https://blog.csdn.net/Hanford/article/details/54631522
   */
  double meridianArcLength(
    double lat,
    bool isRadian = true,
    int presicion = 5
  );

  /**
   * 指定子午线弧长反算该纬度到赤道的弧度差值,待完善
   * @param length 弧长
   * @param isRadian 返回结果是否弧度表示
   * @param presicion 迭代项数
   */
  double inverseMeridianArcLengthDefault(
    double length,
    bool isRadian = true,
    int presicion = 10
  );

  /**
   * 指定子午线弧长反算该纬度到赤道的弧度差值,待完善
   * @param length 弧长
   * @param isRadian 返回结果是否弧度表示
   * @param presicion 迭代项数
   */
  double inverseMeridianArcLength(
    double length,
    bool isRadian = true,
    int presicion = 5
  );

  static Ellipsoid BEIJING54;
  static Ellipsoid XIAN80;
  static Ellipsoid CGCS2000;
};

#endif // ELLIPSOID_H
