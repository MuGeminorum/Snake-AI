#include "hamilton.h"
#define DEBUG_MODE true
#define MAX_VERTEX_NUM 20

Hamilton::Hamilton(int screen_size)
{
    N = screen_size;
}

Hamilton::~Hamilton()
{
}

void Hamilton::InitHamilton(vector<QPoint> snake)
{
    // if (snake.size() < N * N - MAX_VERTEX_NUM + 2) // 2 = head + tail
    //     return false;

    mask = CreateMask(snake);
    head = snake.front();
    found = false;
    visited.clear();
    vertex.clear();
    prior.clear();
    G.clear();

    // return true;
}

bool Hamilton::GetPath(QPoint target, vector<QPoint> &qpath)
{
    RegionGrow(target);
    int vCount = vertex.size();

    if (vCount > MAX_VERTEX_NUM) // Algorithm Limitations
        return false;

    G = vector<vector<int>>(vCount, vector<int>(vCount));
    visited.resize(vCount);
    prior.resize(vCount);

    for (int i = 0; i < vCount - 1; i++)
    {
        for (int j = i + 1; j < vCount; j++)
        {
            if (IsNeigbor(vertex[i], vertex[j]))
            {
                G[i][j] = 1;
                G[j][i] = 1;
            }
        }
    }

    if (DEBUG_MODE) // For debug
        PrintMat(G);

    DFS(0, 0, vCount, qpath);

    if (DEBUG_MODE) // For debug
        PrintArr(qpath);

    return found;
}

bool Hamilton::IsNeigbor(QPoint p, QPoint q)
{
    return abs(p.x() - q.x()) + abs(p.y() - q.y()) == 1;
}

vector<vector<Block>> Hamilton::CreateMask(vector<QPoint> snake)
{
    int i, j;
    int snakeLen = snake.size();
    vector<vector<Block>> mask(N + 2);

    for (i = 0; i < N + 2; i++)
        mask[i].resize(N + 2);

    for (i = 0; i < N + 2; i++)
    {
        for (j = 0; j < N + 2; j++)
        {
            mask[i][j].available = !(i == 0 || j == 0 || i == N + 1 || j == N + 1);
            mask[i][j].visited = false;
        }
    }

    for (i = 1; i < snakeLen; i++)
        mask[snake[i].x() + 1][snake[i].y() + 1].available = false;

    return mask;
}

void Hamilton::DFS(int v, int from, int vCount, vector<QPoint> &qpath)
{
    if (visited[v] != 0)
        return;

    bool flag = true;
    visited[v] = 1;
    prior[v] = from;

    if (v == 1)
    {
        for (int i = 0; i < vCount; i++)
            flag = visited[i] && flag;

        if (flag)
        {
            LoadPath(vCount, qpath);
            return;
        }

        visited[v] = 0;
        return;
    }

    for (int j = 0; j < vCount; j++)
    {
        if ((visited[j] == 0) && (G[v][j] == 1))
            DFS(j, v, vCount, qpath);
    }

    visited[v] = 0;
}

void Hamilton::PrintMat(vector<vector<int>> m)
{
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m[0].size(); j++)
            cout << m[i][j] << " ";

        cout << endl;
    }
}

void Hamilton::PrintArr(vector<QPoint> v)
{
    if (v.size() == 0)
        return;

    for (int i = 0; i < v.size() - 1; i++)
        cout << "(" << v[i].x() << ", " << v[i].y() << ")->";

    cout << "(" << v.back().x() << ", " << v.back().y() << ")" << endl;
}

void Hamilton::LoadPath(int vCount, vector<QPoint> &qpath)
{
    int tmp = 0;
    vector<int> path(vCount, 0);

    for (int v = 1; v > 0; v = prior[v])
    {
        path[tmp] = v;
        tmp++;
    }

    path[tmp] = 0;
    qpath.clear();
    found = true;

    for (tmp = vCount - 1; tmp >= 0; tmp--)
        qpath.push_back(vertex[path[tmp]]);
}

void Hamilton::RegionGrow(QPoint seed)
{
    if (seed == head)
    {
        vertex.insert(vertex.begin(), seed);
    }
    else
    {
        vertex.push_back(seed);
    }

    mask[seed.x() + 1][seed.y() + 1].visited = true;

    if (mask[seed.x() + 2][seed.y() + 1].available && !mask[seed.x() + 2][seed.y() + 1].visited)
        RegionGrow(seed + QPoint(1, 0)); // right

    if (mask[seed.x()][seed.y() + 1].available && !mask[seed.x()][seed.y() + 1].visited)
        RegionGrow(seed + QPoint(-1, 0)); // left

    if (mask[seed.x() + 1][seed.y() + 2].available && !mask[seed.x() + 1][seed.y() + 2].visited)
        RegionGrow(seed + QPoint(0, 1)); // up

    if (mask[seed.x() + 1][seed.y()].available && !mask[seed.x() + 1][seed.y()].visited)
        RegionGrow(seed + QPoint(0, -1)); // down
}