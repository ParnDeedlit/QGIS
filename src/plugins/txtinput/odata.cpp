#include "odata.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QTextCodec>

#include <unordered_map>

std::vector<QString> SUFFIXS = {"MS", "SX", "TP", "ZB"};

std::vector<QString> NAMES = {
    "测量控制点",
    "工农业社会文化设施",
    "居民地及附属设施",
    "陆地交通",
    "管线",
    "水域陆地",
    "海底地貌及底质",
    "礁石沉船障碍物",
    "水文",
    "陆地地貌及土质",
    "境界与政区",
    "植被",
    "地磁要素",
    "助航设备及航道",
    "海上区域界线",
    "航空要素",
    "军事区域",
    "注记",
};

std::vector<QString> SHORTNAMES = {
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
};

std::vector<QString> CODES = {
    "110000",
    "120000",
    "130000",
    "140000",
    "150000",
    "160000",
    "170000",
    "180000",
    "190000",
    "200000",
    "210000",
    "220000",
    "230000",
    "240000",
    "250000",
    "260000",
    "270000",
    "280000",
};

ODATA* ODATA::instance = nullptr;
std::vector<File> * ODATA::metadata = nullptr;

ODATA* ODATA::getInstance() {
    if (instance == nullptr) {
        instance = new ODATA();
        metadata = new std::vector<File>;
        initMetaData();
    }
    return instance;
}

void ODATA::initMetaData() {
   int count = NAMES.size();
   for (int i = 0; i < count; i++) {
       if (count == count - 1) {
           File anno_point;
           anno_point.name = SHORTNAMES[i] + "_" + NAMES[i] + "_" + CODES[i] + "_P";
           anno_point.code = CODES[i];
           anno_point.layername = SHORTNAMES[i];

           metadata->push_back(anno_point);
       } else {
           File layer_point;
           layer_point.name = SHORTNAMES[i] + "_" + NAMES[i] + "_" + CODES[i] + "_P";
           layer_point.code = CODES[i];
           layer_point.layername = SHORTNAMES[i];
           File layer_line;
           layer_point.name = SHORTNAMES[i] + "_" + NAMES[i] + "_" + CODES[i] + "_L";
           layer_point.code = CODES[i];
           layer_point.layername = SHORTNAMES[i];
           File layer_area;
           layer_point.name = SHORTNAMES[i] + "_" + NAMES[i] + "_" + CODES[i] + "_A";
           layer_point.code = CODES[i];
           layer_point.layername = SHORTNAMES[i];

           metadata->push_back(layer_point);
           metadata->push_back(layer_point);
           metadata->push_back(layer_point);
       }
   }
}

std::vector<File>* ODATA::getMetaData() {
    return metadata;
}

Map* ODATA::initMap(QFileInfoList list) {
    Map *map = new Map;
    int size = list.count();
    for(int i = 0; i < size; i++) {
        QString suffix = list[i].suffix();
        if (suffix.compare("SMS") == 0) {
            File layer;
            layer.name = list[i].baseName();
            layer.layername = suffix.mid(0, 1).toUpper();
            layer.suffix = list[i].suffix();
            layer.path = list[i].absoluteFilePath();
            layer.prefix = list[i].baseName();
            layer.code = getCodeByShortName(layer.layername);
            MapMataData mapmata;
            mapmata.file = layer;
            map->metadata = mapmata;
            break;
        }
    }
    map->groups = checkValidGroup(list);
    return map;
}

std::vector<FileGroup*> ODATA::checkValidGroup(QFileInfoList list) {
    std::vector<FileGroup*> layergroup;
    int size = list.count();
    std::unordered_map<QString, FileGroup*> groupMap;

    for(int i = 0; i < size; i++) {
        QString suffix = list[i].suffix();
        if (suffix.compare("SMS") == 0) {
        } else {
        //if (suffix.length() > 0 && suffix.mid(0, 1).toUpper().compare("C") == 0) {
            File layer;
            layer.name = list[i].baseName();
            layer.layername = suffix.mid(0, 1).toUpper();
            layer.filetype = suffix.mid(1, 2).toUpper();
            layer.suffix = list[i].suffix();
            layer.path = list[i].absoluteFilePath();
            layer.prefix = list[i].baseName();
            layer.cn_name = getCnnameByShortName(layer.layername);
            layer.code = getCodeByShortName(layer.layername);
            auto it = groupMap.find(layer.layername);
            if ( it != groupMap.end()) {
                if (layer.filetype.compare("MS") == 0) {
                    it->second->matadata = layer;
                } else if (layer.filetype.compare("SX") == 0) {
                    it->second->attrbution = layer;
                } else if (layer.filetype.compare("TP") == 0) {
                    it->second->topylogy = layer;
                } else if (layer.filetype.compare("ZB") == 0) {
                    it->second->geometry = layer;
                }
            } else {
                FileGroup *group = new FileGroup();
                group->layername = layer.layername;
                if (layer.filetype.compare("MS") == 0) {
                    group->matadata = layer;
                } else if (layer.filetype.compare("SX") == 0) {
                    group->attrbution = layer;
                } else if (layer.filetype.compare("TP") == 0) {
                    group->topylogy = layer;
                } else if (layer.filetype.compare("ZB") == 0) {
                    group->geometry = layer;
                }
                groupMap.emplace(layer.layername, group);
            }
        //}
        }
    }

    for (auto it = groupMap.begin(); it != groupMap.end(); it++) {
        layergroup.push_back(it->second);
    }

    return layergroup;
}

