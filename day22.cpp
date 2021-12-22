#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <cmath>

using namespace std;

using Instruction = tuple<bool,int,int,int,int,int,int>;
using Point = tuple<int,int,int>;

static void parse_file(fstream& file, vector<Instruction>& instructions)
{
  string line;
  while(getline(file, line)) {
    int x1, x2, y1, y2, z1, z2;
    sscanf(line.c_str(), "%*s x=%d..%d,y=%d..%d,z=%d..%d", &x1, &x2, &y1, &y2, &z1, &z2);
    instructions.emplace_back(line.substr(0,2) == "on", x1, x2, y1, y2, z1, z2);
  }
}

bool day22(long& p1, long& p2)
{
  fstream file;
  file.open("input/22", ios::in);
  if(file.is_open()) {
    vector<Instruction> instructions;

    parse_file(file, instructions);
    file.close();
    vector<Point> points;

    Point pp = {0,0,0};
    for(unsigned i = 0; i < instructions.size(); ++i) {
      cout << i << endl;
      cout << points.size() << endl;
      for(int x = get<1>(instructions[i]); x <= get<2>(instructions[i]); ++x) {
        for(int y = get<3>(instructions[i]); y <= get<4>(instructions[i]); ++y) {
          for(int z = get<5>(instructions[i]); z <= get<6>(instructions[i]); ++z) {
            if(abs(x) <= 50 && abs(y) <= 50 && z <= 50) {
              get<0>(pp) = x;
              get<1>(pp) = y;
              get<2>(pp) = z;
              auto p = find(points.begin(), points.end(), pp);
              if(get<0>(instructions[i]) && p == points.end()) {
                points.emplace_back(x, y, z);
              } else if(!get<0>(instructions[i]) && p != points.end()) {
                points.erase(p);
              }
            }
          }
        }
      }
    }
    p1 = points.size();

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
  day22(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
