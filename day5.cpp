#!/bin/bash
# This c++ file behaves like a script and running it with ./ will both compile and run
(echo -e '\n\n'; tail +4 $0) | g++ -std=c++17 -Wall -Werror -O3 -x c++ - && exec time -p ./a.out

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

void parse_file(FILE* file, vector<pair<int,int>>& from, vector<pair<int,int>>& to)
{
  int fromX, fromY, toX, toY;
  while(fscanf(file, "%d,%d -> %d,%d", &fromX, &fromY, &toX, &toY) != EOF) {
    from.emplace_back(fromX, fromY);
    to.emplace_back(toX, toY);
  }
}

void init_map(const vector<pair<int,int>>& from, const vector<pair<int,int>>& to, vector<vector<int>>& map)
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

void fill_map1(const vector<pair<int,int>>& from, const vector<pair<int,int>>& to, vector<vector<int>>& map) {
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

void fill_map2(const vector<pair<int,int>>& from, const vector<pair<int,int>>& to, vector<vector<int>>& map) {
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

int count_crossings(vector<vector<int>> map)
{
  return transform_reduce(
    map.begin(),
    map.end(),
    0,
    plus<int>(),
    [](vector<int> v){return count_if(v.begin(), v.end(), [](int e){return e > 1;});}
  );
}

int main()
{
  FILE* file = fopen("input", "r");
  if(file != NULL) {
    vector<pair<int,int>> from, to;
    vector<vector<int>> map1, map2;

    parse_file(file, from, to);

    init_map(from, to, map1);

    map2 = map1;

    fill_map1(from, to, map1);
    fill_map2(from, to, map2);

    cout << "Part 1: " << count_crossings(map1) << endl;
    cout << "Part 2: " << count_crossings(map2) << endl;

    fclose(file);

  } else {
    cout << "Couldn't open file" << endl;
  }
}