QString ODATA::getCodeByShortName(QString shortname) {
    int length = SHORTNAMES.size();
    for(int i = 0; i < length; i++) {
        if (SHORTNAMES[i].toUpper().compare(shortname.toUpper()) == 0) {
            return CODES[i];
        }
    }
    return "999999";
}

QString ODATA::getCnnameByShortName(QString shortname) {
    int length = SHORTNAMES.size();
    for(int i = 0; i < length; i++) {
        if (SHORTNAMES[i].toUpper().compare(shortname.toUpper()) == 0) {
            return NAMES[i];
        }
    }
    return "999999";
}

void ODATA::fixLayer(File *layer) {

}

//--------------------------------------------------Map------------------------------------------------
void ODATA::parseMap(Map *map) {
    parseMapMataData(map);
    parseFileGroups(map);
}

bool ODATA::parseMapMataData(Map *map) {
    QFile file(map->metadata.file.path);
    if (!file.exists()) {
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("gb2312"));
    // stream.setAutoDetectUnicode(true);

    // https://www.toolhelper.cn/EncodeDecode/UnicodeChineseEncodeDecode
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        if(MetadataLineStringMatchReg(line, "椭球长半径")) {
            map->metadata.a = MetadataLineStringToDoubleByReg(line, "椭球长半径");
        } else if (MetadataLineStringMatchReg(line, "椭球扁率")) {
            map->metadata.e = MetadataLineStringToDoubleByReg(line, "椭球扁率");
        } else if (MetadataLineStringMatchReg(line, "大地基准")) {
            map->metadata.datum = MetadataLineStringToStringByReg(line, "大地基准");
        } else if (MetadataLineStringMatchReg(line, "地图投影")) {
            map->metadata.projType = projTypeNameToProjType(MetadataLineStringToStringByReg(line, "地图投影"));
        } else if (MetadataLineStringMatchReg(line, "中央经线")) {
            map->metadata.lon_0 = MetadataLineStringToDoubleByReg(line, "中央经线");
        } else if (MetadataLineStringMatchReg(line, "标准纬线1")) {
            map->metadata.lat_1 = MetadataLineStringToDoubleByReg(line, "标准纬线1");
        } else if (MetadataLineStringMatchReg(line, "标准纬线2")) {
            map->metadata.lat_2 = MetadataLineStringToDoubleByReg(line, "标准纬线2");
        } else if (MetadataLineStringMatchReg(line, "分带方式")) {
            map->metadata.zone_type = MetadataLineStringToStringByReg(line, "分带方式");
        } else if (MetadataLineStringMatchReg(line, "高斯投影带号")) {
            map->metadata.gauss_number = MetadataLineStringToIntByReg(line, "高斯投影带号");
        } else if (MetadataLineStringMatchReg(line, "坐标单位")) {
            map->metadata.projUnit = projUnitNameToProjUnit(MetadataLineStringToStringByReg(line, "坐标单位"));
        } else if (MetadataLineStringMatchReg(line, "坐标放大系数")) {
            map->metadata.scale = MetadataLineStringToDoubleByReg(line, "坐标放大系数");
        } else if (MetadataLineStringMatchReg(line, "相对原点横坐标")) {
            map->metadata.origin_x = MetadataLineStringToDoubleByReg(line, "相对原点横坐标");
        } else if (MetadataLineStringMatchReg(line, "相对原点纵坐标")) {
            map->metadata.origin_y = MetadataLineStringToDoubleByReg(line, "相对原点纵坐标");
        }
    }

    file.close();
    return true;
}

void ODATA::parseFileGroups(Map *map) {
    vector<FileGroup*> *groups = &(map->groups);
    if (nullptr == groups) return;
    int count = groups->size();
    for (int i = 0; i < count; i++) {
        FileGroup* group = (*groups)[i];
        parseFileGroup(group);
    }
}

void ODATA::parseFileGroup(FileGroup *group) {

}

void ODATA::parseFileMetadata(File *meta) {

}

void ODATA::parseFileAttrbution(File *attr) {

}

void ODATA::parseFileGeometry(File *geom) {

}
