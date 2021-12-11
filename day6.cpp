#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

static void parse_file(FILE* file, vector<int>& numbers)
{
  int n;
  while(fscanf(file, "%d,", &n) != EOF) {
    numbers.push_back(n);
  }
}

static long breed(const vector<int>& fish_start, int days) {
    array<long,9> fish = {0};

    for(int f : fish_start)
      ++fish[f];

    for(int i = 0; i < days; ++i) {
      long new_fish = fish[0];
      rotate(begin(fish), begin(fish)+1, begin(fish)+7);
      fish[6] += fish[7];
      fish[7] = fish[8];
      fish[8] = new_fish;
    }

    return accumulate(begin(fish), end(fish), 0l);
}

bool day6(long& p1, long& p2)
{
  FILE* file = fopen("input/6", "r");
  if(file != NULL) {
    vector<int> fish_start;

    parse_file(file, fish_start);

    p1 = breed(fish_start, 80);
    p2 = breed(fish_start, 256);

    fclose(file);

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

