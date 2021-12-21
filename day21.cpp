
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

static void parse_file(fstream& file, int& player1, int& player2)
{
  string line;
  getline(file, line);
  sscanf(line.c_str(), "Player 1 starting position: %d", &player1);
  getline(file, line);
  sscanf(line.c_str(), "Player 2 starting position: %d", &player2);
}

int dieRolls = 0;
int roll_die() {
  int roll = ++dieRolls;
  while(roll > 100)
    roll -= 100;
  return roll;
}

int step(int& p) {
  for(unsigned i = 0; i < 3; ++i) {
    p = p + roll_die();
  }
  while(p > 10)
    p -= 10;
  return p;
}

int play(int p1, int p2) {
  int score1 = 0;
  int score2 = 0;
  const int win = 1000;
  while(score1 < win && score2 < win) {
    score1 += step(p1);
    if(score1 >= win)
      break;
    score2 += step(p2);
  }
  cout << dieRolls << ' ' << min(score1, score2) << ' ' << max(score1, score2) << endl;
  return dieRolls * min(score1, score2);
}

bool day21(long& part1, long& part2)
{
  fstream file;
  file.open("input/21", ios::in);
  if(file.is_open()) {
    int p1, p2;

    parse_file(file, p1, p2);
    file.close();

    part1 = play(p1, p2);

    array<array<array<array<pair<long,long>,10>,10>,22>,22> games;
    for(unsigned i = 0; i < games[0].size() ; ++i) {
      for(unsigned j = 0; j < games[0][0].size(); ++j) {
        for(unsigned k = 0; k < games[0][0][0].size(); ++k) {
          games[21][i][j][k].first = 1;
          if(i != 21)
            games[i][21][j][k].second = 1;
        }
      }
    }

    for(int i = games.size()-2; i >= 0 ; --i) {
      for(int j = games[0].size()-2; j >= 0 ; --j) {
        for(int k = 0; k < (int)games[0][0].size() ; ++k) {
          for(int l = 0; l < (int)games[0][0][0].size() ; ++l) {
            for(int r = 3; r < 10; ++r) {
              int weight;
              if(r == 3 || r == 9)
                weight = 1;
              if(r == 4 || r == 8)
                weight = 3;
              if(r == 5 || r == 7)
                weight = 6;
              if(r == 6)
                weight = 7;
              int score1 = i + 1;
              int new_place1 = k+2;
              if(new_place1 == 11)
                new_place1 = 1;
              score1 += new_place1;
              if(score1 > 21)
                score1 = 21;
               games[i][j][k][l].first += games[score1-1][j][new_place1][l].first * weight;
               games[i][j][k][l].second += games[score1-1][j][new_place1][l].second * weight;
               if(score1 == 21)
                 continue;

              int score2 = j + 1;
              int new_place2 = l+2;
              if(new_place2 == 11)
                new_place2 = 1;
              score2 += new_place2;
              if(score2 > 21)
                score2 = 21;
               games[score1-1][j][k][l].first += games[score1-1][score2-1][new_place1-1][new_place2-1].first * weight;
               games[score1-1][j][k][l].second += games[score1-1][score2-1][new_place1-1][new_place2-1].second * weight;
            }
          }
        }
      }
    }
    cout << games[0][0][p1][p2].first << endl;
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
  day21(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
