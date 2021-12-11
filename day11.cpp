#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <stdio.h>
#include <cassert>

using namespace std;

using matrix = vector<vector<int>>;

static void parse_file(FILE* file, matrix& map)
{
  string line;
  vector<int> row;
  for(unsigned i = 0; i < 10; ++i) {
    char c;
    for(unsigned j = 0; j < 10; ++j) {
      assert(fscanf(file, "%c", &c) == 1);
      row.push_back(c-'0');
    }
    assert(fscanf(file, "%c", &c) == 1);
    map.push_back(row);
    row.clear();
  }
}

static int get(const matrix& map, int x, int y)
{
  if(x < 0 || y < 0 || (unsigned) x >= map.size() || (unsigned) y >= map[0].size())
    return -1;
  return map[x][y];
}

static int light_one(matrix& map, int x, int y)
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

static bool add_one(matrix& map, int& count)
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

bool day11(long& p1, long& p2)
{
  FILE* file;
  file = fopen("input/11", "r");
  if(file != NULL) {
    matrix map;

    parse_file(file, map);
    fclose(file);

    int count = 0;
    unsigned i = 1;
    while(!add_one(map, count)) {
      if(i == 100)
        p1 = count;
      ++i;
    }
    p2 = i;

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

