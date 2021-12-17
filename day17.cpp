#include <chrono>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

static void parse_file(FILE* file, pair<int,int>& x, pair<int,int>& y)
{
  fscanf(file, "target area: x=%d..%d, y=%d..%d", &x.first, &x.second, &y.first, &y.second);
}

int in_target(const pair<int,int>& x, const pair<int,int>& y, const pair<int,int>& current)
{
  if(current.second < y.first || current.first > x.second)
    return 2; // No
  if(current.first < x.first || current.second > y.second)
    return 0; // Not yet
  return 1; // YES!!
}

int shoot(const pair<int,int>& x, const pair<int,int>& y, pair<int,int>& velocity, long* max)
{
  pair<int,int> pos = {0, 0};
  int res = 0;
  while((res = in_target(x, y, pos)) == 0)
  {
    pos.first += velocity.first;
    pos.second += velocity.second;
    velocity.first -= velocity.first == 0 ? 0 : velocity.first / abs(velocity.first);
    --velocity.second;
    if(*max < pos.second)
      *max = pos.second;

  }
  return res;
}

void find_max_height(pair<int,int>& x, pair<int,int>& y, long* max, long* num)
{
  pair<int,int> velocity = {1,1};

  for(int i = 1; i < x.second+1; ++i) {
    if(x.first > (i*i + i) / 2)
      continue;
    for(int j = y.first; j < 200; ++j) {
      velocity.first = i;
      velocity.second = j;
      long m = 0;
      int r = shoot(x, y, velocity, &m);
      if(r == 1) {
        ++(*num);
        if(*max < m)
          *max = m;
      }
    }
  }
}



bool day17(long& p1, long& p2)
{
  FILE* file;
  file = fopen("input/17", "r");
  if(file != NULL) {
    pair<int,int> x, y;
    p1 = 0, p2 = 0;

    parse_file(file, x, y);
    fclose(file);

    find_max_height(x, y, &p1, &p2);

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

#ifdef MAIN
using namespace std::chrono;
int main()
{
  long p1, p2;
  const int runs = 1;
  size_t total_time = 0;
  auto start = high_resolution_clock::now();
  for(int i = 0; i < runs; ++i)
    day17(p1, p2);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  total_time += duration.count()/runs;
  cout << "Time: " << total_time << endl;
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
