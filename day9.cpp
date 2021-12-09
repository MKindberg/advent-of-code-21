#!/bin/bash
# This c++ file behaves like a script and running it with ./ will both compile and run
(echo -e '\n\n'; tail +4 $0) | g++ -g -std=c++17 -Wall -Werror -x c++ - && exec time -p ./a.out

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int count_adj(vector<string>& map, int x, int y)
{
  map[x][y] = '9';
  int count = 1;
  if(map[x+1][y] != '9')
    count += count_adj(map, x+1, y);
  if(map[x-1][y] != '9')
    count += count_adj(map, x-1, y);
  if(map[x][y+1] != '9')
    count += count_adj(map, x, y+1);
  if(map[x][y-1] != '9')
    count += count_adj(map, x, y-1);
  return count;
}
int basin_size(vector<string> map, int x, int y)
{
  return count_adj(map, x, y);
}

void parse_file(fstream& file, vector<string>& map)
{
  string line;
  while(getline(file, line)) {
    line = '9' + line + '9';
    map.push_back(line);
  }

  line = map[0];
  fill(line.begin(), line.end(), '9');
  map.insert(map.begin(), line);
  map.push_back(line);
}

int main()
{
  fstream file;
  file.open("input", ios::in);
  if(file.is_open()) {
    vector<string> map;

    parse_file(file, map);

    vector<int> sizes;
    int risk_sum = 0;
    for(size_t i = 1; i < map.size() - 1; ++i) {
      for(size_t j = 1; j < map[i].length() - 1; ++j) {
        if(map[i][j] < map[i+1][j] &&
              map[i][j] < map[i-1][j] &&
              map[i][j] < map[i][j+1] &&
              map[i][j] < map[i][j-1]) {
          risk_sum += map[i][j] - '0' + 1;
          sizes.push_back(basin_size(map, i, j));
        }
      }
    }
    partial_sort(sizes.begin(), sizes.begin() + 3, sizes.end(), std::greater<int>());

    cout << risk_sum << endl;
    cout << accumulate(sizes.begin(), sizes.begin()+3, 1, multiplies<int>()) << endl;

    file.close();

  } else {
    cout << "Couldn't open file" << endl;
  }
}

