#include "mainwindow.h"

#include <QGridLayout>
#include <QMenu>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include<QGraphicsView>
#include<QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include"gamefield.h"
#include"islands.h"
#include"cell.h"
#include "islanditem.h"

void MainWindow::drawField(int cellSize, int bridgeSize)
{
    for(int i =0;i<game.width;i++){
        for(int j =0;j<game.height;j++){
            scene->addRect(i*cellSize, j*cellSize, cellSize,cellSize, QPen(), QBrush(Qt::blue));
            if(game.field[i][j].type == 'I'){
                scene->addEllipse(i*cellSize,j*cellSize,cellSize,cellSize,QPen(),QBrush(Qt::yellow));
                Island* island = game.field[i][j].island;
                if (island) {
                    IslandItem* islandItem = new IslandItem(island, cellSize);
                    islandItem->setPos(i * cellSize, j * cellSize);
                    scene->addItem(islandItem);
                    connect(islandItem, &IslandItem::clicked, this, &MainWindow::onIslandClicked);
                    connect(islandItem, &IslandItem::rightClicked, this, &MainWindow::onIslandRightClicked);
                }
            }
            else if (game.field[i][j].type == 'V') {
                int count = game.field[i][j].count;
                if (count == 1) {
                    scene->addRect(
                        i * cellSize + (cellSize - bridgeSize) / 2,
                        j * cellSize,
                        bridgeSize,
                        cellSize,
                        QPen(),
                        QBrush(Qt::black)
                        );
                } else if (count == 2) {
                    int offset = cellSize / 6;
                    // Левая линия
                    scene->addRect(
                        i * cellSize + offset,
                        j * cellSize,
                        bridgeSize,
                        cellSize,
                        QPen(),
                        QBrush(Qt::black)
                        );
                    // Правая линия
                    scene->addRect(
                        i * cellSize + cellSize - bridgeSize - offset,
                        j * cellSize,
                        bridgeSize,
                        cellSize,
                        QPen(),
                        QBrush(Qt::black)
                        );
                }
            }
            else if (game.field[i][j].type == 'H') {
                int count = game.field[i][j].count;
                if (count == 1) {
                    scene->addRect(
                        i * cellSize,
                        j * cellSize + (cellSize - bridgeSize) / 2,
                        cellSize,
                        bridgeSize,
                        QPen(),
                        QBrush(Qt::black)
                        );
                } else if (count == 2) {
                    int offset = cellSize / 6;
                    // Верхняя линия
                    scene->addRect(
                        i * cellSize,
                        j * cellSize + offset,
                        cellSize,
                        bridgeSize,
                        QPen(),
                        QBrush(Qt::black)
                        );
                    // Нижняя линия
                    scene->addRect(
                        i * cellSize,
                        j * cellSize + cellSize - bridgeSize - offset,
                        cellSize,
                        bridgeSize,
                        QPen(),
                        QBrush(Qt::black)
                        );
                }
            }
        }
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    // void hideElement(QWidget element){
    //     element.hide();
    // }
    resize(500, 500);
    QToolBar *toolBar = new QToolBar("Главная панель", this);
    this->addToolBar(toolBar);

    QToolButton *dropDownButton = new QToolButton(this);
    dropDownButton->setText("Игра");
    dropDownButton->setPopupMode(QToolButton::MenuButtonPopup);

    QMenu *menu = new QMenu(this);
    QAction *startNewGameAction = menu->addAction("Начать новую игру");

    dropDownButton->setMenu(menu);

    toolBar->addWidget(dropDownButton);



    setCentralWidget(centralWidget);

    while(!game.startNewGame());


    view->setScene(scene);
    scene->setSceneRect(0, 0, game.width*cellSize, game.height*cellSize);
    layout->addWidget(view);

    drawField(cellSize, bridgeSize);


    connect(startNewGameAction, &QAction::triggered, [=](){
        firstSelectedIsland = nullptr;
        selectedItem = nullptr;
        while(!game.startNewGame());
        scene->clear();
        drawField(cellSize, bridgeSize);
    });
}

void MainWindow::onIslandClicked(Island* clickedIsland, IslandItem* clickedItem){
    if(!firstSelectedIsland){
        firstSelectedIsland = clickedIsland;
        qDebug()<<"First selected";
        selectedItem = clickedItem;

        selectedItem->setBrush(QBrush(Qt::darkYellow));
        selectedItem->update();
        return;
    }

    if(clickedIsland != firstSelectedIsland){
        qDebug()<<"Selected second";
        game.addBridge(firstSelectedIsland, clickedIsland);

        scene->clear();
        drawField(cellSize, bridgeSize);

        if(game.checkWinCondition()){
            qDebug()<<"Win";

            QMessageBox msgBox;
            msgBox.setWindowTitle("Победа!");
            msgBox.setText("Вы успешно построили все мосты!");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.addButton("Новая игра", QMessageBox::AcceptRole);
            msgBox.addButton("Закрыть", QMessageBox::RejectRole);

            int ret = msgBox.exec();
            qDebug()<<ret;
            if (ret == 2) { // Новая игра
                while(!game.startNewGame());
                scene->clear();
                drawField(cellSize, bridgeSize);
            }
        }
    }

    // selectedItem->setBrush(QBrush(Qt::yellow));
    // selectedItem->update();
    selectedItem = nullptr;
    firstSelectedIsland = nullptr;
}

void MainWindow::onIslandRightClicked(Island* island){
    game.removeBridgesConectedTo(island);
    scene->clear();
    drawField(cellSize, bridgeSize);
}

MainWindow::~MainWindow() {}
