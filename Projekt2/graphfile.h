#ifndef GRAPHFILE_H
#define GRAPHFILE_H
#include <QVector>
//#include <graphtype.h>

class GraphFile
{
public:
    GraphFile();
    QStringList linesList;
    QVector<QString> types;
    float start;
    float stop;
    float step;
    float empty;
    QVector<float> values;
    QVector<float> depth;
};

#endif // GRAPHFILE_H
