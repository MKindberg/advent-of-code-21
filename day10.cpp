#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

static void parse_file(fstream& file, vector<string>& lines)
{
  string line;
  while(getline(file, line)) {
    lines.push_back(line);
  }
}

static int get_score2(char b)
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

static int get_score(char b)
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

static bool calc_score(string line, long& invalid, long& incomplete)
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

bool day10(long& p1, long& p2)
{
  fstream file;
  file.open("input/10", ios::in);
  if(file.is_open()) {
    vector<string> lines;

    parse_file(file, lines);
    file.close();

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

    p1 = invalid_score;
    p2 = incomplete_scores[mid];

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

