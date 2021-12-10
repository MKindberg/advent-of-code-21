#!/bin/bash
# This c++ file behaves like a script and running it with ./ will both compile and run
(echo -e '\n\n'; tail +4 $0) | g++ -g -std=c++17 -Wall -Werror -x c++ - && exec ./a.out

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;
using namespace std::chrono;

struct bracket
{
  char open;
  char close;
};

void parse_file(fstream& file, vector<string>& lines)
{
  string line;
  while(getline(file, line)) {
    lines.push_back(line);
  }
}

int get_score2(char b)
{
  switch(b) {
    case ')':
      return 1;
    case ']':
      return 2;
    case '}':
      return 3;
    case '>':
      return 4;
  }
  return 0;
}
int get_score(char b)
{
  switch(b) {
    case ')':
      return 3;
    case ']':
      return 57;
    case '}':
      return 1197;
    case '>':
      return 25137;
  }
  return 0;
}

bool calc_score(string line, long& invalid, long& incomplete)
{
  invalid = 0;
  incomplete = 0;
  array<pair<char,char>,4> brackets = {{
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'},
  }};

  stack<pair<char,char>> s;
  // Push opening brackets to stack, pop and compare if closing bracket
  for(char c : line) {
    auto b = find_if(brackets.begin(), brackets.end(), [c](pair<char,char> b){return b.first == c;});
    if(b == brackets.end()) {
      pair<char,char> e = s.top();
      s.pop();
      if(e.second != c) {
        invalid = get_score(c);
        return false;
      }
    }
    else {
      s.push(*b);
    }
  }
  while(!s.empty()) {
    incomplete *= 5;
    incomplete += get_score2(s.top().second);
    s.pop();
  }
  return true;
}

int main()
{
  fstream file;
  file.open("input", ios::in);
  if(file.is_open()) {
    vector<string> lines;

    parse_file(file, lines);
    file.close();

    auto start = high_resolution_clock::now();

    long invalid_score = 0;
    vector<long> incomplete_scores;

    long invalid, incomplete;
    for(string line : lines)
      if(calc_score(line, invalid, incomplete))
        incomplete_scores.push_back(incomplete);
      else
        invalid_score += invalid;

    int mid =  incomplete_scores.size()/2;
    nth_element(incomplete_scores.begin(), incomplete_scores.begin() + mid, incomplete_scores.end());
    cout << invalid_score << endl;;
    cout << incomplete_scores[mid] << endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << "µs" << endl;

  } else {
    cout << "Couldn't open file" << endl;
  }
}

