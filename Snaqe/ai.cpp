#include "ai.h"
#include <math.h>
#include <unordered_map>
#include <map>
#include <functional>
#define DEBUG_MODE true

AI::AI(int screen_size)
{
    ResetAI();
    N = screen_size;
    astar = new Astar(screen_size);
    hamilton = new Hamilton(screen_size);
}

AI::~AI()
{
}

QPoint AI::GetNextStep(vector<QPoint> snake, vector<QPoint> predict, int step)
{
    if (!ValidPath(predict))
    {
        printf_s("Invalid path!\n");
    }
    QPoint nextStep = predict[step] - snake[0];
    if (!ValidStep(nextStep))
    {
        printf_s("(%d, %d)\n", nextStep.x(), nextStep.y());
    }
    return nextStep;
}

QPoint AI::MakeDecision(vector<QPoint> snake, QPoint food)
{
    // try to find food by Hamilton
    if (FindFoodByHamilton(snake, food))
    {
        // if found by Hamilton, update nextstep and predicted path by the found path
        return GetNextStep(snake, predict, step);
    }

    if (PathNotUsedUp(HAMILTON_FOOD))
    {
        return GetNextStep(snake, predict, step);
    }

    // if the predicted path is used up, try to find food by A*
    if (FindFood(snake, food))
    {
        // if found by A*, update nextstep and predicted path by the found path
        return GetNextStep(snake, predict, step);
    }

    if (PathNotUsedUp(ASTAR_FOOD))
    {
        return GetNextStep(snake, predict, step);
    }

    // if the predicted path is used up, try to find tail by Hamilton
    if (FindTailByHamilton(snake))
    {
        // if tail found by Hamilton, update nextstep and predicted path by the found path
        return GetNextStep(snake, predict, step);
    }

    if (PathNotUsedUp(HAMILTON_TAIL))
    {
        return GetNextStep(snake, predict, step);
    }

    // if the predicted path is used up, try to find tail by A*
    if (FindTail(snake))
    {
        // if tail found by A*, update nextstep and predicted path by the found path
        return GetNextStep(snake, predict, step);
    }

    if (PathNotUsedUp(ASTAR_TAIL))
    {
        return GetNextStep(snake, predict, step);
    }
    else
    {
        // if all above not happened
        return CanGo(snake, food);
    }
}

bool AI::PathNotUsedUp(uint8 mode)
{
    if (typath <= mode && step < predict.size() - 1)
    {
        step++;
        return true;
    }

    return false;
}

bool AI::DeadStep(QPoint p, vector<QPoint> snake)
{
    if (p.x() >= N || p.y() >= N || p.x() < 0 || p.y() < 0)
        return true;

    for (auto i : snake)
        if (i.x() == p.x() && i.y() == p.y())
            return true;

    return false;
}

QPoint AI::CanGo(vector<QPoint> snake, QPoint food)
{
    int w1 = 1; // away from food
    int w2 = 1; // away from tail
    int w3 = 1; // go straight
    QPoint head = snake.front();
    QPoint towards = head - snake[1];
    QPoint tail = snake.back();

    // w1 * food + w2 * tail + w3 * direction
    int weightUp = w1 * (head.y() - food.y()) + w2 * (head.y() - tail.y()) + w3 * int(towards == QPoint(0, 1));
    int weightDown = -weightUp + w3 * int(towards == QPoint(0, -1));
    int weightRight = w1 * (head.x() - food.x()) + w2 * (head.x() - tail.x()) + w3 * int(towards == QPoint(1, 0));
    int weightLeft = -weightRight + w3 * int(towards == QPoint(-1, 0));

    std::unordered_map<int, QPoint> intToPointMap;

    // add mapping
    intToPointMap[weightUp] = QPoint(0, 1);
    intToPointMap[weightDown] = QPoint(0, -1);
    intToPointMap[weightRight] = QPoint(1, 0);
    intToPointMap[weightLeft] = QPoint(-1, 0);

    // use std::map to order the mapping by int
    std::map<int, QPoint, std::greater<int>> sortedMap(intToPointMap.begin(), intToPointMap.end());

    // judge if it is a dead step one after another ordered by weight
    for (const auto &pair : sortedMap)
    {
        QPoint pred = head + pair.second;
        if (!DeadStep(pred, snake))
        {
            return pair.second;
        }
    }

    return towards; // go the hell
}

