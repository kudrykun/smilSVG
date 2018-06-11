#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QGraphicsRectItem>

class Document : public QGraphicsItem
{
    Q_OBJECT
public:
    Document();
private:
    QColor background = QColor(255,255,255);
    QColor border = QColor(160,160,160);
};

#endif // DOCUMENT_H
