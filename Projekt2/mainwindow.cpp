#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scaleBox->addItem("liniowa");
    ui->scaleBox->addItem("logarytmiczna");

    ui->filesBox->addItem("K.LAS");
    ui->filesBox->addItem("SOB.LAS");
    ui->filesBox->addItem("W.LAS");

    readFile();
    start();
    stop();
    step();
    empty();
    types();
    depth();

    foreach(QString type, files[0].types)
    {
        ui->typesBox->addItem(type);
    }

    connect(ui->filesBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filesComboBox(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readFile()
{
    QFile fileK("K.las");
    if(!fileK.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error";
        return;
    }
    while(!fileK.atEnd())
    {
        QString line = fileK.readLine();
        files[0].linesList.append(line);
    }

    QFile fileS("SOB.LAS");
    if(!fileS.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error";
        return;
    }
    while(!fileS.atEnd())
    {
        QString line = fileS.readLine();
        files[1].linesList.append(line);
    }

    QFile fileW("W.LAS");
    if(!fileW.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error";
        return;
    }
    while(!fileW.atEnd())
    {
        QString line = fileW.readLine();
        files[2].linesList.append(line);
    }
}

void MainWindow::start()
{
    for(int i = 0; i < 3; i++ )
    {
        QStringList startLine = files[i].linesList.filter(QRegExp("STRT.M"));
       // qDebug() << startLine[0];
        startLine[0] = startLine[0].replace(QRegExp("\\s+")," ");
        QStringList splitted = startLine[0].split(QRegExp("\\s"));
        QStringList start = splitted.filter(QRegExp("[0-9]+"));
        //qDebug() << start;
        QString st = start[0];
        if(st[st.size()-1] == QChar(':'))
        {
            st = start[0].remove(':');
        }
        files[i].start = st.toFloat();
        //qDebug() << files[i].start;
    }
}

void MainWindow::stop()
{
    for(int i = 0; i < 3; i++ )
    {
        QStringList startLine = files[i].linesList.filter(QRegExp("STOP.M"));
       // qDebug() << startLine[0];
        startLine[0] = startLine[0].replace(QRegExp("\\s+")," ");
        QStringList splitted = startLine[0].split(QRegExp("\\s"));
        QStringList start = splitted.filter(QRegExp("[0-9]+"));
        //qDebug() << start;
        QString st = start[0];
        if(st[st.size()-1] == QChar(':'))
        {
            st = start[0].remove(':');
        }
        files[i].stop = st.toFloat();
        //qDebug() << files[i].stop;
    }
}

void MainWindow::step()
{
    for(int i = 0; i < 3; i++ )
    {
        QStringList startLine = files[i].linesList.filter(QRegExp("STEP.M"));
       // qDebug() << startLine[0];
        startLine[0] = startLine[0].replace(QRegExp("\\s+")," ");
        QStringList splitted = startLine[0].split(QRegExp("\\s"));
        QStringList start = splitted.filter(QRegExp("[0-9]+"));
        //qDebug() << start;
        QString st = start[0];
        if(st[st.size()-1] == QChar(':'))
        {
            st = start[0].remove(':');
        }
        files[i].step = st.toFloat();
       // qDebug() << files[i].step;
    }

}

void MainWindow::empty()
{
    for(int i = 0; i < 3; i++ )
    {
        QStringList startLine = files[i].linesList.filter(QRegExp("NULL"));
       // qDebug() << startLine[0];
        startLine[0] = startLine[0].replace(QRegExp("\\s+")," ");
        QStringList splitted = startLine[0].split(QRegExp("\\s"));
        QStringList start = splitted.filter(QRegExp("[0-9]+"));
        //qDebug() << start;
        QString st = start[0];
        if(st[st.size()-1] == QChar(':'))
        {
            st = start[0].remove(':');
        }
        files[i].empty = st.toFloat();
        //qDebug() << files[i].empty;
    }
}

void MainWindow::types()
{
    for(int i = 0; i < 3; i++ )
    {
        QStringList line = files[i].linesList.filter(QRegExp("~A"));
        QStringList typesSplit = line[0].split(QRegExp("\\s+"));
        typesSplit.removeFirst();
        typesSplit.removeFirst();
        typesSplit.removeLast();
        //GraphType type[typesSplit.size()];
        foreach(QString str, typesSplit)
        {
            files[i].types.append(str);
            //type[j].typeName = str;
            //qDebug() << type[j++].typeName;
        }
//        qDebug() << files[i].types;
    }
}

void MainWindow::depth()
{
    for(int i = 0; i < 3; i++ )
    {
        QStringList values = files[i].linesList.filter(QRegExp("^ +[0-9].+$"));
        //qDebug() << values[0];
        for(int j = 0; j < values.size(); j++ )
        {
            QStringList dept = values[j].split(QRegExp("\\s+"));
            dept.removeFirst();
            dept.removeLast();
          //  qDebug() << dept;
            float depthVal = dept[0].toFloat();
            files[i].depth.append(depthVal);
        }
        for(int k = 0; k < files[i].types.size(); k++ )
        {
            for(int j = 0; j < values.size(); j++ )
            {
                QStringList dept = values[j].split(QRegExp("\\s+"));
                dept.removeFirst();
                dept.removeLast();
                //qDebug() << dept[k+1];
                float value = dept[k+1].toFloat();
                files[i].values.append(value);
            }
            qDebug() << files[i].values;
            qDebug() << "elo";
        }
    }
}

void MainWindow::filesComboBox(int index)
{
    switch(index)
    {
    case 0:
        ui->typesBox->clear();
        foreach(QString type, files[0].types)
        {
            ui->typesBox->addItem(type);
        };
        break;
    case 1:
        ui->typesBox->clear();
        foreach(QString type, files[1].types)
        {
            ui->typesBox->addItem(type);
        };
        break;
    case 2:
        ui->typesBox->clear();
        foreach(QString type, files[2].types)
        {
            ui->typesBox->addItem(type);
        };
        break;
    }
}



