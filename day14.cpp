
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <stdio.h>
#include <map>
#include <cstring>
#include <cassert>

using namespace std;

static void add_pair(map<string,long>& polymer, char c1, char c2, long num)
{
  string s({c1, c2});
  if(polymer.contains(s))
    polymer[s] += num;
  else
    polymer.insert({s, num});
}
static void parse_file(FILE* file, map<string,long>& polymer, map<string,char>& transformations, string& start)
{
  char buf[30];
  char in[3];
  char out;
  assert(fscanf(file, "%s", buf)==1);
  start = buf;
  for(unsigned i = 0; i < start.size()-1; ++i) {
    add_pair(polymer, start[i], start[i+1], 1);
  }
  assert(fscanf(file, "\n") == 0);

  while(fscanf(file, "%2s -> %c", in, &out) != EOF) {
    transformations.insert({in, out});
  }
}

static map<string,long> step(map<string,char>& transformations, const map<string,long>& polymer)
{
  map<string,long> temp;
  for(auto [k, v] : polymer) {
    if(transformations.contains(k)) {
      add_pair(temp, k[0], transformations[k], v);
      add_pair(temp, transformations[k], k[1], v);
    } else {
      add_pair(temp, k[0], k[1], v);
    }
  }
  return temp;
}

static void count_letters(const map<string,long>& polymer, map<char,long>& letters){
  for(auto [k, v] : polymer) {
    for(char c : k) {
      if(letters.contains(c)) {
        letters[c] += v;
      } else {
        letters.insert({c, v});
      }
    }
  }
}

static long diff_min_max(const string& start, const map<string,long>& polymer)
{
  map<char,long> letters;
  count_letters(polymer, letters);
  ++letters[start[0]];
  ++letters[start[start.size()-1]];

  long most = (*max_element(letters.cbegin(), letters.cend(), [](auto a, auto b){return a.second < b.second;})).second/2;
  long least = (*min_element(letters.cbegin(), letters.cend(), [](auto a, auto b){return a.second < b.second;})).second/2;
  return most - least;
}

bool day14(long& p1, long& p2)
{
  FILE* file;
  file = fopen("input/14", "r");
  if(file != NULL) {
    map<string,long> polymer;
    map<string,char> transformations;
    string start;

    parse_file(file, polymer, transformations, start);
    fclose(file);

    for(unsigned i = 0; i < 10; ++i) {
      polymer = step(transformations, polymer);
    }
    p1 = diff_min_max(start, polymer);
    for(unsigned i = 0; i < 30; ++i) {
      polymer = step(transformations, polymer);
    }
    p2 = diff_min_max(start, polymer);
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
  day14(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
