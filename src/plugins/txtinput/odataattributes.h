#ifndef ODATA_ATTRIBUTES_H
#define ODATA_ATTRIBUTES_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QVector>
#include <QSet>

// key = field index, value = field value
typedef QMap<int, QVariant> ODataAttributeMap;

// key = field index, value = field name
typedef QMap<int, QString> ODataFieldNameMap;

class ODataAttributeMeta {
public:
    QString fieldname;
    QString fieldtype;
    int fieldlength;
    ODataAttributeMeta();
    ODataAttributeMeta(QString name, QString type, int length = 20) {
        fieldname = name;
        fieldtype = type;
        fieldlength = length;
    };
};

class ODataAttribute {
public:
    QString type;
    QVariant value;

    inline bool operator==( const ODataAttribute &v ) const {
        bool t = type.compare(v.type) == 0;
        bool va = value == v.value;
        return t & va;
    }
};

class ODataAttributes : public QVector<ODataAttribute> {
public:
    ODataAttributes() = default;

    ODataAttributes( int size ): QVector<ODataAttribute>( size ) {}

    ODataAttributes( int size, const ODataAttribute &v )
      : QVector<ODataAttribute>( size, v )
    {}

    ODataAttributes( const QVector<ODataAttribute> &v )
      : QVector<ODataAttribute>( v )
    {}

    inline bool operator!=( const ODataAttributes &v ) const { return !( *this == v ); }
};

#endif // ODATA_ATTRIBUTES_H
