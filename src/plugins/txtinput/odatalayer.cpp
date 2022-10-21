#include "odatalayer.h"
#include "odatafeature.h"

#include <stdio.h>

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QTextCodec>

using namespace std;

vector<ODataAttributeMeta> FIELDS_A = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("等级"), QString("int"), 8),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("理论横坐标"), QString("double"), 10),
    ODataAttributeMeta(QString("理论纵坐标"), QString("double"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_B = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("类别"), QString("string"), 10),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_C = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("居住月份"), QString("int"), 8),
    ODataAttributeMeta(QString("人口"), QString("int"), 8),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("行政区划代码"), QString("string"), 20),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_D = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("编号"), QString("string"), 20),
    ODataAttributeMeta(QString("等级"), QString("string"), 20),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("铺宽"), QString("double"), 10),
    ODataAttributeMeta(QString("桥长"), QString("double"), 10),
    ODataAttributeMeta(QString("净空高"), QString("double"), 10),
    ODataAttributeMeta(QString("载重吨数"), QString("double"), 10),
    ODataAttributeMeta(QString("里程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("通行月份"), QString("int"), 8),
    ODataAttributeMeta(QString("名称水深"), QString("double"), 10),
    ODataAttributeMeta(QString("底质"), QString("string"), 20),
    ODataAttributeMeta(QString("最小曲率半径"), QString("double"), 10),
    ODataAttributeMeta(QString("最大纵坡"), QString("double"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_E = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("净空高"), QString("double"), 10),
    ODataAttributeMeta(QString("埋藏深度"), QString("double"), 10),
    ODataAttributeMeta(QString("存在状态"), QString("string"), 10),
    ODataAttributeMeta(QString("作用方式"), QString("string"), 20),
    ODataAttributeMeta(QString("限制种类"), QString("string"), 40),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_F = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("水深"), QString("double"), 10),
    ODataAttributeMeta(QString("泥深"), QString("double"), 10),
    ODataAttributeMeta(QString("时令月份"), QString("int"), 8),
    ODataAttributeMeta(QString("长度"), QString("double"), 10),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("库容量"), QString("double"), 10),
    ODataAttributeMeta(QString("吨位"), QString("int"), 8),
    ODataAttributeMeta(QString("河流代码"), QString("string"), 20),
    ODataAttributeMeta(QString("通航性质"), QString("string"), 8),
    ODataAttributeMeta(QString("植被类型"), QString("string"), 30),
    ODataAttributeMeta(QString("表面物质"), QString("string"), 30),
    ODataAttributeMeta(QString("存在状态"), QString("string"), 10),
    ODataAttributeMeta(QString("位置质量"), QString("string"), 20),
    ODataAttributeMeta(QString("颜色"), QString("string"), 20),
    ODataAttributeMeta(QString("与水面关系"), QString("string"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_G = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("水深值"), QString("double"), 10),
    ODataAttributeMeta(QString("水深值1"), QString("double"), 10),
    ODataAttributeMeta(QString("水深值2"), QString("double"), 10),
    ODataAttributeMeta(QString("测深技术"), QString("string"), 40),
    ODataAttributeMeta(QString("测深质量"), QString("string"), 40),
    ODataAttributeMeta(QString("位置质量"), QString("string"), 20),
    ODataAttributeMeta(QString("作用方式"), QString("string"), 20),
    ODataAttributeMeta(QString("表面物质"), QString("string"), 30),
    ODataAttributeMeta(QString("危险级"), QString("string"), 30),
    ODataAttributeMeta(QString("危险级"), QString("string"), 20),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_H = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("水深值"), QString("double"), 10),
    ODataAttributeMeta(QString("高度"), QString("double"), 10),
    ODataAttributeMeta(QString("垂高"), QString("double"), 10),
    ODataAttributeMeta(QString("存在状态"), QString("string"), 10),
    ODataAttributeMeta(QString("测深技术"), QString("string"), 40),
    ODataAttributeMeta(QString("测深质量"), QString("string"), 40),
    ODataAttributeMeta(QString("位置质量"), QString("string"), 20),
    ODataAttributeMeta(QString("限制种类"), QString("string"), 40),
    ODataAttributeMeta(QString("作用方式"), QString("string"), 20),
    ODataAttributeMeta(QString("表面物质"), QString("string"), 30),
    ODataAttributeMeta(QString("与水面关系"), QString("string"), 10),
    ODataAttributeMeta(QString("危险级"), QString("string"), 20),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_I = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("水深"), QString("double"), 10),
    ODataAttributeMeta(QString("底质"), QString("string"), 20),
    ODataAttributeMeta(QString("流速"), QString("double"), 10),
    ODataAttributeMeta(QString("方位"), QString("double"), 10),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("月份"), QString("int"), 8),
    ODataAttributeMeta(QString("描述"), QString("int"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_J = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("类别"), QString("string"), 20),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("沟宽"), QString("double"), 10),
    ODataAttributeMeta(QString("方向"), QString("double"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_K = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("代码"), QString("string"), 40),
    ODataAttributeMeta(QString("编号"), QString("int"), 8),
    ODataAttributeMeta(QString("序号"), QString("int"), 8),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_L = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("高度"), QString("double"), 10),
    ODataAttributeMeta(QString("胸径"), QString("double"), 10),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_M = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("磁差值"), QString("double"), 10),
    ODataAttributeMeta(QString("参考年"), QString("int"), 8),
    ODataAttributeMeta(QString("年变值"), QString("double"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_N = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("性质"), QString("string"), 60),
    ODataAttributeMeta(QString("编号"), QString("int"), 8),
    ODataAttributeMeta(QString("颜色"), QString("string"), 20),
    ODataAttributeMeta(QString("色彩图案"), QString("string"), 20),
    ODataAttributeMeta(QString("顶标颜色"), QString("string"), 20),
    ODataAttributeMeta(QString("发光状态"), QString("string"), 20),
    ODataAttributeMeta(QString("高度"), QString("double"), 10),
    ODataAttributeMeta(QString("灯光特性"), QString("string"), 20),
    ODataAttributeMeta(QString("信号组"), QString("string"), 20),
    ODataAttributeMeta(QString("信号周期"), QString("double"), 10),
    ODataAttributeMeta(QString("作用距离"), QString("double"), 10),
    ODataAttributeMeta(QString("灯光可视"), QString("string"), 20),
    ODataAttributeMeta(QString("方位"), QString("double"), 10),
    ODataAttributeMeta(QString("航行指向"), QString("int"), 8),
    ODataAttributeMeta(QString("光弧角度1"), QString("double"), 10),
    ODataAttributeMeta(QString("光弧角度2"), QString("double"), 10),
    ODataAttributeMeta(QString("雷达可视"), QString("int"), 8),
    ODataAttributeMeta(QString("水深值1"), QString("double"), 10),
    ODataAttributeMeta(QString("作用方式"), QString("string"), 20),
    ODataAttributeMeta(QString("浮标系统"), QString("int"), 8),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_O = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("国家代码"), QString("string"), 20),
    ODataAttributeMeta(QString("编号"), QString("string"), 20),
    ODataAttributeMeta(QString("限制种类"), QString("string"), 40),
    ODataAttributeMeta(QString("存在状态"), QString("string"), 10),
    ODataAttributeMeta(QString("雷达可视"), QString("int"), 8),
    ODataAttributeMeta(QString("半径"), QString("double"), 10),
    ODataAttributeMeta(QString("高度"), QString("double"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_P = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("编号"), QString("string"), 20),
    ODataAttributeMeta(QString("跑道磁方向"), QString("double"), 10),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_Q = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 30),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("限制种类"), QString("string"), 40),
    ODataAttributeMeta(QString("图形特征"), QString("string"), 2),
    ODataAttributeMeta(QString("注记指针"), QString("int"), 20),
    ODataAttributeMeta(QString("外挂表指针"), QString("int"), 10)
};

