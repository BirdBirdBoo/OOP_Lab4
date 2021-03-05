#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define PLAYING_FIELD_SIZE_TILES 16
#define PLAYING_FIELD_TILE_SIZE 32

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setUpPlayingField();
}

MainWindow::~MainWindow()
{
    delete ui;
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
}

