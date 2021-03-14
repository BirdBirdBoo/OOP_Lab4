#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QRgb>

#include <TileType.h>
#include <gamecontroller.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct GamePalette {
    unsigned int backgroundColor;
    unsigned int snakeColor;
    unsigned int wallColor;
    unsigned int fruitColor;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTileUpdate(int x, int y, TileType type);
    void onGameStopped(int score);

private:
    Ui::MainWindow *ui;
    GameController *gameController = nullptr;
    GamePalette *gamePalette = nullptr;

    QBrush *backgroundBrush = nullptr;
    QBrush *snakeBrush = nullptr;
    QBrush *wallBrush = nullptr;
    QBrush *fruitBrush = nullptr;

    QTableWidgetItem* cellAt(int x, int y);

    void keyPressEvent(QKeyEvent *event);

    void setUpPlayingField();
    void setUpGameController();

    void updatePalette(GamePalette *newPalette);
    QBrush *createBrush(unsigned int rgba);
    void deleteBrushes();
    void deleteBrush(QBrush **brush);
};
#endif // MAINWINDOW_H
