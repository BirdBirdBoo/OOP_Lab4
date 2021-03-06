#include "gamecontroller.h"

#define TILE_NONE 0

GameController::GameController(int fieldSize)
{
    this->fieldSize = fieldSize;
    allocateField();
}

GameController::~GameController()
{
    deleteField();
}

inline void GameController::updateTileAt(int x, int y, tile newTile, TileType tileType) {
    field[x][y] = newTile;
    emit tileUpdateEvent(x, y, tileType);
}

void GameController::resetGame()
{
    updateGameState(READY_FOR_START);
}

void GameController::startGame()
{
    if (gameState() != READY_FOR_START)
        return;

    updateGameState(RUNNING);
}

void GameController::tick() {
    tiletype type = getTileTypeInFront();

    if (type == WALL_TILE || type == SNAKE_TILE) {
        updateGameState(STOPPED);
        return;
    }

    if (type == FRUIT_TILE) {
        placeFruit();
        ++snakeLength;
    } else {
        moveTail();
    }
    moveHead(newSnakeHeadDirection);

    snakeHeadDirection = newSnakeHeadDirection;
}

void GameController::getRandomCoordinates(int *x, int *y)
{
    *x = arc4random() % fieldSize;
    *y = arc4random() % fieldSize;
}

void GameController::moveHead(const direction headDirection)
{
    int px = snakeHeadX, py = snakeHeadY;
    moveCoordinates(&snakeHeadX, &snakeHeadY, headDirection);

    updateTileAt(px, py, convertHeadToBodyTile(field[px][py]), SNAKE_BODY);
    updateTileAt(snakeHeadX, snakeHeadY, createSnakeHeadTile(oppositeTo(headDirection)), SNAKE_HEAD);
}

void GameController::moveTail()
{
    int tailX = snakeHeadX, tailY = snakeHeadY;
    tile tile = field[tailX][tailY];
    int px = 0, py = 0;
    while (!isSnakeTail(tile)) {
        px = tailX;
        py = tailY;

        direction dir = getDirection(tile);
        moveCoordinates(&tailX, &tailY, dir);
        tile = field[tailX][tailY];
    }
    updateTileAt(tailX, tailY, EMPTY_TILE, EMPTY);
    updateTileAt(px, py, createSnakeTailTile(), SNAKE_TAIL);
}

void GameController::updateGameState(GameState newState)
{
    this->state = newState;
    switch (newState) {
    case STOPPED:
        killTimer(timerId);
        emit gameStopEvent(snakeLength);
        break;
    case RUNNING:
        timerId = startTimer(BASE_TICK_TIME_MS / gameSpeed);
        emit gameStartEvent();
    case READY_FOR_START:
        prepareFieldForStart();
        break;
    }
}

void GameController::allocateField()
{
    this->field = new tile*[fieldSize];
    for (int i = 0; i < fieldSize; ++i) {
        field[i] = new tile[fieldSize];
    }
}

void GameController::prepareFieldForStart()
{
    for (int i = 0; i < fieldSize; ++i) {
        for (int j = 0; j < fieldSize; ++j) {
            updateTileAt(i, j, TILE_NONE, EMPTY);
        }
    }

    prepareSnake();
    placeFruit();
}

void GameController::prepareSnake()
{
    snakeLength = INITIAL_SNAKE_LENGTH;

    snakeHeadY = snakeHeadX = fieldSize / 2;
    updateTileAt(snakeHeadX, snakeHeadY, createSnakeHeadTile(DIRECTION_LEFT), SNAKE_HEAD);

    newSnakeHeadDirection = snakeHeadDirection = DIRECTION_RIGHT;

    for (int dx = 1; dx < INITIAL_SNAKE_LENGTH - 1; ++dx) {
        const int bodyX = snakeHeadX - dx;
        updateTileAt(bodyX, snakeHeadY, createSnakeBodyTile(DIRECTION_LEFT), SNAKE_BODY);
    }

    const int tailX = snakeHeadX - INITIAL_SNAKE_LENGTH + 1;
    updateTileAt(tailX, snakeHeadY, createSnakeTailTile(), SNAKE_TAIL);
}

void GameController::timerEvent(QTimerEvent *event) {
    if (event->timerId() != this->timerId)
        return;
    tick();
}

void GameController::setGameSpeed(int speed)
{
    if (gameState() != RUNNING) {
        this->gameSpeed = speed;
    }
}

tiletype GameController::getTileTypeInFront()
{
    int x = snakeHeadX, y = snakeHeadY;
    moveCoordinates(&x, &y, newSnakeHeadDirection);

    if (x < 0 || x >= fieldSize)
        return WALL_TILE;
    if (y < 0 || y >= fieldSize)
        return WALL_TILE;

    return getTileType(field[x][y]);
}

void GameController::deleteField()
{
    for (int i = 0; i < fieldSize; ++i) {
        delete[] field[i];
    }
    delete[] field;
}

void GameController::placeFruit()
{
    int x, y;
    do {
        getRandomCoordinates(&x, &y);
    } while(field[x][y] != EMPTY_TILE);
    updateTileAt(x, y, createFruitTile(), FRUIT);
}

void GameController::tryChangeDirection(direction newDirection)
{
    if (gameState() != RUNNING)
        return;

    if (!isOppositeTo(snakeHeadDirection, newDirection)) {
        newSnakeHeadDirection = newDirection;
    }
}
