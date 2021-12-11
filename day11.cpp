#!/bin/bash
# This c++ file behaves like a script and running it with ./ will both compile and run
(echo -e '\n\n'; tail +4 $0) | g++ -g -std=c++17 -Wall -Werror -x c++ - && exec ./a.out

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace std::chrono;

using matrix = vector<vector<int>>;

void parse_file(FILE* file, matrix& map)
{
  string line;
  vector<int> row;
  for(unsigned i = 0; i < 10; ++i) {
    char c;
    for(unsigned j = 0; j < 10; ++j) {
      fscanf(file, "%c", &c);
      row.push_back(c-'0');
    }
    fscanf(file, "%c", &c);
    map.push_back(row);
    row.clear();
  }
}

int get(const matrix& map, int x, int y)
{
  if(x < 0 || y < 0 || (unsigned) x >= map.size() || (unsigned) y >= map[0].size())
    return -1;
  return map[x][y];
}

int light_one(matrix& map, int x, int y)
{
  int count = 1;
  map[x][y] = -1;
  for(int i = -1; i < 2; ++i) {
    for(int j = -1; j < 2; ++j) {
      int val = get(map, x+i, y+j);
      if(val != -1)
        map[x+i][y+j] = ++val;
      if(val > 9)
        count += light_one(map, x+i, y+j);
    }
  }
  return count;
}

bool add_one(matrix& map, int& count)
{
  for(unsigned i = 0; i < map.size(); ++i) {
    for(unsigned j = 0; j < map[i].size(); ++j) {
      if(map[i][j] != -1) {
        map[i][j] += 1;
      }
      if(map[i][j] > 9) {
        count += light_one(map, i, j);
      }
    }
  }

  bool simultaneously = true;
  for(unsigned i = 0; i < map.size(); ++i) {
    for(unsigned j = 0; j < map[i].size(); ++j) {
      if(map[i][j] == -1) {
        map[i][j] = 0;
      } else {
        simultaneously = false;
      }
    }
  }
  return simultaneously;
}

int main()
{
  FILE* file;
  file = fopen("input", "r");
  if(file != NULL) {
    matrix map;

    auto start = high_resolution_clock::now();
    parse_file(file, map);
    fclose(file);

    int count = 0;
    unsigned i = 1;
    while(!add_one(map, count)) {
      if(i == 100)
        cout << count << endl;
      ++i;
    }
    cout << i << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << "µs" << endl;

  } else {
    cout << "Couldn't open file" << endl;
  }
}

