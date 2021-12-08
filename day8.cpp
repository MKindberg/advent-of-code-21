#!/bin/bash
# This c++ file behaves like a script and running it with ./ will both compile and run
(echo -e '\n\n'; tail +4 $0) | g++ -g -std=c++17 -Wall -Werror -x c++ - && exec time -p ./a.out

#include <array>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;


struct number
{
  size_t length;
  vector<int> prereq;
  function<bool(array<number,10>, string)> verif;
  string pattern;
};

int part1(const vector<vector<string>>& outputs)
{
  int count = 0;
  for(auto o : outputs) {
    for(size_t i = 0; i < o.size(); ++i) {
      size_t l = o[i].length();
      if(l == 2 || l == 3 || l == 4 || l == 7) {
        ++count;
      }
    }
  }
  return count;
}

void parse_file(FILE* file, vector<vector<string>>& patterns, vector<vector<string>>& outputs)
{
  char buf[10];
  int n = 0;
  vector<string> p;
  vector<string> o;

  while(fscanf(file, "%s ", buf) != EOF) {
    string s = buf;
    sort(s.begin(), s.end());
    if(n > 14) {
      n = 0;
      patterns.push_back(p);
      outputs.push_back(o);
      p.clear();
      o.clear();
    }
    if(n < 10) {
      p.push_back(s);
      ++n;
    } else if(n == 10) {
      ++n;
    } else {
      o.push_back(s);
      ++n;
    }
  }
  patterns.push_back(p);
  outputs.push_back(o);
}

bool expect_missing(string a, string b, int m)
{
  int missing = 0;
  for(char c : b)
    if(a.find(c) == string::npos && ++missing > m)
      return false;
  return missing == m;
}

int indexOf(array<number,10> numbers, string s)
{
  for(size_t i = 0; i < numbers.size(); ++i) {
    if(numbers[i].pattern == s)
      return i;
  }
  return -1;
}

void init_numbers(array<number,10>& n)
{
  n = {{
    {6, {7, 9}, [](array<number,10> n, string s){return expect_missing(s, n[7].pattern, 0);}, ""},
    {2, {}, [](array<number,10> n, string s){return true;}, ""},
    {5, {4}, [](array<number,10> n, string s){return expect_missing(s, n[4].pattern, 2);}, ""},
    {5, {7}, [](array<number,10> n, string s){return expect_missing(s, n[7].pattern, 0);}, ""},
    {4, {}, [](array<number,10> n, string s){return true;}, ""},
    {5, {3, 4}, [](array<number,10> n, string s){return expect_missing(s, n[4].pattern, 1);}, ""},
    {6, {0, 9}, [](array<number,10> n, string s){return true;}, ""},
    {3, {}, [](array<number,10> n, string s){return true;}, ""},
    {7, {}, [](array<number,10> n, string s){return true;}, ""},
    {6, {4}, [](array<number,10> n, string s){return expect_missing(s, n[4].pattern, 0);}, ""},
  }};
}

bool matches_number(const array<number,10>& numbers, const string pattern, int n) {
  if(pattern.length() != numbers[n].length)
    return false;
  for(int p : numbers[n].prereq)
    if(numbers[p].pattern == "")
      return false;
  return numbers[n].verif(numbers, pattern);
}

int part2(vector<vector<string>>& patterns, vector<vector<string>>& outputs)
{
  int sum = 0;
  array<number,10> numbers;
  for(size_t i = 0; i < patterns.size(); ++i) {
    init_numbers(numbers);
    while(patterns[i].size() > 0) {
      size_t j = 0;
      while(j < patterns[i].size()) {
        for(size_t k = 0; k < numbers.size(); ++k) {
          if(matches_number(numbers, patterns[i][j], k)) {
            numbers[k].pattern = patterns[i][j];
            patterns[i].erase(patterns[i].begin() + j);
            j = 0;
            break;
          }
        }
        ++j;
      }
    }
    sum += indexOf(numbers, outputs[i][0]) * 1000;
    sum += indexOf(numbers, outputs[i][1]) * 100;
    sum += indexOf(numbers, outputs[i][2]) * 10;
    sum += indexOf(numbers, outputs[i][3]);
  }
  return sum;
}

int main()
{
  FILE* file = fopen("input", "r");
  if(file != NULL) {
    vector<vector<string>> patterns;
    vector<vector<string>> outputs;

    parse_file(file, patterns, outputs);

    cout << part1(outputs) << endl;
    cout << part2(patterns, outputs) << endl;

    fclose(file);

  } else {
    cout << "Couldn't open file" << endl;
  }
}

