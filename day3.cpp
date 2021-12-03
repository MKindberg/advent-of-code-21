#!/bin/bash
# This c++ file behaves like a script and running it with ./ will both compile and run
(echo -e '\n\n'; tail +4 $0) | g++ -std=c++17 -Wall -Werror -O3 -x c++ - && exec time -p ./a.out

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

unsigned flip_bits(unsigned n, unsigned l)
{
  return ~n & ((1 << l) - 1);
}

char most_common(const vector<string>& numbers, int bit)
{
  int count = 0;
  for(unsigned i = 0; i < numbers.size(); ++i)
    if(numbers[i][bit] == '1')
      ++count;
    else
      --count;
  if(count >= 0)
    return '1';
  return '0';
}
void remove_non_matching(vector<string>& numbers, int index, int value)
{
  for(unsigned i = 0; i < numbers.size(); ++i) {
    if(numbers[i][index] != value + '0'){
      numbers.erase(numbers.begin() + i);
      --i;
    }
  }
}

void part1(const vector<string>& numbers)
{
  int size = numbers[0].length();
  string n = "";
  for(int i = 0; i < size; ++i)
    n += most_common(numbers, i);
  unsigned gamma = stoi(n, nullptr, 2);
  unsigned epsilon = flip_bits(gamma, size);

  cout << "Part 1: "<< gamma * epsilon << endl;
}

void part2(const vector<string>& numbers)
{
  int size = numbers[0].length();

  vector<string> oxygen = numbers;
  vector<string> co2 = numbers;

  for(int i = 0; i < size; ++i)
  {
    if(oxygen.size() > 1) {
    char m = most_common(oxygen, i);
    auto e = remove_if(begin(oxygen), end(oxygen), [m, i](string s){return s[i] != m;});
    oxygen.erase(e, end(oxygen));
    }

    if(co2.size() > 1) {
      char m = most_common(co2, i);
      auto e = remove_if(begin(co2), end(co2), [m, i](string s){return s[i] == m;});
      co2.erase(e, end(co2));
    }
  }

  assert(oxygen.size() == 1);
  assert(co2.size() == 1);

  cout << "Part 2: " << stoi(oxygen[0], nullptr, 2) * stoi(co2[0], nullptr, 2) << endl;
}

int main()
{
  fstream file;
  file.open("input.txt", ios::in);
  if(file.is_open()) {
    string line;
    vector<string> numbers;
    while(getline(file, line)) {
      numbers.push_back(line);
    }

    part1(numbers);

    part2(numbers);

  } else {
    cout << "Couldn't open file";
  }
}
