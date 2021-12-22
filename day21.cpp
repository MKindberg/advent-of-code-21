
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

static void parse_file(fstream &file, int &player1, int &player2) {
  string line;
  getline(file, line);
  sscanf(line.c_str(), "Player 1 starting position: %d", &player1);
  getline(file, line);
  sscanf(line.c_str(), "Player 2 starting position: %d", &player2);
}

int dieRolls = 0;
int roll_die() {
  int roll = ++dieRolls;
  while (roll > 100)
    roll -= 100;
  return roll;
}

int step(int &p) {
  for (unsigned i = 0; i < 3; ++i) {
    p = p + roll_die();
  }
  while (p > 10)
    p -= 10;
  return p;
}

int play(int p1, int p2) {
  int score1 = 0;
  int score2 = 0;
  const int win = 1000;
  while (score1 < win && score2 < win) {
    score1 += step(p1);
    if (score1 >= win)
      break;
    score2 += step(p2);
  }
  cout << dieRolls << ' ' << min(score1, score2) << ' ' << max(score1, score2)
       << endl;
  return dieRolls * min(score1, score2);
}

int getWeight(int roll) {
  if (roll == 3 || roll == 9)
    return 1;
  if (roll == 4 || roll == 8)
    return 3;
  if (roll == 5 || roll == 7)
    return 6;
  //if (roll == 6)
  return 7;
}

int step2(int tile, int steps) {
  int t = tile + steps;
  if(t > 10)
    t -= 10;
  return t;
}

long win1 = 0;
long win2 = 0;
void play2(int score1, int score2, int tile1, int tile2, long weight);
void play1(int score1, int score2, int tile1, int tile2, long weight) {

  for(unsigned i = 3; i < 10; ++i) {
    int t1 = step2(tile1, i);
    int s1 = score1 + t1;
    if(s1 >= 21) {
      win1 += weight * getWeight(i);
    } else {
      play2(s1, score2, t1, tile2, weight * getWeight(i));
    }
  }

}
void play2(int score1, int score2, int tile1, int tile2, long weight) {

  for(unsigned i = 3; i < 10; ++i) {
    int t2 = step2(tile2, i);
    int s2 = score2 + t2;
    if(s2 >= 21) {
      win2 += weight * getWeight(i);
    } else {
      play1(score1, s2, tile1, t2, weight * getWeight(i));
    }
  }

}

bool day21(long &part1, long &part2) {
  fstream file;
  file.open("input/21", ios::in);
  if (file.is_open()) {
    int p1, p2;

    parse_file(file, p1, p2);
    file.close();

    part1 = play(p1, p2);
    play1(0, 0, p1, p2, 1);
    part2 = max(win1, win2);

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

#ifdef MAIN
int main() {
  long p1, p2;
  day21(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
