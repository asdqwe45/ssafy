#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
using namespace std;
struct Coord
{
  int y, x, bang;
};
Coord wormPare[50000];
Coord nextCoord[200][200][4];
int visited[200][200][4] = {0};
int game[200][200];
int directY[] = {-1, 1, 0, 0};
int directX[] = {0, 0, 1, -1};
int n;
enum
{
  UP,
  DOWN,
  RIGHT,
  LEFT
};
enum
{
  NONE,
  LD_TYPE,
  LU_TYPE,
  RU_TYPE,
  RD_TYPE,
  SQ_TYPE
}; // type 1 ~ 5
const int BLACKHOLE = -1;
const int WORMHOLE_S = 6;
const int WORMHOLE_E = 10;
void init()
{
  memset(wormPare, 0, sizeof(Coord) * 50000);
  memset(nextCoord, 0, 200 * 200 * 4 * sizeof(Coord));
  memset(game, 0, 200 * 200 * sizeof(int));
  memset(visited, 0, 200 * 200 * 4 * sizeof(int));
  n = 0;
}
void input()
{
  cin >> n;
  for (int y = 1; y <= n; y++)
  {
    for (int x = 1; x <= n; x++)
    {
      cin >> game[y][x];
    }
  }
}
void setBarrier()
{
  for (int y = 0; y <= n + 1; y++)
  {
    game[y][0] = SQ_TYPE;
    game[y][n + 1] = SQ_TYPE;
  }
  for (int x = 0; x <= n + 1; x++)
  {
    game[0][x] = SQ_TYPE;
    game[n + 1][x] = SQ_TYPE;
  }
}
int isBlock(Coord now)
{
  int type = game[now.y][now.x];
  if (type == LD_TYPE || type == LU_TYPE || type == RD_TYPE || type == RU_TYPE)
    return 1;
  if (type == SQ_TYPE)
    return 1;
  return 0;
}
int isBlackHole(Coord now)
{
  int type = game[now.y][now.x];
  if (type == BLACKHOLE)
    return 1;
  return 0;
}
int isSame(Coord now, Coord next)
{
  if (now.y == next.y && now.x == next.x)
    return 1;
  return 0;
}
void setWormholeTable()
{
  vector<Coord> wormMaker[11];
  for (int y = 1; y <= n; y++)
  {
    for (int x = 1; x <= n; x++)
    {
      if (game[y][x] < WORMHOLE_S || game[y][x] > WORMHOLE_E)
        continue;
      wormMaker[game[y][x]].push_back({y, x});
    }
  }
  for (int i = WORMHOLE_S; i <= WORMHOLE_E; i++)
  {
    if (wormMaker[i].size() == 0)
      continue;
    Coord w1 = wormMaker[i][0];
    Coord w2 = wormMaker[i][1];
    int index1 = w1.y * 200 + w1.x;
    int index2 = w2.y * 200 + w2.x;
    wormPare[index1] = w2;
    wormPare[index2] = w1;
  }
}
int reverseBang(int bang)
{
  if (bang == UP)
    return DOWN;
  if (bang == DOWN)
    return UP;
  if (bang == LEFT)
    return RIGHT;
  if (bang == RIGHT)
    return LEFT;
  return 0;
}
int turnBang(int bang, int nType)
{
  if (nType == LD_TYPE && bang == LEFT)
    return UP;
  if (nType == LD_TYPE && bang == DOWN)
    return RIGHT;
  if (nType == LU_TYPE && bang == UP)
    return RIGHT;
  if (nType == LU_TYPE && bang == LEFT)
    return DOWN;
  if (nType == RU_TYPE && bang == UP)
    return LEFT;
  if (nType == RU_TYPE && bang == RIGHT)
    return DOWN;
  if (nType == RD_TYPE && bang == DOWN)
    return LEFT;
  if (nType == RD_TYPE && bang == RIGHT)
    return UP;
  return -1;
}
// ??? ?????? ?????? next ????????? ????????? ???????????????.
Coord getNextStep(Coord now)
{
  int ny = now.y + directY[now.bang];
  int nx = now.x + directX[now.bang];
  if (ny < 0 || nx < 0 || ny > n + 1 || nx > n + 1)
    return {0, 0, 0};
  int nType = game[ny][nx];
  if (nType == SQ_TYPE)
  {
    return {ny, nx, reverseBang(now.bang)};
  }
  if (nType >= LD_TYPE && nType <= RD_TYPE)
  {
    int ret = turnBang(now.bang, nType);
    if (ret == -1)
      return {ny, nx, reverseBang(now.bang)};
    return {ny, nx, ret};
  }
  if (nType >= WORMHOLE_S && nType <= WORMHOLE_E)
  {
    int index = ny * 200 + nx;
    return {wormPare[index].y, wormPare[index].x, now.bang};
  }
  return {ny, nx, now.bang};
}
int shot(Coord now)
{
  int turn = 0;
  Coord start = now;
  // ??????????????? ??? ????????? ????????????.
  while (1)
  {
    Coord next = getNextStep(now);
    // ????????? ?????????, ???????????? ?????? ??? x2 + 1 ??? ????????? ??????.
    if (reverseBang(now.bang) == next.bang)
      return turn * 2 + 1;
    if (isBlackHole(next))
      return turn; // ?????????????????? ??????
    if (isSame(next, start))
      return turn; // ??? ?????????????????? ??????
    if (isBlock(next))
    {
      turn += 1;
      //[?????????] ????????? ??? ?????? ????????? ??????????????? ????????? ??????
      if (visited[next.y][next.x][next.bang] > turn)
        return -1;
      visited[next.y][next.x][next.bang] = turn;
    }

    now = next;
  }
  return turn;
}
int main()
{
  // freopen_s(new FILE*, "input.txt", "r", stdin);
  int tcCnt = 0;
  cin >> tcCnt;
  for (int tc = 1; tc <= tcCnt; tc++)
  {
    init();
    input();
    setBarrier();
    setWormholeTable();
    int maxi = -21e8;
    // ????????? ??? ?????? ?????? ???????????? shot??? ????????? ??? ???????????????.
    for (int y = 1; y <= n; y++)
    {
      for (int x = 1; x <= n; x++)
      {
        for (int bang = 0; bang < 4; bang++)
        {
          if (game[y][x] != 0)
            continue;
          int ret = shot({y, x, bang});
          maxi = max(maxi, ret);
        }
      }
    }
    cout << "#" << tc << " " << maxi << "\n";
  }
  return 0;
}