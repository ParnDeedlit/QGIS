#include "odatadefine.h"

#include <QRegularExpression>
#include <QTextCodec>

vector<QString> ProjTypeNames = {"等角圆锥", "等分弧秒投影", "高斯－克吕格投影"};
vector<QString> ProjUnitNames = {"米", "度", "分", "秒" };

QString gb2312ToUtf8(QString gb2312)
{
    QTextCodec* utf8Codec = QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUnicode = gb2312Codec ->toUnicode(gb2312.toLocal8Bit());    //无编码
    QByteArray ByteUTF8 = utf8Codec ->fromUnicode(strUnicode);     //无编码转换

    return QString::fromLocal8Bit(ByteUTF8);
}

QString utf8ToGb2312(QString utf8) {
    QTextCodec* utf8Codec = QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUnicode = utf8Codec ->toUnicode(utf8.toLocal8Bit());    //无编码
    QByteArray ByteUTF8 = gb2312Codec ->fromUnicode(strUnicode);     //无编码转换

    return QString::fromLocal8Bit(ByteUTF8);
}

ProjType projTypeNameToProjType(QString name) {
    ProjType type;
    int count = ProjTypeNames.size();
    int index = -1;
    for (int i = 0; i < count; i++) {
        if (name.compare(ProjTypeNames[i]) == 0) {
            index = i;
            break;
        }
    }
    switch(index) {
    case 0:
        type = ProjType::Equal_Angle_Cone;
        break;
    case 1:
        type = ProjType::Equal_Arc_Lonlat;
        break;
    case 2:
        type = ProjType::Gauss;
        break;
    }
    return type;
}

QString projTypeToProjTypeName(ProjType type) {
    int index = type;
    if (index < ProjTypeNames.size()) {
        return ProjTypeNames[index];
    }
    return "未匹配";
}

ProjUnit projUnitNameToProjUnit(QString name) {
    ProjUnit unit;
    int count = ProjUnitNames.size();
    int index = -1;
    for (int i = 0; i < count; i++) {
        if (name.compare(ProjUnitNames[i]) == 0) {
            index = i;
            break;
        }
    }
    switch(index) {
    case 0:
        unit = ProjUnit::Miter;
        break;
    case 1:
        unit = ProjUnit::Degree;
        break;
    case 2:
        unit = ProjUnit::Minite;
        break;
    case 3:
        unit = ProjUnit::Second;
        break;
    }

    printf(QString(name + " projUnitNameToProjUnit  " + QString::number(index) + "  \r\n").toStdString().c_str());
    return unit;
}

QString projUnitToProjUnitName(ProjUnit unit) {
    int index = unit;
    if (index < ProjUnitNames.size()) {
        return ProjUnitNames[index];
    }
    return "未匹配";
}

bool MetadataLineStringMatchReg(QString linestring, QString patch) {
    QString reg("\\s+" +patch);
    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    return match_a.hasMatch();
}

double MetadataLineStringToDoubleByReg(QString linestring, QString patch) {
    double number = 0;
    QString reg("\\s+" + patch + "\\s*(-?\\d+)(\\.\\d+)?");
    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
       QString a = match_a.captured(1);
       QString b = match_a.captured(2);
       QString result(a+b);
       number = result.toDouble();
    }
    return number;
}

int MetadataLineStringToIntByReg(QString linestring, QString patch) {
    int number = 0;
    QString reg("\\s+" + patch + "\\s*(-?\\d+)");
    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
       QString a = match_a.captured(1);
       QString result(a);
       number = result.toDouble();
    }
    return number;
}

QString MetadataLineStringToStringByReg(QString linestring, QString patch) {
    QString str;
    QString reg("\\s+" + patch + "\\s*([\\w\u4E00-\u9FA5A-Za-z0-9_]+)");
    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
       str = match_a.captured(1);
    }
    return str;
}

int AttributeLineStringTypeCount(QString linestring, LayerType type) {
    QString layertype;
    int count = -1;
    switch(type) {
        case LayerType::Point:
        layertype = "P";
        break;
    case LayerType::Line:
        layertype = "L";
        break;
    case LayerType::Area:
        layertype = "A";
        break;
    default:
        break;
    }

    QString str;
    QString reg("^" + layertype + "+\\s*(\\d+)\\s*$");
    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
       str = match_a.captured(1);
       count = str.toInt();
    }
    return count;
}

int GeometryLineStringTypeCount(QString linestring, LayerType type) {
    QString layertype;
    int count = -1;
    switch(type) {
        case LayerType::Point:
        layertype = "P";
        break;
    case LayerType::Line:
        layertype = "L";
        break;
    case LayerType::Area:
        layertype = "A";
        break;
    default:
        break;
    }

    QString str;
    QString reg("^" + layertype + "+\\s*(\\d+)\\s*$");
    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
       str = match_a.captured(1);
       count = str.toInt();
    }
    return count;
}

bool GeometryLineCoordCount(QString linestring, int &index, int &count) {
    QString indexstr;
    QString countstr;
    QString reg("\\s+(\\d+)\\s+(\\d+)\\s*");
    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
       indexstr = match_a.captured(1);
       countstr = match_a.captured(2);
       index = indexstr.toInt();
       count = countstr.toInt();
       return true;
    }
    return false;
}

bool GeometryAreaRingCount(QString linestring, int &index, int &count) {
    QString indexstr;
    QString countstr;
    QString doubleReg = "(NULL|-?[1-9]\\d*\\.\\d*|0\\.\\d*[1-9]\\d*|0?\\.0+|0)";
    QString reg("\\s+(\\d+)\\s+" + doubleReg + "\\s+" + doubleReg + "\\s+(\\d+)\\s*");
    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
       indexstr = match_a.captured(1);
       countstr = match_a.captured(4);
       index = indexstr.toInt();
       count = countstr.toInt();
       return true;
    }
    return false;
}

bool GeometryAreaCoordCount(QString linestring, int &count) {
    QString indexstr;
    QString countstr;
    QString reg("\\s+(\\d+)\\s+");
    QRegularExpression re_a(reg);
    QRegularExpressionMatch match_a = re_a.match(linestring);
    if(match_a.hasMatch()) {
       countstr = match_a.captured(1);
       count = countstr.toInt();
       return true;
    }
    return false;
}
