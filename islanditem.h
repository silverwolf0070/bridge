#ifndef ISLANDITEM_H
#define ISLANDITEM_H

#include <QBrush>
#include <QGraphicsObject>
#include"islands.h"

class IslandItem : public QGraphicsObject
{
    Q_OBJECT
public:
    IslandItem(Island* island, int cellSize, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    void setBrush(QBrush brush);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Island* island;
    int cellSize;
    QBrush brush = QBrush(Qt::yellow);

signals:
    void clicked(Island* island, IslandItem* clickedItem);
    void rightClicked(Island* island);
};

#endif // ISLANDITEM_H
