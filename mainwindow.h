#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTableWidgetItem>

#include <TileType.h>
#include <gamecontroller.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
    GameController *gameController;

    QTableWidgetItem* cellAt(int x, int y);

    void keyPressEvent(QKeyEvent *event);

    void setUpPlayingField();
    void setUpGameController();
};
#endif // MAINWINDOW_H
