#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>
#include <climits>

using namespace std;

static void parse_file(FILE* file, vector<int>& numbers)
{
  int n;
  while(fscanf(file, "%d,", &n) != EOF) {
    numbers.push_back(n);
  }
}

static int sum_fuel1(vector<int>& crabs, int pos)
{
  return transform_reduce(
    crabs.begin(),
    crabs.end(),
    0,
    plus<int>(),
    [pos](int c){return abs(pos-c);}
  );
}

static int sum_fuel2(vector<int>& crabs, int pos)
{
  return transform_reduce(
    crabs.begin(),
    crabs.end(),
    0,
    plus<int>(),
    [pos](int c){
      int i = 0;
      int steps = abs(pos-c);
      vector<int> v(steps);
      generate_n(v.begin(), steps, [&i](){return ++i;});
      return reduce(v.begin(), v.end());
    }
  );
}

static int part1(vector<int>& crabs)
{
  int last = INT_MAX;
  int current = INT_MAX-1;
  for(int i = 0; last> current;++i) {
    last = current;
    current = sum_fuel1(crabs,i);
  }
  return last;
}

static int part2(vector<int>& crabs)
{
  int mean = reduce(crabs.begin(), crabs.end()) / crabs.size();
  return min(min(sum_fuel2(crabs, mean-1), sum_fuel2(crabs, mean)), sum_fuel2(crabs, mean+1));
}

bool day7(long& p1, long& p2)
{
  FILE* file = fopen("input/7", "r");
  if(file != NULL) {
    vector<int> crabs;

    parse_file(file, crabs);

    p1 = part1(crabs);
    p2 = part2(crabs);

    fclose(file);

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

