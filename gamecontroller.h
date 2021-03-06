#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimerEvent>

#include <TileType.h>
#include <direction.h>
#include <tiles.h>

#define BASE_TICK_TIME_MS 1000

enum GameState {
    READY_FOR_START,
    RUNNING,
    STOPPED
};

class GameController : public QObject
{
    Q_OBJECT

public:
    GameController(int fieldSize);
    ~GameController();

    GameState gameState() {
        return state;
    }

    void snakeLeft() { tryChangeDirection(DIRECTION_LEFT); }
    void snakeRight() { tryChangeDirection(DIRECTION_RIGHT); }
    void snakeUp() { tryChangeDirection(DIRECTION_UP); }
    void snakeDown() { tryChangeDirection(DIRECTION_DOWN); }

    void resetGame();
    void startGame();

    void setGameSpeed(int speed);

signals:
    void gameStartEvent();

    void tileUpdateEvent(int x, int y, TileType newType);

    void gameStopEvent(int score);

private slots:
    void timerEvent(QTimerEvent *event);

private:
    const int INITIAL_SNAKE_LENGTH = 3;

    GameState state;
    int timerId;

    int fieldSize;
    tile** field;

    int snakeHeadX;
    int snakeHeadY;
    direction snakeHeadDirection;
    direction newSnakeHeadDirection;

    int snakeLength;

    int gameSpeed = 3;

    tiletype getTileTypeInFront();

    void moveHead(direction headDirection);
    void moveTail();

    void updateGameState(GameState newState);

    void allocateField();
    void prepareFieldForStart();
    void prepareSnake();
    void deleteField();

    void placeFruit();

    void tick();
    void getRandomCoordinates(int *x, int *y);

    void tryChangeDirection(direction newDirection);

    void updateTileAt(int x, int y, tile newTile, TileType tileType);
};

#endif // GAMECONTROLLER_H
