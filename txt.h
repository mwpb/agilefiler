#ifndef TXT_H
#define TXT_H

#include "qtextedit.h"
#include <QListWidget>

class Txt : public QTextEdit {
    Q_OBJECT
public:
    explicit Txt (QString text, QWidget *parent = 0);
public slots:
    void setTxt (QModelIndex cur, QModelIndex prev);
};

#endif // TXT_H
