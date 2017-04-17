#ifndef GRAPHFILE_H
#define GRAPHFILE_H
#include <QVector>
#include <graphtype.h>

class GraphFile
{
public:
    GraphFile();
    QStringList linesList;
    QVector<GraphType> types;
    float start;
    float stop;
    float step;
    float empty;
//    QVector<float> values;
    QVector<double> depth;
};

#endif // GRAPHFILE_H