QPoint AI::NextStep(QPoint head)
{
    QPoint predStep = predict[step];
    QPoint nextStep = predStep - head;
    if (!ValidStep(nextStep))
    {
        printf_s("(%d, %d)\n", nextStep.x(), nextStep.y());
    }
    return nextStep;
}

void AI::ResetAI()
{
    typath = EMPTY_PATH;
    step = 1;
    predict.clear();
    predict.resize(1);
}

bool AI::ValidStep(QPoint step)
{
    if (!DEBUG_MODE)
        return true;

    return abs(step.x()) + abs(step.y()) == 1;
}

bool AI::ValidPath(vector<QPoint> path)
{
    if (!DEBUG_MODE)
        return true;

    int pSize = path.size();
    bool next = true;

    for (int i = 0; i < pSize - 1; i++)
    {
        next = next && IsNeighbor(path[i], path[i + 1]);
        if (!next)
        {
            return false;
        }
    }

    return next && !HasRepeated(path);
}

bool AI::HasRepeated(vector<QPoint> path)
{
    vector<int> v;
    for (int i = 0; i < path.size(); i++)
    {
        v.push_back(path[i].x() * N + path[i].y());
    }
    set<int> st(v.begin(), v.end());
    return v.size() > st.size();
}

bool AI::IsNeighbor(QPoint p, QPoint q)
{
    return abs((p - q).x()) + abs((p - q).y()) == 1;
}

vector<QPoint> AI::VirtualSnake(vector<QPoint> path, vector<QPoint> snake)
{
    int snakeLen = snake.size();
    vector<QPoint> vSnake, concat;
    concat.assign(path.rbegin(), path.rend());
    concat.insert(concat.end(), snake.begin() + 1, snake.end());

    for (auto it = concat.begin(); it != concat.end(); ++it)
    {
        vSnake.push_back(*it);
        if (vSnake.size() >= snakeLen + 1)
            break;
    }

    return vSnake;
}

bool AI::FindFood(vector<QPoint> snake, QPoint food)
{
    vector<QPoint> path;
    astar->InitAstar(snake);
    if (astar->GetPath(food, path) && CanEat(snake, path))
    {
        typath = ASTAR_FOOD;
        return true;
    }

    return false;
}

bool AI::CanEat(vector<QPoint> snake, vector<QPoint> path)
{
    vector<QPoint> vSnake = VirtualSnake(path, snake);
    QPoint vSnakeTail = vSnake.back();
    vSnake.pop_back();
    astar->InitAstar(vSnake);
    if (astar->HasPath(vSnakeTail))
    {
        LoadSteps(path);
        return true;
    }

    return false;
}

bool AI::FindFoodByHamilton(vector<QPoint> snake, QPoint food)
{
    vector<QPoint> path;
    astar->InitAstar(snake);
    hamilton->InitHamilton(snake);

    if (astar->HasPath(food) && hamilton->GetPath(food, path) && CanEat(snake, path))
    {
        typath = HAMILTON_FOOD;
        return true;
    }

    return false;
}

int AI::LoadSteps(vector<QPoint> path)
{
    int size = path.size();
    step = 1;
    predict.clear();
    predict.resize(size);
    predict.assign(path.begin(), path.end());
    return size;
}

bool AI::FindTail(vector<QPoint> snake)
{
    vector<QPoint> path, vSnake;
    QPoint tail = snake.back();
    vSnake.assign(snake.begin(), snake.end() - 1);

    astar->InitAstar(vSnake);
    if (astar->GetPath(tail, path))
    {
        LoadSteps(path);
        typath = ASTAR_TAIL;
        return true;
    }

    return false;
}

bool AI::FindTailByHamilton(vector<QPoint> snake)
{
    vector<QPoint> path, vSnake;
    QPoint tail = snake.back();

    vSnake.assign(snake.begin(), snake.end() - 1);
    hamilton->InitHamilton(vSnake);

    if (hamilton->GetPath(tail, path))
    {
        LoadSteps(path);
        typath = HAMILTON_TAIL;
        return true;
    }

    return false;
}