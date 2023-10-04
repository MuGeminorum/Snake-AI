#include "astar.h"
#include <math.h>
#define K_COST_1 10
#define K_COST_2 14

Astar::Astar(int screen_size)
{
    N = screen_size;
}

Astar::~Astar()
{
}

void Astar::InitAstar(vector<QPoint> snake)
{
    maze = CreateMaze(snake);
    head = snake[0];
    neck = snake[1];
}

bool Astar::HasPath(QPoint target)
{
    Point start(head.x() + 1, head.y() + 1);
    Point end(target.x() + 1, target.y() + 1);
    return GetPath(start, end, false).size() > 0;
}

bool Astar::GetPath(QPoint target, vector<QPoint> &QPath)
{
    Point start(head.x() + 1, head.y() + 1);
    Point end(target.x() + 1, target.y() + 1);
    list<Point *> path = GetPath(start, end, false);

    if (path.size() <= 0)
        return false;

    QPath.clear();

    for (auto &p : path)
    {
        QPoint node(p->x - 1, p->y - 1);
        QPath.push_back(node);
    }

    if (HasDiag(QPath))
    {
        PrintPath(QPath);
        bool res = SmoothPath(QPath);
        cout << "-----------" << endl;
        PrintPath(QPath);
        cout << "===========" << endl;
    }

    return QPath.size() > 0;
}

void Astar::PrintPath(vector<QPoint> QPath)
{
    for (int i = 0; i < QPath.size(); i++)
    {
        cout << "(" << QPath[i].x() << ", " << QPath[i].y() << ")" << endl;
    }
}

bool Astar::HasDiag(vector<QPoint> QPath)
{
    for (int i = 0; i < QPath.size() - 1; i++)
    {
        if (FindDiag(QPath[i], QPath[i + 1]))
        {
            return true;
        }
    }
    return false;
}

bool Astar::SmoothPath(vector<QPoint> &QPath)
{
    int i = 0;
    QPoint direction = head - neck;

    while (i < QPath.size() - 1)
    {
        if (FindDiag(QPath[i], QPath[i + 1]))
        {
            if (i > 0)
                direction = QPath[i] - QPath[i - 1];

            QPath.insert(QPath.begin() + i + 1, FillDiag(QPath[i], QPath[i + 1], direction));
            i += 2;
        }
        else
        {
            i++;
        }
    }

    return QPath.size() > 0;
}

bool Astar::FindDiag(QPoint p, QPoint q)
{
    return abs((p - q).x()) == 1 && abs((p - q).y()) == 1;
}

QPoint Astar::FillDiag(QPoint p, QPoint q, QPoint d)
{
    QPoint pq(p.x(), q.y());
    QPoint qp(q.x(), p.y());

    if (pq == p + d)
        return pq;

    if (qp == p + d)
        return qp;

    if (maze[pq.x() + 1][pq.y() + 1] == 0 && pq != p - d)
        return pq;

    return qp;
}

vector<vector<int>> Astar::CreateMaze(vector<QPoint> snake)
{
    int i, j;
    int snakeLen = snake.size();
    vector<vector<int>> maze(N + 2);

    for (i = 0; i < N + 2; i++)
        maze[i].resize(N + 2);

    for (i = 0; i < N + 2; i++)
        for (j = 0; j < N + 2; j++)
            maze[i][j] = (int)(i == 0 || j == 0 || i == N + 1 || j == N + 1);

    for (i = 1; i < snakeLen; i++)
        maze[snake[i].x() + 1][snake[i].y() + 1] = 1;

    return maze;
}

int Astar::CalcG(Point *temp_start, Point *point)
{
    int extraG = (abs(point->x - temp_start->x) +
                  abs(point->y - temp_start->y)) == 1
                     ? K_COST_1
                     : K_COST_2;

    int parentG = point->parent == NULL ? 0 : point->parent->G;
    return parentG + extraG;
}

int Astar::CalcH(Point *point, Point *end)
{
    return sqrt(
               (double)(end->x - point->x) * (double)(end->x - point->x) +
               (double)(end->y - point->y) * (double)(end->y - point->y)) *
           K_COST_1;
}

int Astar::CalcF(Point *point)
{
    return point->G + point->H;
}

Point *Astar::GetLeastFpoint()
{
    if (!openList.empty())
    {
        auto resPoint = openList.front();

        for (auto &point : openList)
        {
            if (point->F < resPoint->F)
                resPoint = point;
        }

        return resPoint;
    }

    return NULL;
}

Point *Astar::FindPath(Point &startPoint, Point &endPoint, bool ignoreCorner)
{
    openList.push_back(new Point(startPoint.x, startPoint.y));

    while (!openList.empty())
    {
        auto curPoint = GetLeastFpoint();
        openList.remove(curPoint);
        closeList.push_back(curPoint);
        auto surroundPoints = GetSurroundPoints(curPoint, ignoreCorner);

        for (auto &target : surroundPoints)
        {
            if (!InList(openList, target))
            {
                target->parent = curPoint;
                target->G = CalcG(curPoint, target);
                target->H = CalcH(target, &endPoint);
                target->F = CalcF(target);
                openList.push_back(target);
            }
            else
            {
                int tempG = CalcG(curPoint, target);
                if (tempG < target->G)
                {
                    target->parent = curPoint;
                    target->G = tempG;
                    target->F = CalcF(target);
                }
            }

            Point *resPoint = InList(openList, &endPoint);

            if (resPoint)
                return resPoint;
        }
    }

    return NULL;
}

list<Point *> Astar::GetPath(Point &startPoint, Point &endPoint, bool ignoreCorner) // path includes start & end
{
    Point *result = FindPath(startPoint, endPoint, ignoreCorner);
    list<Point *> path;

    while (result)
    {
        path.push_front(result);
        result = result->parent;
    }

    openList.clear();
    closeList.clear();

    return path;
}

Point *Astar::InList(const list<Point *> &list, const Point *point) const
{
    for (auto p : list)
    {
        if (p->x == point->x && p->y == point->y)
            return p;
    }

    return NULL;
}

bool Astar::CanReach(const Point *point, const Point *target, bool ignoreCorner) const
{
    if (
        target->x < 0 ||
        target->x > maze.size() - 1 ||
        target->y < 0 ||
        target->y > maze[0].size() - 1 ||
        maze[target->x][target->y] == 1 ||
        target->x == point->x && target->y == point->y ||
        InList(closeList, target))
    {
        return false;
    }
    else
    {
        if (abs(point->x - target->x) + abs(point->y - target->y) == 1)
        {
            return true;
        }
        else
        {
            if (maze[point->x][target->y] == 0 && maze[target->x][point->y] == 0)
            {
                return true;
            }
            else
            {
                return ignoreCorner;
            }
        }
    }
}

vector<Point *> Astar::GetSurroundPoints(const Point *point, bool ignoreCorner) const
{
    vector<Point *> surroundPoints;

    for (int x = point->x - 1; x <= point->x + 1; x++)
    {
        for (int y = point->y - 1; y <= point->y + 1; y++)
        {
            Point *newPoint = new Point(x, y);
            if (CanReach(point, newPoint, ignoreCorner))
                surroundPoints.push_back(newPoint); // new Point(x, y)
        }
    }

    return surroundPoints;
}