#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Board {
  int mBoard[5][5];

  bool has_won() {
    // rows
    for(int i = 0; i < 5; ++i)
    for(int j = 0; j < 5; ++j) {
      if(mBoard[j][i] != -1)
      break;
      if(j == 4)
      return true;
    }
    //cols
    for(int j = 0; j < 5; ++j)
    for(int i = 0; i < 5; ++i) {
      if(mBoard[j][i] != -1)
      break;
      if(i == 4)
      return true;
    }
    return false;
  }

  public:
  Board(const vector<string>& numbers)
  {
    for(int i = 0; i < 5; ++i)
    sscanf(numbers[i].c_str(), "%d %d %d %d %d", &mBoard[0][i], &mBoard[1][i], &mBoard[2][i], &mBoard[3][i], &mBoard[4][i]);
  }

  int get_score(int number)
  {
    int score = 0;
    for(int i = 0; i < 5; ++i)
      for(int j = 0; j < 5; ++j)
        if(mBoard[j][i] != -1)
          score += mBoard[j][i];
    return score * number;
  }

  int mark(int number)
  {
    for(int i = 0; i < 5; ++i)
      for(int j = 0; j < 5; ++j)
        if(mBoard[j][i] == number) {
          mBoard[j][i] = -1;
          return has_won() ? get_score(number) : -1;
        }
    return -1;
  }

  void print()
  {
    for(int i = 0; i < 5; ++i){
      for(int j = 0; j < 5; ++j){
        cout << mBoard[j][i] << ' ';
      }
      cout << endl;
    }
    cout << endl;
  }
};

static void parse_file(fstream& file, vector<int>& numbers, vector<Board>& boards)
{
  string line;
  vector<string> data;

  getline(file, line);
  while(line.length() != 0) {
    numbers.push_back(stoi(line.substr(0, line.find(','))));
    line.erase(0, line.find(','));
    line.erase(0, 1);
  }
  getline(file, line);
  while(getline(file, line)) {
    if(line == "") {
      boards.emplace_back(data);
      data.clear();
    }
    else
      data.push_back(line);
  }
}

static void play(const vector<int>& numbers, vector<Board>& boards, long& first, long& last)
{
  bool first_win = true;
  for(int number : numbers) {
    for(size_t i = 0; i < boards.size(); ++i) {
      int score = boards[i].mark(number);
      if(score != -1) {
        if(first_win) {
          first = score;
          first_win = false;
        } else if(boards.size() == 1) {
          last = score;
        }
        boards.erase(begin(boards) + i);
        --i;
      }
    }
  }
}

bool day4(long& p1, long& p2)
{
  fstream file;
  file.open("input/4", ios::in);
  if(file.is_open()) {
    vector<int> numbers;
    vector<Board> boards;

    parse_file(file, numbers, boards);

    play(numbers, boards, p1, p2);

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}
