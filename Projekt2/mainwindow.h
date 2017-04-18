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
#include <qcustomplot.h>

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
    void setupPlot(int fileIndex, int typeIndex);
    double min;
    double max;
private:
    Ui::MainWindow *ui;

public slots:
    void YRange(int index);
    void filesComboBox(int fileIndex);
    void typesComboBox(int typeIndex);
    void scaleComboBox(int scaleIndex);
    void vertScrollBarChanged(int value);
    void yAxisChanged(QCPRange range);
};

#endif // MAINWINDOW_H
