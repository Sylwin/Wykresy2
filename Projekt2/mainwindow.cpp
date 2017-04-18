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

    connect(ui->filesBox, SIGNAL(activated(int)), this, SLOT(filesComboBox(int)));
    connect(ui->typesBox, SIGNAL(activated(int)), this, SLOT(typesComboBox(int)));
    connect(ui->scaleBox, SIGNAL(activated(int)), this, SLOT(scaleComboBox(int)));
    filesComboBox(0);

    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
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
                float depthVal = valSplitted[0].toFloat();
                valSplitted.removeFirst();
                valSplitted.removeLast();
                float value = valSplitted[k].toFloat();
                if( value != files[i].empty )
                {
                type[k].values.append(value);
                type[k].depth.append(depthVal);
                }
            }
            files[i].types.append(type[k]);
        }
    }
}

void MainWindow::YRange(int index)
{
    int center = files[index].start + ((files[index].stop-files[index].start)/2);
    int odch = files[index].stop-files[index].start;
    if( index == 0 )
    {
        ui->plot->yAxis->setRange(center, odch/2, Qt::AlignCenter);
    }
    else if( index == 1 )
    {
        ui->plot->yAxis->setRange(center, odch/6, Qt::AlignCenter);
    }
    else if( index == 2 )
    {
        ui->plot->yAxis->setRange(center, odch/15, Qt::AlignCenter);
    }
}

void MainWindow::setupPlot(int fileIndex, int typeIndex)
{
    ui->plot->clearGraphs();
    ui->plot->addGraph(ui->plot->yAxis, ui->plot->xAxis);
    ui->plot->graph()->setPen(QPen(Qt::blue));
    ui->plot->graph(0)->setData(files[fileIndex].types[typeIndex].depth, files[fileIndex].types[typeIndex].values);
    ui->plot->axisRect()->setupFullAxesBox(true);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    qSort(files[fileIndex].types[typeIndex].values);
    min = files[fileIndex].types[typeIndex].values.first();
    max = files[fileIndex].types[typeIndex].values.last();

    qDebug() << files[fileIndex].empty;
    if( min == files[fileIndex].empty )
    {
        min = files[fileIndex].types[typeIndex].values[1];
    }
    int last = files[fileIndex].types[typeIndex].values.size();
    int i = 1;
    while( max == files[fileIndex].empty )
    {
        max = files[fileIndex].types[typeIndex].values[last-i];
        i++;
    }

    qDebug() << min;
    qDebug() << max;

    qDebug() << files[fileIndex].start;
    qDebug() << files[fileIndex].stop;
    int st = files[fileIndex].start;
    int sp = files[fileIndex].stop;

    ui->verticalScrollBar->setRange(-sp, -st);
    ui->verticalScrollBar->setInvertedAppearance(true);

    ui->plot->yAxis->setRangeReversed(true);
    ui->plot->xAxis->grid()->setVisible(false);
    ui->plot->yAxis->grid()->setVisible(false);
    ui->plot->xAxis->setLabel("values");
    ui->plot->yAxis->setLabel("depth");

    ui->plot->xAxis->setRange((max+min)/2, max-min, Qt::AlignCenter);
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
//    const QStandardItemModel* model = qobject_cast<const QStandardItemModel*>(ui->scaleBox->model());
//    QStandardItem* item = model->item(1);
//    item->setFlags(item->flags() & Qt::ItemIsEnabled);

    if(ui->filesBox->currentIndex() == 0)
    {
        for( int i = 0; i < files[ui->filesBox->currentIndex()].types[typeIndex].values.size(); i++ )
        {
            if(files[ui->filesBox->currentIndex()].types[typeIndex].values[i] < 0)
            {
                const QStandardItemModel* model = qobject_cast<const QStandardItemModel*>(ui->scaleBox->model());
                QStandardItem* item = model->item(1);
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
                break;
            }
        }
        setupPlot(0,typeIndex);
    }
    if(ui->filesBox->currentIndex() == 1)
    {
        for( int i = 0; i < files[ui->filesBox->currentIndex()].types[typeIndex].values.size(); i++ )
        {
            if(files[ui->filesBox->currentIndex()].types[typeIndex].values[i] < 0)
            {
                const QStandardItemModel* model = qobject_cast<const QStandardItemModel*>(ui->scaleBox->model());
                QStandardItem* item = model->item(1);
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
                break;
            }
        }
        setupPlot(1,typeIndex);
    }
    if(ui->filesBox->currentIndex() == 2)
    {
        for( int i = 0; i < files[ui->filesBox->currentIndex()].types[typeIndex].values.size(); i++ )
        {
            if(files[ui->filesBox->currentIndex()].types[typeIndex].values[i] < 0)
            {
                const QStandardItemModel* model = qobject_cast<const QStandardItemModel*>(ui->scaleBox->model());
                QStandardItem* item = model->item(1);
                item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
                break;
            }
        }
        setupPlot(2,typeIndex);
    }
}

void MainWindow::scaleComboBox(int scaleIndex)
{
    switch(scaleIndex)
    {
    case 0:
    {
        ui->plot->xAxis->setScaleType(QCPAxis::stLinear);
      //  ui->plot->yAxis->setScaleType(QCPAxis::stLinear);
        QSharedPointer<QCPAxisTickerFixed> linTicker(new QCPAxisTickerFixed);
        linTicker->setTickStep(((int)max-(int)min)/10);
        ui->plot->xAxis->setTicker(linTicker);
        QSharedPointer<QCPAxisTickerFixed> linTicker2(new QCPAxisTickerFixed);
      //  int st = files[ui->filesBox->currentIndex()].start;
      //  int sp = files[ui->filesBox->currentIndex()].stop;
      //  linTicker2->setTickStep((sp-st)/10);
      //  ui->plot->yAxis->setTicker(linTicker2);
        ui->plot->replot();
        break;
    }
    case 1:
    {
        ui->plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
       // ui->plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
        ui->plot->xAxis->setTicker(logTicker);
     //   ui->plot->yAxis->setTicker(logTicker);
        ui->plot->replot();
        break;
    }
    }
}
