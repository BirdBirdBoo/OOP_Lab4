#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define PLAYING_FIELD_SIZE_TILES 16
#define PLAYING_FIELD_TILE_SIZE 40

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setUpPlayingField();
    setUpGameController();

    gameController->startGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTileUpdate(int x, int y, TileType type)
{
    auto cell = cellAt(x, y);
    switch (type) {
    case SNAKE_HEAD:
        cell->setText("H");
        break;
    case SNAKE_BODY:
        cell->setText("B");
        break;
    case SNAKE_TAIL:
        cell->setText("T");
        break;
    case FRUIT:
        cell->setText("F");
        break;
    case EMPTY:
        cell->setText(".");
        break;
    }

}

void MainWindow::onGameStopped(int score)
{
    QMessageBox gameLost(this);
    gameLost.setWindowTitle("You lost");
    gameLost.setText(tr("Score: %1").arg(score));
    gameLost.setStandardButtons(QMessageBox::Retry);

    auto button = gameLost.exec();
    gameController->resetGame();

    if (button == QMessageBox::Retry) {
        gameController->startGame();
    }
}

QTableWidgetItem *MainWindow::cellAt(int x, int y)
{
    QTableWidgetItem *item = ui->tableWidget->item(y, x);
    if (item == nullptr) {
        item = new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(y, x, item);
    }
    return item;
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:
        gameController->snakeUp();
        keyEvent->accept();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        gameController->snakeDown();
        keyEvent->accept();
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        gameController->snakeLeft();
        keyEvent->accept();
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        keyEvent->accept();
        gameController->snakeRight();
        break;
    }
}

void MainWindow::setUpPlayingField()
{
    ui->tableWidget->setColumnCount(PLAYING_FIELD_SIZE_TILES);
    ui->tableWidget->setRowCount(PLAYING_FIELD_SIZE_TILES);

    for (int i = 0; i < PLAYING_FIELD_SIZE_TILES; ++i) {
        ui->tableWidget->setColumnWidth(i, PLAYING_FIELD_TILE_SIZE);
        ui->tableWidget->setRowHeight(i, PLAYING_FIELD_TILE_SIZE);
    }

    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);

    QSize size = this->size();
#ifdef _WIN32
    size.setWidth(PLAYING_FIELD_SIZE_TILES * PLAYING_FIELD_TILE_SIZE + 2 * 9 + 2);
    size.setHeight(PLAYING_FIELD_SIZE_TILES * PLAYING_FIELD_TILE_SIZE + 2 * 9 + ui->menubar->height() + 4);
#elif TARGET_OS_MAC
    size.setWidth(PLAYING_FIELD_SIZE_TILES * PLAYING_FIELD_TILE_SIZE + 2 * 9);
    size.setHeight(PLAYING_FIELD_SIZE_TILES * PLAYING_FIELD_TILE_SIZE + 2 * 9 + ui->menubar->height());
#endif
    this->resize(size);
    this->setMaximumSize(size);
    this->setMinimumSize(size);
}

void MainWindow::setUpGameController()
{
    // focus on window to receive key events
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    gameController = new GameController(PLAYING_FIELD_SIZE_TILES);

    connect(gameController, SIGNAL(tileUpdateEvent(int,int,TileType)), this, SLOT(onTileUpdate(int,int,TileType)));
    connect(gameController, SIGNAL(gameStopEvent(int)), this, SLOT(onGameStopped(int)));

    gameController->resetGame();
    gameController->setGameSpeed(1);
}

