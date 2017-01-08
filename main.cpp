#include <QApplication>
#include <QLayout>
#include <QDir>
#include <QListWidget>
#include <QTextEdit>
#include <QShortcut>
#include "wind.h"
#include "txt.h"
#include <QDebug>
#include <QFileSystemModel>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QMainWindow>
#include <QDockWidget>

QDir* dir = new QDir("C:/Users/X230/Documents");
QFileSystemModel *mod = new QFileSystemModel;

Txt::Txt (QString,QWidget*) {

}

void Txt::setTxt(QModelIndex cur,QModelIndex prev) {
    QString filePath = mod->filePath(cur);
    QString type = mod->type(cur);
    qDebug() << type;
    if (type=="txt File") {
        QFile f(filePath);
        if (f.open(QFile::ReadOnly | QFile::Truncate)) {
            QTextStream stream(&f);
            QString fileString = stream.readAll();
    //        qDebug() << fileString;
            this->setText(fileString);
        }
    }

    else {
        this->setText(type+" Selected");
    }
}

FileList::FileList (QWidget*) {

}

void FileList::upDir() {
    dir->cdUp();
    QString newDir = dir->path();
    qDebug() << newDir;
    this->setModel(mod);
    this->setRootIndex(mod->setRootPath(newDir));
}

void FileList::openFile() {
    QModelIndex index = this->currentIndex();
    if (mod->isDir(index)) {
        QString newDir = mod->filePath(index);
        dir->cd(newDir);
        qDebug() << newDir;
        this->setModel(mod);
        this->setRootIndex(mod->setRootPath(newDir));
    }
    else {
        QString filePath = mod->filePath(index);
        qDebug() << "Opening file: "+filePath;
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
}

void FileList::inDir() {
    QModelIndex index = this->currentIndex();
    if (mod->isDir(index)) {
        QString newDir = mod->filePath(index);
        dir->cd(newDir);
        qDebug() << newDir;
        this->setModel(mod);
        this->setRootIndex(mod->setRootPath(newDir));
    }
    else if (mod->type(index)=="txt File") {
        qDebug() << this->parent();
        this->parent()->children();
        qDebug() << "HI";
    }
    else {
        qDebug() << "Not a directory.";
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow* mainWin = new QMainWindow();

    QDockWidget *planDock = new QDockWidget(QObject::tr("Plan"),mainWin);
    planDock->setAllowedAreas(Qt::RightDockWidgetArea);
    QVBoxLayout* vlayout = new QVBoxLayout();
    Txt* planEdit = new Txt("Default",planDock);
    mainWin->addDockWidget(Qt::RightDockWidgetArea,planDock);
    planDock->setWidget(planEdit);

    QWidget* window = new QWidget();
    QHBoxLayout* hlayout = new QHBoxLayout();
    Txt* textEdit = new Txt("Default",window);

    QStringList entries = dir->entryList();
//    LstWid* listWidget = new LstWid(window);
    FileList* listWidget = new FileList(window);
    listWidget->setModel(mod);
    listWidget->setRootIndex(mod->setRootPath(dir->path()));

//    listWidget->addItems(entries);
//    listWidget->setFocus();

    window->setLayout(hlayout);
    hlayout->addWidget(listWidget);
    hlayout->addWidget(textEdit);
    //window->show();

    mainWin->setCentralWidget(window);
    mainWin->show();

    //Shortcuts below.

    QShortcut *enter = new QShortcut(QKeySequence(Qt::Key_Return), window);
    QObject::connect(enter, SIGNAL(activated()), listWidget, SLOT(openFile()));

    QShortcut *rightArrow = new QShortcut(QKeySequence::MoveToNextChar, window);
    QObject::connect(rightArrow, SIGNAL(activated()), listWidget, SLOT(inDir()));

    QShortcut *deleteKey = new QShortcut(QKeySequence(Qt::Key_Backspace), window);
    QObject::connect(deleteKey, SIGNAL(activated()), listWidget, SLOT(upDir()));

    QShortcut *leftArrow = new QShortcut(QKeySequence::MoveToPreviousChar, window);
    QObject::connect(leftArrow, SIGNAL(activated()), listWidget, SLOT(upDir()));

    QShortcut *changeFocus = new QShortcut(QKeySequence::NextChild, window);
    QObject::connect(changeFocus, SIGNAL(activated()), textEdit, SLOT(setFocus()));
    //QObject::connect(changeFocus, SIGNAL(activated()), window->focusNextChild(), SLOT(setFocus()));

    QObject::connect(listWidget->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),textEdit,SLOT(setTxt(QModelIndex,QModelIndex)));

    listWidget->setFocus();

    return a.exec();
}
