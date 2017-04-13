#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <graphfile.h>
#include <QVector>
#include <QRegExp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void readFile();
    GraphFile files[3];
    QStringList linesListK;
    QStringList linesListS;
    QStringList linesListW;
    void start();
    void stop();
    void step();
    void empty();
    void types();
    void depth();
    //static QStringList typesSplit;
    //GraphType type[typesSplit.size()];

private:
    Ui::MainWindow *ui;

public slots:
    void filesComboBox(int index);
};

#endif // MAINWINDOW_H
