#ifndef GRAPHTYPE_H
#define GRAPHTYPE_H
#include <QVector>

class GraphType
{
public:
    GraphType();
    QString typeName;
    QVector<float> values;
};

#endif // GRAPHTYPE_H
