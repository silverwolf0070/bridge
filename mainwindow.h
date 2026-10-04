#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gamefield.h"
#include "islanditem.h"
#include<QGraphicsView>
#include <QMainWindow>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawField(int cellSize, int bridgeSize);

private:
    GameField game;
    Island* firstSelectedIsland = nullptr;
    QWidget *centralWidget = new QWidget(this);
    QGraphicsView* view = new QGraphicsView(centralWidget);
    QGraphicsScene* scene = new QGraphicsScene(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    int cellSize = 40;
    int bridgeSize = 10;
    IslandItem* selectedItem = nullptr;
private slots:
    void onIslandClicked(Island* clickedIsland, IslandItem* clickedItem);
    void onIslandRightClicked(Island* island);

};
#endif // MAINWINDOW_H
