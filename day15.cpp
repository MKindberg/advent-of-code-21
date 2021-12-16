#include <cassert>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <stack>
#include <vector>
#include <climits>

using namespace std;

using matrix=vector<vector<long>>;

static void print(matrix m)
{
  for(auto v : m) {
    for(auto i : v) {
      if(i != INT_MAX)
        printf("%4ld ", i);
      else
        printf("%4s", "-");
    }
    cout << endl;
  }
  cout << endl;
}

static void parse_file(fstream& file, matrix& map)
{
  string line;
  while(getline(file, line)) {
    vector<long> values;
    for(char c : line)
      values.push_back(c - '0');
    map.push_back(values);
  }
}

pair<int,int> visit_next(const matrix &paths, vector<vector<bool>>& visited, vector<pair<int,int>>& seen) {
  pair<int,int> min_p = {-1, -1};
  long min_v = INT_MAX;
  for(unsigned i = 0; i < seen.size(); ++i) {
    auto[y,x] = seen[i];
    if(visited[y][x]) {
      seen.erase(seen.begin()+i);
      --i;
      continue;
    }
    if(min_v >= paths[y][x]) {
      min_v = paths[y][x];
      min_p.first = y;
      min_p.second = x;
    }
  }
  return min_p;
}

static void fill_adj(const matrix& map, matrix &paths, vector<vector<bool>>& visited, vector<pair<int,int>>& seen, unsigned y, unsigned x)
{
  visited[y][x] = true;
  unsigned height = map.size();
  unsigned width = map[0].size();

  if(y == height-1 && x == width-1) {
    return;
  }

  vector<tuple<int,int>> dirs;
  if(x < width - 1 && !visited[y][x+1])
    dirs.push_back({0,1});
  if(y < height - 1 && !visited[y+1][x])
    dirs.push_back({1,0});
  if(x > 0 && !visited[y][x-1])
    dirs.push_back({0,-1});
  if(y > 0 && !visited[y-1][x])
    dirs.push_back({-1,0});
  for(auto& d : dirs){
    paths[y+get<0>(d)][x+get<1>(d)] = min(paths[y+get<0>(d)][x+get<1>(d)], paths[y][x] + map[y+get<0>(d)][x+get<1>(d)]);
    seen.push_back({y+get<0>(d), x+get<1>(d)});
  }
}

static void fill_paths(const matrix& map, matrix &paths, vector<vector<bool>>& visited, unsigned y, unsigned x) {
  vector<pair<int,int>> seen = {{0, 0}};
  auto[yy, xx] = visit_next(paths, visited, seen);
  while(yy != -1) {
    fill_adj(map, paths, visited, seen, yy, xx);
    tie(yy, xx) = visit_next(paths, visited, seen);
  }

}

static long fill_paths(const matrix& map, matrix &paths, unsigned y, unsigned x)
{
    unsigned height = map.size();
    unsigned width = map[0].size();
    if(y == height-1 && x == width-1)
      return map[y][x];
    if(paths[y+1][x+1] == 0) {
      vector<long> possible{
        fill_paths(map, paths, y+1, x),
        fill_paths(map, paths, y, x+1),
      };
      paths[y+1][x+1] = map[y][x] + *min_element(possible.begin(), possible.end());
    }
    return paths[y+1][x+1];
}

static void expand_map(const matrix& map, matrix& new_map)
{
  unsigned height = map.size();
  unsigned width = map[0].size();

  for(unsigned y = 0; y < height; ++y) {
    vector<long> v;
    for(unsigned i = 0; i < 5; ++i) {
      for(unsigned x = 0; x < width; ++x) {
        int n = map[y][x]+i;
        if(n > 9)
          n-=9;
        v.push_back(n);
      }
    }
    new_map.push_back(v);
  }
  for(unsigned i = 1; i < 5; ++i) {
    for(unsigned y = 0; y < height; ++y) {
      vector<long> v;
      for(unsigned x = 0; x < 5*width; ++x) {
        int n = new_map[y][x]+i;
        if(n > 9)
          n-=9;
        v.push_back(n);
      }
      new_map.push_back(v);
    }
  }
}

static void init_paths(matrix& map, matrix& paths, vector<vector<bool>>& visited)
{
  unsigned height = map.size();
  unsigned width = map[0].size();
  for(unsigned i = 0; i < height; ++i) {
    paths.emplace_back(width, INT_MAX);
    visited.emplace_back(width, false);
  }
}

bool day15(long& p1, long& p2)
{
  fstream file;
  file.open("input/15", ios::in);
  if(file.is_open()) {
    matrix map, paths, e_map, e_paths;
    vector<vector<bool>> visited, e_visited;

    parse_file(file, map);
    file.close();

    init_paths(map, paths, visited);
    expand_map(map, e_map);
    init_paths(e_map, e_paths, e_visited);

    map[0][0] = 0;
    paths[0][0] = 0;
    fill_paths(map, paths, visited, 0, 0);
    p1 = paths[paths.size()-1][paths[0].size()-1];

    e_map[0][0] = 0;
    e_paths[0][0] = 0;
    fill_paths(e_map, e_paths, e_visited, 0, 0);
    p2 = e_paths[e_paths.size()-1][e_paths[0].size()-1];
  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

#ifdef MAIN
int main()
{
  long p1, p2;
  day15(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
