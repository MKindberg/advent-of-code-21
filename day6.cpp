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

using namespace std;

void parse_file(FILE* file, vector<int>& numbers)
{
  int n;
  while(fscanf(file, "%d,", &n) != EOF) {
    numbers.push_back(n);
  }
}

long breed(const vector<int>& fish_start, int days) {
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

int main()
{
  FILE* file = fopen("input", "r");
  if(file != NULL) {
    vector<int> fish_start;

    parse_file(file, fish_start);

    cout << "Part 1: " << breed(fish_start, 80) << endl;
    cout << "Part 2: " << breed(fish_start, 256) << endl;

    fclose(file);

  } else {
    cout << "Couldn't open file" << endl;
  }
}

