#include "islanditem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

IslandItem::IslandItem(Island* island, int cellSize, QGraphicsItem* parent) : QGraphicsObject(parent), island(island), cellSize(cellSize)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
}

QRectF IslandItem::boundingRect() const
{
    return QRectF(0, 0, cellSize, cellSize);
}

void IslandItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // Остров — жёлтый круг
    painter->setBrush(brush);
    painter->drawEllipse(0, 0, cellSize, cellSize);

    // Число мостов
    painter->setPen(Qt::black);
    QFont font = painter->font();
    font.setPointSize(cellSize / 3);
    painter->setFont(font);

    QString text = QString::number(island->required);
    QRectF textRect = painter->boundingRect(QRectF(0, 0, cellSize, cellSize), Qt::AlignCenter, text);
    painter->drawText(textRect, Qt::AlignCenter, text);
}

void IslandItem::setBrush(QBrush _brush){
    brush = _brush;
}

void IslandItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button()==Qt::LeftButton){
        emit clicked(island, this);
    }
    else if(event->button() == Qt::RightButton){
        emit rightClicked(island);
    }
}
