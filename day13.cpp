
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <stdio.h>

using namespace std;

static void parse_file(FILE* file, vector<pair<int,int>>& dots, vector<pair<char,int>>& folds)
{
  char axis;
  int x, y, val;
  while(fscanf(file, "%d,%d\n", &x, &y) != 0) {
    dots.emplace_back(y, x);
  }
  while(fscanf(file, "fold along %c=%d\n", &axis, &val) != EOF) {
    folds.emplace_back(axis, val);
  }
}

static void fill_paper(vector<vector<bool>>& paper, const vector<pair<int,int>>& dots, int maxX, int maxY) {
  for(int i = 0; i < maxY; ++i) {
    vector<bool> v(maxX, false);
    paper.push_back(v);
  }

  for(pair<int,int> p : dots) {
    paper[p.first][p.second] = true;
  }
}

static void fold(vector<vector<bool>>& paper, char axis, int foldv)
{
  if(axis == 'y') {
    for(unsigned x = 0; x < paper[0].size(); ++x) {
      for(int i = 1; i < foldv+1; ++i) {
        paper[foldv-i][x] = paper[foldv-i][x] || paper[foldv+i][x];
      }
    }
    paper.resize(foldv);
  } else {
    for(unsigned y = 0; y < paper.size(); ++y) {
      for(int i = 1; i < foldv+1; ++i) {
        paper[y][foldv-i] = paper[y][foldv-i] || paper[y][foldv+i];
      }
      paper[y].resize(foldv);
    }
  }
}

bool day13(long& p1, long& p2)
{
  FILE* file;
  file = fopen("input/13", "r");
  if(file != NULL) {
    vector<pair<int,int>> dots;
    vector<pair<char,int>> folds;

    vector<vector<bool>> paper;

    parse_file(file, dots, folds);
    fclose(file);

    int maxX = (*max_element(dots.cbegin(), dots.cend(), [](pair<int,int> a, pair<int,int> b){return a.second < b.second;})).second +1;
    int maxY = (*max_element(dots.cbegin(), dots.cend(), [](pair<int,int> a, pair<int,int> b){return a.first < b.first;})).first+1;

    fill_paper(paper, dots, maxX, maxY);

    p1 = 0;
    fold(paper, folds[0].first, folds[0].second);
    for_each(paper.cbegin(), paper.cend(),
      [&p1](auto v){
        p1 += count(v.cbegin(), v.cend(), true);
      }
    );
    for_each(folds.cbegin()+1, folds.cend(),
      [&paper](auto f){
        fold(paper, f.first, f.second);
      }
    );
#ifdef MAIN
    for(auto v : paper) {
      for(auto e : v)
        if(e)
          cout << '#';
        else
          cout << '.';
      cout << endl;
    }
#endif

    p2 = 0;
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
  day13(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
