#ifndef TXTREADER_H
#define TXTREADER_H
#include <QString>
#include <QFileInfoList>
#include <stdio.h>

class FileInfo {
public:
    QString name;
    QString path;
    QString type;
    QString suffux;
};

class DirInfo {
public:
    QString name;
    QString path;
    std::vector<FileInfo> files;
};

class TxtReader {
public:
  std::vector<DirInfo> loopRootDir(QString path);
  QFileInfoList readRootDir(QString path);
  QFileInfoList readNodeDir(QString path);
};

#endif // TXTREADER_H