vector<ODataAttributeMeta> FIELDS_R = {
    ODataAttributeMeta(QString("编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 100)
};

Layer* Layer::update() {
    parseUrl();
    parseField();
    return this;
}

Layer* Layer::parseField() {
    File meta = filegroup.matadata;
    QString shortname = meta.layername;
    if (shortname.compare("A") == 0) {
        fields = FIELDS_A;
    } else if (shortname.compare("B") == 0) {
        fields = FIELDS_B;
    } else if (shortname.compare("C") == 0) {
        fields = FIELDS_C;
    } else if (shortname.compare("D") == 0) {
        fields = FIELDS_D;
    } else if (shortname.compare("E") == 0) {
        fields = FIELDS_E;
    } else if (shortname.compare("F") == 0) {
        fields = FIELDS_F;
    } else if (shortname.compare("G") == 0) {
        fields = FIELDS_G;
    } else if (shortname.compare("H") == 0) {
        fields = FIELDS_H;
    } else if (shortname.compare("I") == 0) {
        fields = FIELDS_I;
    } else if (shortname.compare("J") == 0) {
        fields = FIELDS_J;
    } else if (shortname.compare("K") == 0) {
        fields = FIELDS_K;
    } else if (shortname.compare("L") == 0) {
        fields = FIELDS_L;
    } else if (shortname.compare("M") == 0) {
        fields = FIELDS_M;
    } else if (shortname.compare("N") == 0) {
        fields = FIELDS_N;
    } else if (shortname.compare("O") == 0) {
        fields = FIELDS_O;
    } else if (shortname.compare("P") == 0) {
        fields = FIELDS_P;
    } else if (shortname.compare("Q") == 0) {
        fields = FIELDS_Q;
    } else if (shortname.compare("R") == 0) {
        fields = FIELDS_R;
    }
    return this;
}

Layer* Layer::parseUrl() {
    File meta = filegroup.matadata;
    switch (type) {
        case LayerType::Point:
            uri = meta.layername + "_" + meta.cn_name + "_" + meta.code + "_POINT";
            break;
        case LayerType::Line:
            uri = meta.layername + "_" + meta.cn_name + "_" + meta.code + "_LINE";
            break;
        case LayerType::Area:
            uri = meta.layername + "_" + meta.cn_name + "_" + meta.code + "_AREA";
            break;
        case LayerType::Anno:
            uri = meta.layername + "_" + meta.cn_name + "_" + meta.code + "_ANNO";
            break;
        default:
            break;
    }
    return this;
}

QString Layer::fieldsToColumn() {
    QString fs("id serial primary key");
    int count = fields.size();
    for (int i = 0; i < count; i++) {
        ODataAttributeMeta field = fields[i];
        fs = fs + ", " + field.fieldname;
        QString f_item(" ");
        if (field.fieldtype.compare("string") == 0 ) {
            f_item = f_item + "varchar(" + QString::number(field.fieldlength) + ")";
        } else if (field.fieldtype.compare("int") == 0 ) {
            f_item = f_item + "int";
        } else if (field.fieldtype.compare("double") == 0 ) {
            f_item = f_item + "double precision";
        }
        fs += f_item;
    }
    return fs;
}

QString Layer::typeToColumn() {
    QString fs;
    switch(type) {
    case LayerType::Point:
        fs = "Point";
        break;
    case LayerType::Line:
        fs = "Point";
        break;
    case LayerType::Area:
        fs = "Point";
        break;
    case LayerType::Anno:
        fs = "Point";
        break;
    }
    return fs;
}

Layer* Layer::setFileGroup(FileGroup group) {
    filegroup = group;
    return this;
}

Layer* Layer::setLayerType(LayerType t) {
    type = t;
    return this;
}

void Layer::excutePostgis(PGconn *conn) {
    checkPostgisHasExist(conn);
    readFromFile();
    writeToPostgis(conn);
}

void Layer::deletePostgisHasExist(PGconn *conn) {
    QString delsql = "DROP TABLE "
        + uri
        + ";";
    PGresult *res = PQexec(conn, delsql.toStdString().c_str());
}

void Layer::checkPostgisHasExist(PGconn *conn) {
    QString sql = "CREATE TABLE IF NOT EXISTS "
        + uri
        + " ("
        + fieldsToColumn()
        + ",geom geometry"
        +  ");";
    printf(sql.toStdString().c_str());
    printf("\r\n");
    PGresult *res = PQexec(conn, sql.toStdString().c_str());
}

Layer* Layer::readFromFile() {
    readFromMsFile();
    readFromSxFile();
    readFromZbFile();
    return this;
}

Layer* Layer::readFromMsFile() {
    return this;
}

Layer* Layer::readFromSxFile() {
    if (type == LayerType::Point) {
        readFromSxFilePoint();
    } else if (type == LayerType::Line) {
        readFromSxFileLine();
    } else if (type == LayerType::Area) {
        readFromSxFileArea();
    }
    return this;
}

Layer* Layer::readFromSxFilePoint() {
    QFile file(filegroup.attrbution.path);
    if (!file.exists()) {
        return this;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return this;
    }

    QTextStream stream(&file);
    int file_row_number = 0;
    bool find_point = false;
    int file_start_index = 0;
    int file_end_index = 0;
    int file_cur_index = 0;

    stream.setCodec(QTextCodec::codecForName("gb2312"));
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        file_row_number++;
        if (!find_point) {
            // printf("find_point %d \r\n", file_row_number);
            // 开始 定位 "P 1000" 的开始位置
            int count = AttributeLineStringTypeCount(line, LayerType::Point);
            if (count >= 0) {
                file_start_index = file_row_number;
                file_end_index = file_start_index + count;
                find_point = true;
            }
        } else if (AttributeLineStringTypeCount(line, LayerType::Line) >= 0) {
            // 结束 定位 "P 1000" 的结束位置
            // printf("end_point %d %d %d \r\n", file_row_number, file_start_index, file_end_index);
            break;
        } else {
            file_cur_index++;
            if (file_row_number > file_start_index && file_row_number <= file_end_index) {
                // 实际属性转换代码
                ODataFeature feature;
                feature.type = FeatureType::POINT;
                feature.setProperties(line, fields, file_cur_index);
                features.push_back(feature);
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    return this;
}

Layer* Layer::readFromSxFileLine() {
    QFile file(filegroup.attrbution.path);
    if (!file.exists()) {
        return this;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return this;
    }

    QTextStream stream(&file);
    int file_row_number = 0;
    bool find_point = false;
    int file_start_index = 0;
    int file_end_index = 0;
    int file_cur_index = 0;

    stream.setCodec(QTextCodec::codecForName("gb2312"));
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        file_row_number++;
        if (!find_point) {
            // printf("find_point %d \r\n", file_row_number);
            // 开始 定位 "L 1000" 的开始位置
            int count = AttributeLineStringTypeCount(line, LayerType::Line);
            if (count >= 0) {
                file_start_index = file_row_number;
                file_end_index = file_start_index + count;
                find_point = true;
            }
        } else if (AttributeLineStringTypeCount(line, LayerType::Area) >= 0) {
            // 结束 定位 "P 1000" 的结束位置
            // printf("end_point %d %d %d \r\n", file_row_number, file_start_index, file_end_index);
            break;
        } else {
            file_cur_index++;
            if (file_row_number > file_start_index && file_row_number <= file_end_index) {
                // 实际属性转换代码
                ODataFeature feature;
                feature.setProperties(line, fields, file_cur_index);
                features.push_back(feature);
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    return this;
}

Layer* Layer::readFromSxFileArea() {
    QFile file(filegroup.attrbution.path);
    if (!file.exists()) {
        return this;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return this;
    }

    QTextStream stream(&file);
    int file_row_number = 0;
    bool find_point = false;
    int file_start_index = 0;
    int file_end_index = 0;
    int file_cur_index = 0;

    stream.setCodec(QTextCodec::codecForName("gb2312"));
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        file_row_number++;
        if (!find_point) {
            // printf("find_point %d \r\n", file_row_number);
            // 开始 定位 "A 1000" 的开始位置
            int count = AttributeLineStringTypeCount(line, LayerType::Area);
            if (count >= 0) {
                file_start_index = file_row_number;
                file_end_index = file_start_index + count;
                find_point = true;
            }
        } else if (AttributeLineStringTypeCount(line, LayerType::Anno) >= 0) {
            // 结束 定位 "A 1000" 的结束位置
            // printf("end_point %d %d %d \r\n", file_row_number, file_start_index, file_end_index);
            break;
        } else {
            file_cur_index++;
            if (file_row_number > file_start_index && file_row_number <= file_end_index) {
                // 实际属性转换代码
                ODataFeature feature;
                feature.setProperties(line, fields, file_cur_index);
                features.push_back(feature);
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    return this;
}

Layer* Layer::readFromZbFile() {
    if (type == LayerType::Point) {
        readFromZbFilePoint();
    } else if (type == LayerType::Line) {
        readFromZbFileLine();
    } else if (type == LayerType::Area) {
        readFromZbFileArea();
    }
    return this;
}

Layer* Layer::readFromZbFilePoint() {
    QFile file(filegroup.geometry.path);
    if (!file.exists()) {
        return this;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return this;
    }

    QTextStream stream(&file);
    int file_row_number = 0;
    bool find_point = false;
    int file_start_index = 0;
    int file_end_index = 0;
    int file_cur_index = 0;

    stream.setCodec(QTextCodec::codecForName("gb2312"));
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        file_row_number++;
        if (!find_point) {
            // printf("find_point %d \r\n", file_row_number);
            // 开始 定位 "P 1000" 的开始位置
            int count = GeometryLineStringTypeCount(line, LayerType::Point);
            if (count >= 0) {
                file_start_index = file_row_number;
                file_end_index = file_start_index + count;
                find_point = true;
            }
        } else if (GeometryLineStringTypeCount(line, LayerType::Line) >= 0) {
            // 结束 定位 "P 1000" 的结束位置
            // printf("end_point %d %d %d \r\n", file_row_number, file_start_index, file_end_index);
            break;
        } else {
            file_cur_index++;
            if (file_row_number > file_start_index && file_row_number <= file_end_index) {
                // 实际属性转换代码
                if (file_cur_index <= features.size()) {
                    features[file_cur_index - 1].setGeometryPoint(line, file_cur_index);
                }
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    return this;
}

Layer* Layer::readFromZbFileLine() {

    return this;
}

Layer* Layer::readFromZbFileArea() {

    return this;
}

void Layer::writeToPostgis(PGconn *conn) {
    int count = features.size();
    for (int i = 0; i < count; i++) {
        ODataFeature feature = features[i];
        if (true || feature.isValid) {
            QString sql = feature.toPostgis(uri, fields);
            printf(sql.toStdString().c_str());
            printf("\r\n");
            PGresult *res = PQexec(conn, sql.toStdString().c_str());
        } else {
//            QString sql = feature.toPostgis(uri, fields);
//            printf(sql.toStdString().c_str());
//            printf("\r\n");
        }
    }
}
