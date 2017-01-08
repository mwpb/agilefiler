#ifndef WIND_H
#define WIND_H

#include "qlistwidget.h"
#include <QFileSystemModel>

class FileList : public QListView {
    Q_OBJECT
public:
    explicit FileList (QWidget *parent = 0);
public slots:
    void upDir();
    void inDir();
    void openFile();
};

#endif // WIND_H
