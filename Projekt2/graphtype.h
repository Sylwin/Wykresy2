#ifndef GRAPHTYPE_H
#define GRAPHTYPE_H
#include <QVector>

class GraphType
{
public:
    GraphType();
    QString typeName;
    QVector<double> values;
};

#endif // GRAPHTYPE_H
