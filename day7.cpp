#!/bin/bash
# This c++ file behaves like a script and running it with ./ will both compile and run
(echo -e '\n\n'; tail +4 $0) | g++ -std=c++17 -Wall -Werror -O3 -x c++ - && exec time -p ./a.out

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

void parse_file(FILE* file, vector<int>& numbers)
{
  int n;
  while(fscanf(file, "%d,", &n) != EOF) {
    numbers.push_back(n);
  }
}

int sum_fuel1(vector<int>& crabs, int pos)
{
  return transform_reduce(
    crabs.begin(),
    crabs.end(),
    0,
    plus<int>(),
    [pos](int c){return abs(pos-c);}
  );
}
int sum_fuel2(vector<int>& crabs, int pos)
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

int part1(vector<int>& crabs)
{
  int last = INT_MAX;
  int current = INT_MAX-1;
  for(int i = 0; last> current;++i) {
    last = current;
    current = sum_fuel1(crabs,i);
  }
  return last;
}

int part2(vector<int>& crabs)
{
  int mean = reduce(crabs.begin(), crabs.end()) / crabs.size();
  return min(min(sum_fuel2(crabs, mean-1), sum_fuel2(crabs, mean)), sum_fuel2(crabs, mean+1));
}

int main()
{
  FILE* file = fopen("input", "r");
  if(file != NULL) {
    vector<int> crabs;

    parse_file(file, crabs);

    cout<< part1(crabs) << endl;
    cout<< part2(crabs) << endl;

    fclose(file);

  } else {
    cout << "Couldn't open file" << endl;
  }
}

