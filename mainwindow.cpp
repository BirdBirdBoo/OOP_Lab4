#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define PLAYING_FIELD_SIZE_TILES 16
#define PLAYING_FIELD_TILE_SIZE 40

GamePalette candyPalette = {
    0xfff0e4d7,
    0xff9fd8ef,
    0xfff5c0c0,
    0xffff7171,
};

GamePalette desertPalette = {
    0xff4a3933,
    0xfff0a500,
    0xffe6d5b8,
    0xffe45826,
};

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
    delete gamePalette;

    deleteBrushes();
}

void MainWindow::deleteBrush(QBrush **brush) {
    if (*brush != nullptr) {
        delete  *brush;
        *brush = nullptr;
    }
}

void MainWindow::deleteBrushes() {
    deleteBrush(&backgroundBrush);
    deleteBrush(&wallBrush);
    deleteBrush(&fruitBrush);
    deleteBrush(&snakeBrush);
}

void MainWindow::onTileUpdate(int x, int y, TileType type)
{
    auto cell = cellAt(x, y);
    switch (type) {
    case SNAKE_BODY:
        cell->setBackground(*snakeBrush);
        break;
    case FRUIT:
        cell->setBackground(*fruitBrush);
        break;
    case EMPTY:
        cell->setBackground(*backgroundBrush);
        break;
    case WALL:
        cell->setBackground(*wallBrush);
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
    size.setWidth(PLAYING_FIELD_SIZE_TILES * PLAYING_FIELD_TILE_SIZE);
    size.setHeight(PLAYING_FIELD_SIZE_TILES * PLAYING_FIELD_TILE_SIZE + ui->menubar->height());
#elif TARGET_OS_MAC
    size.setWidth(PLAYING_FIELD_SIZE_TILES * (PLAYING_FIELD_TILE_SIZE + 0) + 2 * 13);
    size.setHeight(PLAYING_FIELD_SIZE_TILES * (PLAYING_FIELD_TILE_SIZE + 0) + 2 * 13);
#endif
    this->resize(size);
    this->setMaximumSize(size);
    this->setMinimumSize(size);

    updatePalette(&desertPalette);
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
    gameController->setGameSpeed(3);
}

QBrush *MainWindow::createBrush(unsigned int rgba) {
    return new QBrush(QColor::fromRgba(rgba));
}

void MainWindow::updatePalette(GamePalette *newPalette)
{
    gamePalette = newPalette;

    deleteBrushes();

    backgroundBrush = createBrush(gamePalette->backgroundColor);
    wallBrush = createBrush(gamePalette->wallColor);
    snakeBrush = createBrush(gamePalette->snakeColor);
    fruitBrush = createBrush(gamePalette->fruitColor);

    if (gameController != nullptr) {
        gameController->forceFieldRedraw();
    }
}

