#include <QDir>
#include <QFileInfoList>

#include "txtreader.h"

std::vector<DirInfo> TxtReader::loopRootDir(QString path) {
    std::vector<DirInfo> list;
    QFileInfoList dirs = readRootDir(path);
    for(int i = 0; i < dirs.length(); i++) {
        DirInfo dir;
        std::vector<FileInfo> files;

        dir.name = dirs[i].baseName();
        dir.path = dirs[i].absoluteFilePath();
        QFileInfoList nodes = readNodeDir(dir.path);

        for(int j = 0; j < nodes.count(); j++) {
            FileInfo file;
            file.name = nodes[j].baseName();
            file.path = nodes[j].absoluteFilePath();
            files.push_back(file);
        }

        dir.files = files;
        list.push_back(dir);
    }
    return list;
}

QFileInfoList TxtReader::readRootDir(QString path){
    QFileInfoList list;

    QDir dir(path);

    if(!dir.exists()) {
        return list;
    }

    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    list = dir.entryInfoList();

    return list;
}

QFileInfoList TxtReader::readNodeDir(QString path){
    QFileInfoList list;

    QDir dir(path);

    if(!dir.exists()) {
        return list;
    }

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    list = dir.entryInfoList();

    return list;
}
