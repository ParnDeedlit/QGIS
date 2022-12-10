#include "odatalayer.h"
#include "odatafeature.h"

#include <stdio.h>

#include <spatialite/gaiageo.h>
#include <spatialite.h>

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QTextCodec>
#include <QtMath>

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
    ODataAttributeMeta(QString("名称"), QString("string"), 100),
    ODataAttributeMeta(QString("字体"), QString("string"), 20),
    ODataAttributeMeta(QString("字形"), QString("string"), 20),
    ODataAttributeMeta(QString("字级"), QString("string"), 10),
    ODataAttributeMeta(QString("字向"), QString("string"), 10),
    ODataAttributeMeta(QString("颜色"), QString("string"), 20)
};

vector<ODataAttributeMeta> NEW_FIELDS_A = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("等级"), QString("string"), 12),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("大地高"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("状态"), QString("string"), 12),
    ODataAttributeMeta(QString("重力值"), QString("double"), 15),
    ODataAttributeMeta(QString("理论横坐标"), QString("double"), 12),
    ODataAttributeMeta(QString("理论纵坐标"), QString("double"), 12),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_B = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("其他名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("水质"), QString("string"), 20),
    ODataAttributeMeta(QString("时令月份"), QString("string"), 20),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("水深"), QString("double"), 10),
    ODataAttributeMeta(QString("泥深"), QString("double"), 10),
    ODataAttributeMeta(QString("井深"), QString("double"), 10),
    ODataAttributeMeta(QString("长度"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("库容量"), QString("double"), 10),
    ODataAttributeMeta(QString("通航吨数"), QString("int"), 12),
    ODataAttributeMeta(QString("通航性质"), QString("string"), 8),
    ODataAttributeMeta(QString("水系名称代码"), QString("string"), 20),
    ODataAttributeMeta(QString("水系主体代码"), QString("string"), 20),
    ODataAttributeMeta(QString("水利行业编码"), QString("string"), 20),
    ODataAttributeMeta(QString("共享河段"), QString("string"), 255),
    ODataAttributeMeta(QString("植被类型"), QString("string"), 30),
    ODataAttributeMeta(QString("表面物质"), QString("string"), 30),
    ODataAttributeMeta(QString("存在状态"), QString("string"), 10),
    ODataAttributeMeta(QString("位置质量"), QString("string"), 20),
    ODataAttributeMeta(QString("颜色"), QString("string"), 20),
    ODataAttributeMeta(QString("与水面关系"), QString("string"), 10),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_C = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("性质"), QString("string"), 20),
    ODataAttributeMeta(QString("使用月份"), QString("string"), 20),
    ODataAttributeMeta(QString("几何形态"), QString("string"), 20),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("深度"), QString("double"), 10),
    ODataAttributeMeta(QString("人口数"), QString("double"), 10),
    ODataAttributeMeta(QString("行政区划代码"), QString("string"), 20),
    ODataAttributeMeta(QString("状态"), QString("string"), 12),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_D = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("编号"), QString("string"), 20),
    ODataAttributeMeta(QString("技术等级"), QString("string"), 20),
    ODataAttributeMeta(QString("铺设材料"), QString("string"), 20),
    ODataAttributeMeta(QString("车道数"), QString("int"), 8),
    ODataAttributeMeta(QString("单双行线"), QString("string"), 20),
    ODataAttributeMeta(QString("通行月份"), QString("string"), 20),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("铺面宽"), QString("double"), 10),
    ODataAttributeMeta(QString("道路行政归属"), QString("string"), 40),
    ODataAttributeMeta(QString("最小曲率半径"), QString("double"), 10),
    ODataAttributeMeta(QString("最大纵坡"), QString("double"), 10),
    ODataAttributeMeta(QString("载重"), QString("double"), 10),
    ODataAttributeMeta(QString("层数"), QString("int"), 8),
    ODataAttributeMeta(QString("长度"), QString("double"), 10),
    ODataAttributeMeta(QString("净空高"), QString("double"), 10),
    ODataAttributeMeta(QString("里程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("水深"), QString("double"), 10),
    ODataAttributeMeta(QString("河底性质"), QString("string"), 20),
    ODataAttributeMeta(QString("状态"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_E = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("电压值"), QString("string"), 20),
    ODataAttributeMeta(QString("净空高"), QString("double"), 10),
    ODataAttributeMeta(QString("埋藏深度"), QString("double"), 10),
    ODataAttributeMeta(QString("存在状态"), QString("string"), 10),
    ODataAttributeMeta(QString("作用方式"), QString("string"), 20),
    ODataAttributeMeta(QString("限制种类"), QString("string"), 40),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_F = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("行政区划代码"), QString("string"), 20),
    ODataAttributeMeta(QString("界碑编号"), QString("string"), 40),
    ODataAttributeMeta(QString("点号"), QString("string"), 20),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("保护区级别"), QString("string"), 8),
    ODataAttributeMeta(QString("人口数"), QString("double"), 10),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_G = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("类别"), QString("string"), 20),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高值"), QString("double"), 10),
    ODataAttributeMeta(QString("沟宽"), QString("double"), 10),
    ODataAttributeMeta(QString("方向"), QString("double"), 15),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_H = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("高度"), QString("double"), 10),
    ODataAttributeMeta(QString("胸径"), QString("double"), 10),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("状态"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_I = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("水深值"), QString("double"), 10),
    ODataAttributeMeta(QString("水深值1"), QString("double"), 10),
    ODataAttributeMeta(QString("水深值2"), QString("double"), 10),
    ODataAttributeMeta(QString("测深技术"), QString("string"), 40),
    ODataAttributeMeta(QString("测深质量"), QString("string"), 40),
    ODataAttributeMeta(QString("位置质量"), QString("string"), 20),
    ODataAttributeMeta(QString("颜色"), QString("string"), 20),
    ODataAttributeMeta(QString("与周围水深的关系"), QString("string"), 20),
    ODataAttributeMeta(QString("作用方式"), QString("string"), 20),
    ODataAttributeMeta(QString("表面物质"), QString("string"), 30),
    ODataAttributeMeta(QString("物质形态"), QString("string"), 30),
    ODataAttributeMeta(QString("危险级"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_J = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("水深值"), QString("double"), 10),
    ODataAttributeMeta(QString("高度"), QString("double"), 10),
    ODataAttributeMeta(QString("垂高"), QString("double"), 10),
    ODataAttributeMeta(QString("存在状态"), QString("string"), 10),
    ODataAttributeMeta(QString("测深技术"), QString("string"), 40),
    ODataAttributeMeta(QString("测深质量"), QString("string"), 40),
    ODataAttributeMeta(QString("位置质量"), QString("string"), 20),
    ODataAttributeMeta(QString("限制种类"), QString("string"), 40),
    ODataAttributeMeta(QString("区域界线类型"), QString("string"), 40),
    ODataAttributeMeta(QString("作用方式"), QString("string"), 20),
    ODataAttributeMeta(QString("表面物质"), QString("string"), 30),
    ODataAttributeMeta(QString("与水面关系"), QString("string"), 10),
    ODataAttributeMeta(QString("危险级"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_K = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("国家代码"), QString("string"), 20),
    ODataAttributeMeta(QString("编号"), QString("string"), 20),
    ODataAttributeMeta(QString("限制种类"), QString("string"), 40),
    ODataAttributeMeta(QString("存在状态"), QString("string"), 10),
    ODataAttributeMeta(QString("雷达可视"), QString("int"), 8),
    ODataAttributeMeta(QString("半径"), QString("double"), 10),
    ODataAttributeMeta(QString("高度"), QString("double"), 10),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_L = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("性质"), QString("string"), 60),
    ODataAttributeMeta(QString("编号"), QString("string"), 20),
    ODataAttributeMeta(QString("颜色"), QString("string"), 20),
    ODataAttributeMeta(QString("色彩图案"), QString("string"), 20),
    ODataAttributeMeta(QString("顶标颜色"), QString("string"), 20),
    ODataAttributeMeta(QString("发光状态"), QString("string"), 20),
    ODataAttributeMeta(QString("高度"), QString("double"), 10),
    ODataAttributeMeta(QString("灯光特性"), QString("string"), 20),
    ODataAttributeMeta(QString("岸别"), QString("string"), 20),
    ODataAttributeMeta(QString("信号组"), QString("string"), 20),
    ODataAttributeMeta(QString("信号周期"), QString("double"), 10),
    ODataAttributeMeta(QString("作用距离"), QString("double"), 10),
    ODataAttributeMeta(QString("灯光可视"), QString("string"), 20),
    ODataAttributeMeta(QString("方位"), QString("double"), 15),
    ODataAttributeMeta(QString("航行指向"), QString("double"), 15),
    ODataAttributeMeta(QString("光弧角度1"), QString("double"), 15),
    ODataAttributeMeta(QString("光弧角度2"), QString("double"), 15),
    ODataAttributeMeta(QString("雷达可视"), QString("int"), 8),
    ODataAttributeMeta(QString("视觉可见"), QString("string"), 20),
    ODataAttributeMeta(QString("限制种类"), QString("string"), 20),
    ODataAttributeMeta(QString("水深值1"), QString("double"), 10),
    ODataAttributeMeta(QString("作用方式"), QString("string"), 20),
    ODataAttributeMeta(QString("浮标系统"), QString("string"), 8),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_M = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("深度"), QString("double"), 10),
    ODataAttributeMeta(QString("底质"), QString("string"), 20),
    ODataAttributeMeta(QString("流速"), QString("double"), 10),
    ODataAttributeMeta(QString("方位"), QString("double"), 15),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("月份"), QString("int"), 8),
    ODataAttributeMeta(QString("描述"), QString("int"), 12),
    ODataAttributeMeta(QString("潮汐潮位精度"), QString("string"), 20),
    ODataAttributeMeta(QString("潮汐预报方法"), QString("string"), 20),
    ODataAttributeMeta(QString("分道通航制类"), QString("string"), 20),
    ODataAttributeMeta(QString("推荐航线类"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_N = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("代码"), QString("string"), 40),
    ODataAttributeMeta(QString("编号"), QString("string"), 20),
    ODataAttributeMeta(QString("跑道数"), QString("int"), 8),
    ODataAttributeMeta(QString("跑道磁方向"), QString("double"), 15),
    ODataAttributeMeta(QString("频率"), QString("double"), 15),
    ODataAttributeMeta(QString("长度"), QString("double"), 15),
    ODataAttributeMeta(QString("高程"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("限制高度"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_O = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("磁力值"), QString("double"), 12),
    ODataAttributeMeta(QString("磁差值"), QString("double"), 15),
    ODataAttributeMeta(QString("参考年"), QString("int"), 8),
    ODataAttributeMeta(QString("年变值"), QString("double"), 15),
    ODataAttributeMeta(QString("重力值"), QString("double"), 15),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_P = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("目标所在国家地区"), QString("string"), 128),
    ODataAttributeMeta(QString("目标所属国家地区"), QString("string"), 128),
    ODataAttributeMeta(QString("目标概况"), QString("string"), 1024),
    ODataAttributeMeta(QString("子目标名称"), QString("string"), 128),
    ODataAttributeMeta(QString("子目标编码"), QString("string"), 128),
    ODataAttributeMeta(QString("子目标类型"), QString("string"), 32),
    ODataAttributeMeta(QString("定点类型"), QString("string"), 32),
    ODataAttributeMeta(QString("结构材质"), QString("string"), 64),
    ODataAttributeMeta(QString("子目标说明"), QString("string"), 1024),
    ODataAttributeMeta(QString("子目标地址"), QString("string"), 256),
    ODataAttributeMeta(QString("长度"), QString("double"), 10),
    ODataAttributeMeta(QString("宽度"), QString("double"), 10),
    ODataAttributeMeta(QString("比高"), QString("double"), 10),
    ODataAttributeMeta(QString("直径"), QString("double"), 10),
    ODataAttributeMeta(QString("面积"), QString("double"), 10),
    ODataAttributeMeta(QString("形状"), QString("string"), 256),
    ODataAttributeMeta(QString("大地高"), QString("double"), 10),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("限制种类"), QString("string"), 40),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("角度"), QString("double"), 15),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_Q = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("中文名称"), QString("string"), 100),
    ODataAttributeMeta(QString("汉语拼音"), QString("string"), 100),
    ODataAttributeMeta(QString("中文副名"), QString("string"), 100),
    ODataAttributeMeta(QString("简称"), QString("string"), 100),
    ODataAttributeMeta(QString("别名"), QString("string"), 100),
    ODataAttributeMeta(QString("外文名称"), QString("string"), 100),
    ODataAttributeMeta(QString("外文语种"), QString("string"), 100),
    ODataAttributeMeta(QString("罗马化转写名称"), QString("string"), 100),
    ODataAttributeMeta(QString("名称编码"), QString("string"), 255),
    ODataAttributeMeta(QString("行政区划代码"), QString("string"), 30),
    ODataAttributeMeta(QString("使用状态"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("名称唯一标识码"), QString("string"), 100),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_R = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 60),
    ODataAttributeMeta(QString("成因类型"), QString("string"), 60),
    ODataAttributeMeta(QString("状态"), QString("string"), 60),
    ODataAttributeMeta(QString("密实度"), QString("string"), 20),
    ODataAttributeMeta(QString("坚硬程度"), QString("string"), 20),
    ODataAttributeMeta(QString("承载力"), QString("string"), 20),
    ODataAttributeMeta(QString("厚度"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_S = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("坚硬程度"), QString("string"), 60),
    ODataAttributeMeta(QString("完整程度"), QString("string"), 60),
    ODataAttributeMeta(QString("质量等级"), QString("string"), 20),
    ODataAttributeMeta(QString("承载力"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_T = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("位置"), QString("string"), 60),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("稳定性"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_U = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("位置"), QString("string"), 60),
    ODataAttributeMeta(QString("水位埋深"), QString("double"), 8),
    ODataAttributeMeta(QString("含水层厚度"), QString("double"), 7),
    ODataAttributeMeta(QString("埋藏条件"), QString("string"), 10),
    ODataAttributeMeta(QString("水质"), QString("string"), 20),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_V = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("位置"), QString("string"), 60),
    ODataAttributeMeta(QString("类型"), QString("string"), 20),
    ODataAttributeMeta(QString("规模"), QString("string"), 20),
    ODataAttributeMeta(QString("汇水面积"), QString("double"), 8),
    ODataAttributeMeta(QString("相对高差"), QString("double"), 8),
    ODataAttributeMeta(QString("稳定程度"), QString("string"), 60),
    ODataAttributeMeta(QString("诱发因素"), QString("string"), 100),
    ODataAttributeMeta(QString("易发程度"), QString("string"), 60),
    ODataAttributeMeta(QString("破坏程度"), QString("string"), 60),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_W = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("名称"), QString("string"), 255),
    ODataAttributeMeta(QString("位置"), QString("string"), 60),
    ODataAttributeMeta(QString("分类"), QString("string"), 60),
    ODataAttributeMeta(QString("规模"), QString("string"), 60),
    ODataAttributeMeta(QString("开采利用情况"), QString("string"), 60),
    ODataAttributeMeta(QString("资源量"), QString("double"), 11),
    ODataAttributeMeta(QString("开采量"), QString("double"), 11),
    ODataAttributeMeta(QString("矿种元素"), QString("string"), 10),
    ODataAttributeMeta(QString("矿种名称"), QString("string"), 10),
    ODataAttributeMeta(QString("现势性"), QString("string"), 8),
    ODataAttributeMeta(QString("名称指针"), QString("int"), 12),
    ODataAttributeMeta(QString("外挂表名称"), QString("string"), 30),
    ODataAttributeMeta(QString("要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("上一级要素唯一编码"), QString("string"), 75),
    ODataAttributeMeta(QString("网格码"), QString("string"), 30),
    ODataAttributeMeta(QString("更新状态标识"), QString("string"), 8),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};
vector<ODataAttributeMeta> NEW_FIELDS_Y = {
    ODataAttributeMeta(QString("分类编码"), QString("int"), 10),
    ODataAttributeMeta(QString("信息"), QString("string"), 100),
    ODataAttributeMeta(QString("版本标识"), QString("string"), 8),
    ODataAttributeMeta(QString("备注"), QString("string"), 60)
};

Layer* Layer::update() {
    parseUrl();
    parseField();
    return this;
}

Layer* Layer::parseField() {
    File meta = filegroup.matadata;
    if (meta.layername.compare("") == 0) {
        meta = filegroup.geometry;
    }

    QString shortname = meta.layername;
    if (mapMetadata.version == OdataVersion::ODATA_1_0) {
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
    } else if (mapMetadata.version == OdataVersion::ODATA_2_0
        || mapMetadata.version == OdataVersion::ENTITY_1_0) {
        if (shortname.compare("A") == 0) {
            fields = NEW_FIELDS_A;
        } else if (shortname.compare("B") == 0) {
            fields = NEW_FIELDS_B;
        } else if (shortname.compare("C") == 0) {
            fields = NEW_FIELDS_C;
        } else if (shortname.compare("D") == 0) {
            fields = NEW_FIELDS_D;
        } else if (shortname.compare("E") == 0) {
            fields = NEW_FIELDS_E;
        } else if (shortname.compare("F") == 0) {
            fields = NEW_FIELDS_F;
        } else if (shortname.compare("G") == 0) {
            fields = NEW_FIELDS_G;
        } else if (shortname.compare("H") == 0) {
            fields = NEW_FIELDS_H;
        } else if (shortname.compare("I") == 0) {
            fields = NEW_FIELDS_I;
        } else if (shortname.compare("J") == 0) {
            fields = NEW_FIELDS_J;
        } else if (shortname.compare("K") == 0) {
            fields = NEW_FIELDS_K;
        } else if (shortname.compare("L") == 0) {
            fields = NEW_FIELDS_L;
        } else if (shortname.compare("M") == 0) {
            fields = NEW_FIELDS_M;
        } else if (shortname.compare("N") == 0) {
            fields = NEW_FIELDS_N;
        } else if (shortname.compare("O") == 0) {
            fields = NEW_FIELDS_O;
        } else if (shortname.compare("P") == 0) {
            fields = NEW_FIELDS_P;
        } else if (shortname.compare("Q") == 0) {
            fields = NEW_FIELDS_Q;
        } else if (shortname.compare("R") == 0) {
            fields = NEW_FIELDS_R;
        } else if (shortname.compare("S") == 0) {
            fields = NEW_FIELDS_S;
        } else if (shortname.compare("T") == 0) {
            fields = NEW_FIELDS_T;
        } else if (shortname.compare("U") == 0) {
            fields = NEW_FIELDS_U;
        } else if (shortname.compare("V") == 0) {
            fields = NEW_FIELDS_V;
        } else if (shortname.compare("W") == 0) {
            fields = NEW_FIELDS_W;
        } else if (shortname.compare("Y") == 0) {
            fields = NEW_FIELDS_Y;
        }
    }

    return this;
}

Layer* Layer::parseUrl() {
    File meta = filegroup.matadata;
    if (meta.layername.compare("") == 0) {
        meta = filegroup.geometry;
    }
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

QString Layer::fieldsToColumnBySqlite() {
    QString fs("id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
    int count = fields.size();
    for (int i = 0; i < count; i++) {
        ODataAttributeMeta field = fields[i];
        fs = fs + ", " + field.fieldname;
        QString f_item(" ");
        if (field.fieldtype.compare("string") == 0 ) {
            f_item = f_item + "TEXT(" + QString::number(field.fieldlength) + ")";
        } else if (field.fieldtype.compare("int") == 0 ) {
            f_item = f_item + "INTEGER";
        } else if (field.fieldtype.compare("double") == 0 ) {
            f_item = f_item + "DOUBLE";
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

Layer* Layer::setMapMetadata(MapMataData meta) {
    mapMetadata = meta;
    return this;
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
    printf("postgis \r\n");
    PGresult *res = PQexec(conn, sql.toStdString().c_str());
}


void Layer::excuteGeopackage(sqlite3 *conn) {
    checkGeopackageHasExist(conn);
    readFromFile();
    writeToGeopackage(conn);
}
void Layer::deleteGeopackageHasExist(sqlite3 *conn) {

}

void Layer::checkGeopackageHasExist(sqlite3 *conn){
    QString geometryType;
    char *err_msg = nullptr;
    switch(type) {
        case LayerType::Point:
        case LayerType::Anno:
            geometryType = "POINT";
            break;
        case LayerType::Line:
            geometryType = "LINESTRING";
            break;
        case LayerType::Area:
            geometryType = "POLYGON";
            break;
    }

    QString sql = "CREATE TABLE IF NOT EXISTS "
        + uri
        + " ("
        + fieldsToColumnBySqlite()
        + ",geom " + geometryType
        +  ");";
    // printf(sql.toStdString().c_str());
    // printf("\r\n");
    sqlite3_exec(conn, sql.toStdString().c_str(), 0, 0, &err_msg);

    QString insert_4326("INSERT INTO gpkg_contents (table_name, data_type, identifier, srs_id) ");
    QString values("VALUES('" + uri + "',");
    QString v1("'features', ");
    QString v2("'" + uri + "',");
    QString v3("4326);");
    QString all = insert_4326 + values + v1 + v2 + v3;
    sqlite3_exec(conn, all.toStdString().c_str(), 0, 0, &err_msg);

    QString insert_cols("INSERT INTO gpkg_geometry_columns (table_name, column_name, geometry_type_name, srs_id, z, m) ");
    QString table("VALUES('" + uri + "',");
    QString geom = "'geom', '" + geometryType + "', 4326, 0, 0);";
    QString sql_col = insert_cols + table + geom;
    sqlite3_exec(conn, sql_col.toStdString().c_str(), 0, 0, &err_msg);
}

void Layer::writeToGeopackage(sqlite3 *conn, int batchcount){
    char *err_msg = nullptr;
    int count = features.size();
    QString table("");
    QString values("");
    QString sql("");
    int rtn;

    if (count > 0 ) {
        for (int i = 0; i < count; i++) {
            ODataFeature feature = features[i];
            QString table = feature.toGeoPackageTable(uri, fields);
            QString value = feature.toGeoPackageVaules(uri, fields);
            QString exe = table + value;
            //printf(exe.toStdString().c_str());
            // printf("\r\n");
            // sqlite3_exec(conn, exe.toStdString().c_str(), 0, 0, &err_msg);
            sqlite3_stmt *stmt = 0;

            if (feature.geometry.type == FeatureType::POINT) {
                unsigned char *blob;
                int blob_size;
                gaiaGeomCollPtr geo = NULL;
                /* preparing the geometry to insert */
                geo = gaiaAllocGeomColl();
                geo->Srid = 4326;
                gaiaAddPointToGeomColl (geo, feature.geometry.point.coordinates.x, feature.geometry.point.coordinates.y);
                /* transforming this geometry into the SpatiaLite BLOB format */
                gaiaToSpatiaLiteBlobWkb (geo, &blob, &blob_size);
                /* we can now destroy the geometry object */
                gaiaFreeGeomColl (geo);
                rtn = sqlite3_prepare_v2(conn, exe.toStdString().c_str(), strlen(exe.toStdString().c_str()) + 1, &stmt, NULL);
                rtn = sqlite3_bind_blob(stmt, 1, blob, blob_size, SQLITE_TRANSIENT);
            } else if (feature.geometry.type == FeatureType::LINESTRING) {
                unsigned char *blob;
                int blob_size;
                gaiaGeomCollPtr geo = NULL;
                /* preparing the geometry to insert */
                geo = gaiaAllocGeomColl();
                geo->Srid = 4326;
                gaiaLinestringPtr line = gaiaAddLinestringToGeomColl(geo, feature.geometry.line.coordinates.size());
                for(int i = 0; i < feature.geometry.line.coordinates.size(); i++) {
                   BasePoint point = feature.geometry.line.coordinates[i];
                   gaiaLineSetPoint(line, i, point.x, point.y, 0, 0);
                }

                /* transforming this geometry into the SpatiaLite BLOB format */
                gaiaToSpatiaLiteBlobWkb (geo, &blob, &blob_size);
                /* we can now destroy the geometry object */
                gaiaFreeGeomColl (geo);
                rtn = sqlite3_prepare_v2(conn, exe.toStdString().c_str(), strlen(exe.toStdString().c_str()) + 1, &stmt, NULL);
                rtn = sqlite3_bind_blob(stmt, 1, blob, blob_size, SQLITE_TRANSIENT);
            } else if (feature.geometry.type == FeatureType::POLYGON) {
                unsigned char *blob;
                int blob_size;
                gaiaGeomCollPtr geo = NULL;
                /* preparing the geometry to insert */
                geo = gaiaAllocGeomColl();
                geo->Srid = 4326;
                gaiaPolygonPtr area;
                if (feature.geometry.polygon.coordinates.size() > 1) {
                    int innersize = feature.geometry.polygon.coordinates.size() - 1;
                    area = gaiaAddPolygonToGeomColl(geo, feature.geometry.polygon.coordinates[0].size(), innersize);
                    gaiaRingPtr ring = area->Exterior;
                    for(int i = 0; i < feature.geometry.polygon.coordinates[0].size(); i++) {
                       BasePoint point = feature.geometry.polygon.coordinates[0][i];
                       gaiaSetPoint (ring->Coords, i, point.x, point.y);
                    }
                    for (int j = 0; j < innersize; j++) {
                        int innerringsize = feature.geometry.polygon.coordinates[j+1].size();
                        ring = gaiaAddInteriorRing (area, j+1, innerringsize);
                        for (int k =0; k < innerringsize; k++) {
                            BasePoint point = feature.geometry.polygon.coordinates[j+1][k];
                            gaiaSetPoint (ring->Coords, k, point.x, point.y);
                        }
                    }

                } else if (feature.geometry.polygon.coordinates.size() > 0) {
                    area = gaiaAddPolygonToGeomColl(geo, feature.geometry.polygon.coordinates[0].size(), 0);
                    gaiaRingPtr ring = area->Exterior;
                    for(int i = 0; i < feature.geometry.polygon.coordinates[0].size(); i++) {
                       BasePoint point = feature.geometry.polygon.coordinates[0][i];
                       gaiaSetPoint (ring->Coords, i, point.x, point.y);
                    }
                } else {
                    // do nothing
                }

                /* transforming this geometry into the SpatiaLite BLOB format */
                gaiaToSpatiaLiteBlobWkb (geo, &blob, &blob_size);
                /* we can now destroy the geometry object */
                gaiaFreeGeomColl (geo);
                rtn = sqlite3_prepare_v2(conn, exe.toStdString().c_str(), strlen(exe.toStdString().c_str()) + 1, &stmt, NULL);
                rtn = sqlite3_bind_blob(stmt, 1, blob, blob_size, SQLITE_TRANSIENT);
            }

            rtn = sqlite3_step(stmt);
            if (rtn != SQLITE_DONE) {
                printf("Error message: %s\n", sqlite3_errmsg(conn));
            }
            rtn = sqlite3_finalize(stmt);
        }

//        ODataFeature feature = features[0];
//        QString sql = feature.toGeoPackage(uri, fields);
//        printf(sql.toStdString().c_str());
//        printf("\r\n");
//        const char * exe = sql.toStdString().c_str();
//        gaiaGeomCollPtr geo = NULL;
//        unsigned char *blob;
//        QString begin("BEGIN");
//           int ret = sqlite3_exec (conn, begin.toStdString().c_str(), NULL, NULL, &err_msg);
//           if (ret != SQLITE_OK)
//             {
//       /* an error occurred */
//                 printf ("BEGIN error: %s\n", err_msg);
//                 sqlite3_free (err_msg);
//             }
//        sqlite3_stmt *stmt;
//        if (sqlite3_prepare_v2(conn,exe,strlen(exe),&stmt,nullptr) != SQLITE_OK) {
//            if (stmt) {
//              sqlite3_finalize(stmt);
//              return;
//            }
//        }
//        const char* str1 = "ts";
//        for (int i = 0; i < count; i++) {
//            ODataFeature feature = features[i];
//            if (feature.isValid) {
//                feature.geometry.unprojection(&mapMetadata);


//                for(int j = 0; j < feature.properties.size(); j++) {
//                    ODataAttribute attr = feature.properties[j];
//                    if (attr.type.compare("string") == 0) {
//                        const char* str = attr.value.toString().toStdString().c_str();

//                        sqlite3_bind_text(stmt,j+1, str1, strlen(str1), SQLITE_TRANSIENT);
//                        printf(" sting %d %s", j+1, str);
//                    } else if (attr.type.compare("int") == 0) {
//                        sqlite3_bind_int(stmt,j+1, attr.value.toInt());
//                        printf(" int %d %d", j+1, attr.value.toInt());
//                    } else if (attr.type.compare("double") == 0) {
//                        sqlite3_bind_double(stmt,j+1, attr.value.toDouble());
//                        printf(" double %d %f", j+1, attr.value.toDouble());
//                    }
//                }
//               printf("\r\n ");
//               int ret = sqlite3_step (stmt);
//               if (ret == SQLITE_DONE || ret == SQLITE_ROW) {
//               } else {
//                /* an unexpected error occurred */
//                printf ("sqlite3_step() error: %s\n", sqlite3_errmsg (conn));
//               }
//               sqlite3_reset(stmt);
//               // sqlite3_clear_bindings (stmt);
//            }
//        }
//        sqlite3_finalize(stmt);
//        QString commit("COMMIT");
//        ret = sqlite3_exec (conn, commit.toStdString().c_str(), NULL, NULL, &err_msg);
//        if (ret != SQLITE_OK)
//        {
//          printf ("COMMIT error: %s\n", err_msg);
//          sqlite3_free (err_msg);
//        }

    }
}

void Layer::writeToPostgis(PGconn *conn, int batchcount) {
    int count = features.size();
    printf("features all count %i \r\n", count);
//    for (int i = 0; i < count; i++) {
//        ODataFeature feature = features[i];
//        if (feature.isValid) {
//            feature.geometry.unprojection(&mapMetadata);
//            QString sql = feature.toPostgis(uri, fields);
//            printf(sql.toStdString().c_str());
//            printf("\r\n");
//            PGresult *res = PQexec(conn, sql.toStdString().c_str());
//        }
//    }


    int batchindex = 0;
    QString table("");
    QString values("");
    QString sql("");

    for (int i = 0; i < count; i++) {
        ODataFeature feature = features[i];
        if (feature.isValid) {
            feature.geometry.unprojection(&mapMetadata);
            QString value = feature.toPostgisVaules(uri, fields);
            if (batchindex == 0) {
                table = feature.toPostgisTable(uri, fields);
                values = "";
                if (i == count - 1 ) {
                    value += ";";
                    values += value;
                    sql = table + values;
                    PGresult *res = PQexec(conn, sql.toStdString().c_str());
//                    printf(sql.toStdString().c_str());
//                    printf("1 \r\n");
                } else {
                    value += ", ";
                    values += value;
                }
                batchindex++;
            } else if (batchindex >= batchcount) {
                value += ";";
                values += value;
                sql = table + values;
                PGresult *res = PQexec(conn, sql.toStdString().c_str());
//                printf(sql.toStdString().c_str());
//                printf("2 \r\n");
                batchindex = 0;
                table = "";
                values = "";
            } else {
                if (i == count - 1 ) {
                    value += ";";
                    values += value;
                    sql = table + values;
                    PGresult *res = PQexec(conn, sql.toStdString().c_str());
//                    printf(sql.toStdString().c_str());
//                    printf("3 \r\n");
                } else {
                    value += ", ";
                    values += value;
                }
                batchindex++;
            }
        }
    }
    values += ";";
}

void Layer::writeToGeojson(QString uri, int batchcount) {

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
    } else if (type == LayerType::Anno) {
        readFromSxFileAnno();
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

    if(mapMetadata.version == OdataVersion::ODATA_1_0) {
        stream.setCodec(QTextCodec::codecForName("gb2312"));
    } else if (mapMetadata.version == OdataVersion::ODATA_2_0 ||
        mapMetadata.version == OdataVersion::ENTITY_1_0) {
        stream.setCodec(QTextCodec::codecForName("utf-8"));
    }

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
                feature.setPropertiesSplit(line, fields, file_cur_index);
                features.push_back(feature);
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    file.close();
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

    if(mapMetadata.version == OdataVersion::ODATA_1_0) {
        stream.setCodec(QTextCodec::codecForName("gb2312"));
    } else if (mapMetadata.version == OdataVersion::ODATA_2_0 ||
        mapMetadata.version == OdataVersion::ENTITY_1_0) {
        stream.setCodec(QTextCodec::codecForName("utf-8"));
    }

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
                feature.setPropertiesSplit(line, fields, file_cur_index);
                features.push_back(feature);
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    file.close();
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

    if(mapMetadata.version == OdataVersion::ODATA_1_0) {
        stream.setCodec(QTextCodec::codecForName("gb2312"));
    } else if (mapMetadata.version == OdataVersion::ODATA_2_0 ||
        mapMetadata.version == OdataVersion::ENTITY_1_0) {
        stream.setCodec(QTextCodec::codecForName("utf-8"));
    }

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
                feature.setPropertiesSplit(line, fields, file_cur_index);
                features.push_back(feature);
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    file.close();
    return this;
}

Layer* Layer::readFromSxFileAnno() {
    QFile file(filegroup.attrbution.path);
    if (!file.exists()) {
        return this;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return this;
    }

    QTextStream stream(&file);
    int file_row_number = 0;
    bool find_anno = false;
    int file_start_index = 0;
    int file_end_index = 0;
    int file_cur_index = 0;

    if(mapMetadata.version == OdataVersion::ODATA_1_0) {
        stream.setCodec(QTextCodec::codecForName("gb2312"));
    } else if (mapMetadata.version == OdataVersion::ODATA_2_0 ||
        mapMetadata.version == OdataVersion::ENTITY_1_0) {
        stream.setCodec(QTextCodec::codecForName("utf-8"));
    }

    while(!stream.atEnd()) {
        QString line = stream.readLine();
        file_row_number++;
        if (!find_anno) {
            // 开始 定位 "P 1000" 的开始位置
            int count = AttributeLineStringTypeCount(line, LayerType::Anno, mapMetadata.version);
            if (count >= 0) {
                file_start_index = file_row_number;
                file_end_index = file_start_index + count;
                find_anno = true;
            }
        } else {
            file_cur_index++;
            if (file_row_number > file_start_index && file_row_number <= file_end_index) {
                // 实际属性转换代码
                ODataFeature feature;
                feature.type = FeatureType::POINT;
                feature.setPropertiesSplit(line, fields, file_cur_index);
                features.push_back(feature);
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    file.close();
    return this;
}

Layer* Layer::readFromZbFile() {
    if (type == LayerType::Point) {
        readFromZbFilePoint();
    } else if (type == LayerType::Line) {
        readFromZbFileLine();
    } else if (type == LayerType::Area) {
        readFromZbFileArea();
    } else if (type == LayerType::Anno) {
        readFromZbFileAnno();
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

    if(mapMetadata.version == OdataVersion::ODATA_1_0) {
        stream.setCodec(QTextCodec::codecForName("gb2312"));
    } else if (mapMetadata.version == OdataVersion::ODATA_2_0 ||
        mapMetadata.version == OdataVersion::ENTITY_1_0) {
        stream.setCodec(QTextCodec::codecForName("utf-8"));
    }

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
                    features[file_cur_index - 1].setGeometryPoint(line, file_cur_index, &mapMetadata);
                }
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    file.close();
    return this;
}

Layer* Layer::readFromZbFileLine() {
    QFile file(filegroup.geometry.path);
    if (!file.exists()) {
        return this;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return this;
    }

    QTextStream stream(&file);
    int file_row_number = 0;
    bool find_line = false;
    int line_total_size = 0;
    int line_last_index = 0;
    int line_cur_index = 0;
    int line_cur_coords_size = 0;
    int line_cur_index_temp = 0;
    int feature_count = features.size();

    if(mapMetadata.version == OdataVersion::ODATA_1_0) {
        stream.setCodec(QTextCodec::codecForName("gb2312"));
    } else if (mapMetadata.version == OdataVersion::ODATA_2_0 ||
        mapMetadata.version == OdataVersion::ENTITY_1_0) {
        stream.setCodec(QTextCodec::codecForName("utf-8"));
    }
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        file_row_number++;
        if (!find_line) {
            // 开始 定位 "L 1000" 的开始位置
            int count = GeometryLineStringTypeCount(line, LayerType::Line);
            if (count >= 0) {
                line_total_size = count;
                find_line = true;
            }
        } else if (GeometryLineStringTypeCount(line, LayerType::Area) >= 0) {
            // 结束 定位 "L 1000" 的结束位置
            break;
        } else {
            bool findnewline = false;
            GeometryLineCoordCount(line, line_cur_index_temp, line_cur_coords_size);
            if (line_cur_index_temp == line_last_index + 1) {
                line_cur_index = line_cur_index_temp;
                findnewline = true;
            }

            if (findnewline) {
                line_last_index = line_cur_index;
                // printf("find index %d %d \r\n", line_cur_index, line_cur_coords_size);
            } else {
                // printf("readline check %d %d \r\n", line_cur_index, feature_count);
                if (line_cur_index <= feature_count) {
                    features[line_cur_index - 1].setGeometryLine(line, &mapMetadata);
                    // printf("readline %d \r\n", line_cur_index);
                }
            }
        }
    }
    file.close();
    return this;
}

Layer* Layer::readFromZbFileArea() {
    QFile file(filegroup.geometry.path);
    if (!file.exists()) {
        return this;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return this;
    }

    QTextStream stream(&file);
    int file_row_number = 0;
    bool find_area = false;
    int area_total_size = 0;
    int area_cur_index = 0;
    int area_last_index = 0;
    // int area_end_index = 0;
    int area_cur_ring_count = 0;
    int area_cur_ring_count_temp = 0;
    int area_cur_ring_number = 0;
    int area_cur_ring_index = -1;
    int area_cur_ring_coords_size = 0;
    int area_cur_index_temp = 0;
    int feature_count = features.size();
    OdataVersion version = mapMetadata.version;

    if(mapMetadata.version == OdataVersion::ODATA_1_0) {
        stream.setCodec(QTextCodec::codecForName("gb2312"));
    } else if (mapMetadata.version == OdataVersion::ODATA_2_0 ||
        mapMetadata.version == OdataVersion::ENTITY_1_0) {
        stream.setCodec(QTextCodec::codecForName("utf-8"));
    }
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        file_row_number++;
        if (!find_area) {
            // 开始 定位 "L 1000" 的开始位置
            int count = GeometryLineStringTypeCount(line, LayerType::Area);
            if (count >= 0) {
                area_total_size = count;
                find_area = true;
            }
        } else {
            bool findnewarea = false;
            GeometryAreaRingCount(line, area_cur_index_temp, area_cur_ring_count_temp, version);
            if (area_cur_index_temp == area_last_index + 1) {
                area_cur_index = area_cur_index_temp;
                area_cur_ring_count = area_cur_ring_count_temp;
                findnewarea = true;
            }

            if (findnewarea) {
                area_last_index = area_cur_index;
                area_cur_ring_index = -1;
                if (area_cur_ring_count == 0) {
                    // 1        0.000000        0.000000          0 不做任何处理
                    area_cur_ring_number = file_row_number;
                    // printf("0. empty area do nothing!  \r\n");
                } else {
                    area_cur_ring_number = file_row_number + 1;
                    // printf("1. valid area do loop!  \r\n");
                }
            } else {
                if (file_row_number == area_cur_ring_number) {
                    GeometryAreaCoordCount(line, area_cur_ring_coords_size);
                    // printf("2. count area ring!  \r\n");
                    area_cur_ring_index++;
                } else {
                    if (area_cur_index <= feature_count) {
                        // printf("3. loop area ring point! %d \r\n", area_cur_index);
                        features[area_cur_index - 1].setGeometryArea(line, area_cur_ring_index, &mapMetadata);
                    }
                }
            }
        }
    }
    file.close();
    return this;
}

Layer* Layer::readFromZbFileAnno() {
    QFile file(filegroup.geometry.path);
    if (!file.exists()) {
        return this;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return this;
    }

    QTextStream stream(&file);
    int file_row_number = 0;
    bool find_anno = false;
    int file_start_index = 0;
    int file_end_index = 0;
    int file_cur_index = 0;

    if(mapMetadata.version == OdataVersion::ODATA_1_0) {
        stream.setCodec(QTextCodec::codecForName("gb2312"));
    } else if (mapMetadata.version == OdataVersion::ODATA_2_0 ||
        mapMetadata.version == OdataVersion::ENTITY_1_0) {
        stream.setCodec(QTextCodec::codecForName("utf-8"));
    }
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        file_row_number++;
        if (!find_anno) {
            // printf("find_point %d \r\n", file_row_number);
            // 开始 定位 "P 1000" 的开始位置
            int count = GeometryLineStringTypeCount(line, LayerType::Anno, mapMetadata.version);
            if (count >= 0) {
                file_start_index = file_row_number;
                file_end_index = file_start_index + count;
                find_anno = true;
            }
        } else {
            file_cur_index++;
            if (file_row_number > file_start_index && file_row_number <= file_end_index) {
                // 实际属性转换代码
                if (file_cur_index <= features.size()) {
                    features[file_cur_index - 1].setGeometryPoint(line, file_cur_index, &mapMetadata);
                }
            } else {
                // 无效行，包括属性文件前几行

            }
        }
    }
    file.close();
    return this;
}
