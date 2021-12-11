#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

static void parse_file(FILE* file, vector<pair<int,int>>& from, vector<pair<int,int>>& to)
{
  int fromX, fromY, toX, toY;
  while(fscanf(file, "%d,%d -> %d,%d", &fromX, &fromY, &toX, &toY) != EOF) {
    from.emplace_back(fromX, fromY);
    to.emplace_back(toX, toY);
  }
}

static void init_map(const vector<pair<int,int>>& from, const vector<pair<int,int>>& to, vector<vector<int>>& map)
{
  int maxX = max(
    (*max_element(from.begin(), from.end(), [](auto a, auto b){return a.first < b.first;})).first,
    (*max_element(to.begin(), to.end(), [](auto a, auto b){return a.first < b.first;})).first
  );
  int maxY = max(
    (*max_element(from.begin(), from.end(), [](auto a, auto b){return a.second < b.second;})).second,
    (*max_element(to.begin(), to.end(), [](auto a, auto b){return a.second < b.second;})).second
  );

  for(int i = 0; i <= maxX; ++i) {
    map.emplace_back(maxY+1, 0);
  }
}

static void fill_map1(const vector<pair<int,int>>& from, const vector<pair<int,int>>& to, vector<vector<int>>& map) {
  for(size_t i = 0; i < from.size(); ++i) {
    if(from[i].first == to[i].first || from[i].second == to[i].second) {
      int diffX = to[i].first - from[i].first;
      int diffY = to[i].second - from[i].second;
      int stepX = diffX == 0 ? 0 : diffX/abs(diffX);
      int stepY = diffY == 0 ? 0 : diffY/abs(diffY);

      for(int x = from[i].first, y = from[i].second;
          x != to[i].first || y != to[i].second;
          x += stepX, y += stepY)
        ++map[x][y];
      ++map[to[i].first][to[i].second];
    }
  }
}

static void fill_map2(const vector<pair<int,int>>& from, const vector<pair<int,int>>& to, vector<vector<int>>& map) {
  for(size_t i = 0; i < from.size(); ++i) {
    int diffX = to[i].first - from[i].first;
    int diffY = to[i].second - from[i].second;
    int stepX = diffX == 0 ? 0 : diffX/abs(diffX);
    int stepY = diffY == 0 ? 0 : diffY/abs(diffY);

    for(int x = from[i].first, y = from[i].second;
        x != to[i].first || y != to[i].second;
        x += stepX, y += stepY)
      ++map[x][y];
    ++map[to[i].first][to[i].second];
  }
}

static int count_crossings(vector<vector<int>> map)
{
  return transform_reduce(
    map.begin(),
    map.end(),
    0,
    plus<int>(),
    [](vector<int> v){return count_if(v.begin(), v.end(), [](int e){return e > 1;});}
  );
}

bool day5(long& p1, long& p2)
{
  FILE* file = fopen("input/5", "r");
  if(file != NULL) {
    vector<pair<int,int>> from, to;
    vector<vector<int>> map1, map2;

    parse_file(file, from, to);

    init_map(from, to, map1);

    map2 = map1;

    fill_map1(from, to, map1);
    p1 = count_crossings(map1);

    fill_map2(from, to, map2);
    p2 = count_crossings(map2);

    fclose(file);
  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}
