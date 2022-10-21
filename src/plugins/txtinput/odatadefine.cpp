#include "odatadefine.h"

#include <QRegularExpression>
#include <QTextCodec>

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
       count = str.toUInt();
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
       count = str.toUInt();
    }
    return count;
}

