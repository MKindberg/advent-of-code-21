#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
#include <array>

using namespace std;

static int count_adj(vector<string>& map, int x, int y)
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

static int basin_size(vector<string> map, int x, int y)
{
  return count_adj(map, x, y);
}

static void parse_file(fstream& file, vector<string>& map)
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

bool day9(long& p1, long& p2)
{
  fstream file;
  file.open("input/9", ios::in);
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

    p1 = risk_sum;
    p2 = accumulate(sizes.begin(), sizes.begin()+3, 1, multiplies<int>());

    file.close();

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

