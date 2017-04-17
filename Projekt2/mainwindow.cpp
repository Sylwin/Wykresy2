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

    connect(ui->filesBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filesComboBox(int)));
    connect(ui->typesBox, SIGNAL(currentIndexChanged(int)), this, SLOT(typesComboBox(int)));

    filesComboBox(0);
    // configure scroll bars:
    // Since scroll bars only support integer values, we'll set a high default range of -500..500 and
    // divide scroll bar position values by 100 to provide a scroll range -5..5 in floating point
    // axis coordinates. if you want to dynamically grow the range accessible with the scroll bar,
    // just increase the the minimum/maximum values of the scroll bars as needed.
   // double min = *std::min_element(files[0].types[0].values.constBegin(), files[0].types[0].values.constEnd());
   // double max = *std::max_element(files[0].types[0].values.constBegin(), files[0].types[0].values.constEnd());
//    qSort(files[0].types[0].values);
//    double min = files[0].types[0].values[1];
//    double max = files[0].types[0].values.last();
//    qDebug() << min;
//    qDebug() << max;

//    ui->verticalScrollBar->setRange(2170, 2381);
//    ui->verticalScrollBar->setInvertedControls(true);
//    qDebug() << files[0].start;
//    qDebug() << files[0].stop;

    // create connection between axes and scroll bars:
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    connect(ui->plot->xAxis2, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
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
    for( int i = 0; i < 3; i++ )
    {
        QStringList line = files[i].linesList.filter(QRegExp("~A"));
        QStringList typesSplit = line[0].split(QRegExp("\\s+"));
        typesSplit.removeFirst();
        typesSplit.removeFirst();
        typesSplit.removeLast();
        GraphType type[typesSplit.size()];
        QStringList values = files[i].linesList.filter(QRegExp("^ +[0-9].+$"));
        for( int k = 0; k < typesSplit.size(); k++ )
        {
            type[k].typeName = typesSplit[k];

            for(int j = 0; j < values.size(); j++ )
            {
                QStringList valSplitted = values[j].split(QRegExp("\\s+"));
                valSplitted.removeFirst();
                valSplitted.removeFirst();
                valSplitted.removeLast();
                float value = valSplitted[k].toFloat();
                type[k].values.append(value);
            }
          //  qDebug() << files[i].values;

            files[i].types.append(type[k]);
        }
    }
 //   qDebug() << files[2].types[10].typeName;
//    qDebug() << files[2].types[0].values.at(8999);
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
        //    qDebug() << dept;
            float depthVal = dept[0].toFloat();
            files[i].depth.append(depthVal);
        }
 //       qDebug() << files[i].depth[0];
    }
}

void MainWindow::YRange(int index)
{
    int center = files[index].start + ((files[index].stop-files[index].start)/2);
    int odch = files[index].stop-files[index].start;
    ui->plot->yAxis->setRange(center, odch/4, Qt::AlignCenter);
}

void MainWindow::setupPlot(int fileIndex, int typeIndex)
{
    ui->plot->clearGraphs();
    ui->plot->addGraph(ui->plot->yAxis, ui->plot->xAxis2);
    ui->plot->graph()->setPen(QPen(Qt::blue));
    ui->plot->graph(0)->setData(files[fileIndex].depth, files[fileIndex].types[typeIndex].values);
    ui->plot->axisRect()->setupFullAxesBox(true);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // The following plot setup is mostly taken from the plot demos:
    qSort(files[fileIndex].types[typeIndex].values);
    double min = files[fileIndex].types[typeIndex].values[1];
    double max = files[fileIndex].types[typeIndex].values.last();
    qDebug() << min;
    qDebug() << max;
//    ui->verticalScrollBar->setRange(2170, 2381);
//    ui->verticalScrollBar->setInvertedControls(true);
    qDebug() << files[fileIndex].start;
    qDebug() << files[typeIndex].stop;

    int st = files[fileIndex].start;
    int sp = files[fileIndex].stop;

    ui->verticalScrollBar->setRange(-sp, -st);

    ui->plot->yAxis->setRangeReversed(true);
    ui->plot->xAxis2->setVisible(true);
  //  ui->plot->xAxis->setVisible(false);
    ui->plot->xAxis->grid()->setVisible(false);
    ui->plot->yAxis->grid()->setVisible(false);
    ui->plot->xAxis->setLabel("values");
    ui->plot->yAxis->setLabel("depth");

    ui->plot->xAxis->setRange((max+min)/2, max-min, Qt::AlignCenter);
    ui->plot->xAxis2->setRange((max+min)/2, max-min, Qt::AlignCenter);
    //ui->plot->yAxis->setRange(2275,120,Qt::AlignCenter);
//    int center = files[fileIndex].start + ((files[fileIndex].stop-files[fileIndex].start)/2);
//    int odch = files[fileIndex].stop-files[fileIndex].start;
//    ui->plot->yAxis->setRange(center, odch, Qt::AlignCenter);
    YRange(fileIndex);
}

void MainWindow::vertScrollBarChanged(int value)
{
  if (qAbs(ui->plot->yAxis->range().center()+value) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->plot->yAxis->setRange(-value, ui->plot->yAxis->range().size(), Qt::AlignCenter);
    ui->plot->replot();
  }
}

void MainWindow::yAxisChanged(QCPRange range)
{
  ui->verticalScrollBar->setValue(qRound(-range.center())); // adjust position of scroll bar slider
  ui->verticalScrollBar->setPageStep(qRound(range.size())); // adjust size of scroll bar slider
}

void MainWindow::filesComboBox(int fileIndex)//, int typeIndex)
{
    switch(fileIndex)
    {
    case 0:
        ui->typesBox->clear();
        foreach(GraphType type, files[0].types)
        {
            ui->typesBox->addItem(type.typeName);
        };
        setupPlot(0, 0);
   //     ui->plot->replot();
        break;
    case 1:
        ui->typesBox->clear();
        foreach(GraphType type, files[1].types)
        {
            ui->typesBox->addItem(type.typeName);
        };
        setupPlot(1, 0);
//        ui->plot->replot();
        break;
    case 2:
        ui->typesBox->clear();
        foreach(GraphType type, files[2].types)
        {
            ui->typesBox->addItem(type.typeName);
        };
        setupPlot(2, 0);
//        ui->plot->replot();
        break;
    }
}

void MainWindow::typesComboBox(int typeIndex)//, int typeIndex)
{
//    setupPlot(0, typeIndex);
//    if(ui->filesBox->currentText() == "K.LAS")
//    {
//        setupPlot(0,typeIndex);
//    }
//    if(ui->filesBox->currentText() == "SOB.LAS")
//    {
//        setupPlot(1,typeIndex);
//    }
//    if(ui->filesBox->currentText() == "W.LAS")
//    {
//        setupPlot(2,typeIndex);
//    }
}
