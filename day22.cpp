#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stack>
#include <type_traits>
#include <vector>

using namespace std;

using Point = tuple<int, int, int>;

struct Instruction {
  bool turn_on;
  int x1, x2, y1, y2, z1, z2;
  long value;

  Instruction(bool turn_on, int x1, int x2, int y1, int y2, int z1, int z2, int value = 1)
      : turn_on(turn_on),
        x1(x1),
        x2(x2),
        y1(y1),
        y2(y2),
        z1(z1),
        z2(z2),
        value(value)
  {
  }

  bool overlapps(Instruction i) {
    if(x1 > i.x2 || x2 < i.x1)
      return false;
    if(y1 > i.y2 || y2 < i.y1)
      return false;
    if(z1 > i.z2 || z2 < i.z1)
      return false;
    return true;
  }

  long size() {
    return ((long)abs(x2-x1+1))*abs(y2-y1+1)*abs(z2-z1+1) * value;
  }

  bool operator==(Instruction i) {
    return x1 == i.x1 && x2 == i.x2 && y1 == i.y1 && y2 == i.y2 && z1 == i.z1 && z2 == i.z2;
  }
  void print() {
    printf("(%d..%d, %d..%d, %d..%d\n)", x1, x2, y1, y2, z1, z2);
  }
};


static void
parse_file(fstream &file, vector<Instruction> &instructions) {
  string line;
  while (getline(file, line)) {
    int x1, x2, y1, y2, z1, z2;
    sscanf(line.c_str(), "%*s x=%d..%d,y=%d..%d,z=%d..%d", &x1, &x2, &y1, &y2,
           &z1, &z2);
    instructions.emplace_back(line.substr(0, 2) == "on", x1, x2, y1, y2, z1,
                              z2);
  }
}

static Instruction overlap(Instruction i1, Instruction i2) {
  array<int,4> x{i1.x1, i1.x2, i2.x1, i2.x2};
  array<int,4> y{i1.y1, i1.y2, i2.y1, i2.y2};
  array<int,4> z{i1.z1, i1.z2, i2.z1, i2.z2};
  sort(x.begin(), x.end());
  sort(y.begin(), y.end());
  sort(z.begin(), z.end());
  int value = -i1.value;

  return Instruction(true, x[1], x[2], y[1], y[2], z[1], z[2], value);
}

static void addInstruction(vector<Instruction>& state, Instruction i) {
  unsigned size = state.size();
  for(unsigned j = 0; j < size; ++j) {
    if(state[j].overlapps(i)) {
      state.push_back(overlap(state[j], i));
    }
  }
  if(i.turn_on)
  state.push_back(i);
}

static long count_on(vector<Instruction> state) {
  long size = 0;
  for(Instruction s: state) {
    size += s.size();
  }
  return size;
}

static long reboot(vector<Instruction> instructions) {
  vector<Instruction> state;
  int j = 0;
  for(Instruction i : instructions) {
    cout << j++ << ' ' << count_on(state) << endl;
    if(i.size() == 0) {
      state.push_back(i);
      continue;
    }
    addInstruction(state, i);
  }
  return count_on(state);
}

bool day22(long &p1, long &p2) {
  fstream file;
  file.open("input/22", ios::in);
  if (file.is_open()) {
    vector<Instruction> instructions;

    parse_file(file, instructions);
    file.close();

    p1 = 0;
    p2 = reboot(instructions);
  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

#ifdef MAIN
int main() {
  long p1, p2;
  day22(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
