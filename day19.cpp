// runCmd: make 19
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stack>
#include <vector>

using namespace std;

struct coord {
  int x, y, z;
  int rot;

  coord(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
    rot = 0;
  }

  unsigned dist(const coord c) const {
    return std::abs(x - c.x) + std::abs(y - c.y) + std::abs(z - c.z);
  }

  void rotateX() {
    int tmp = y;
    y = -z;
    z = tmp;
  }
  void rotateY() {
    int tmp = z;
    z = -x;
    x = tmp;
  }
  void rotateZ() {
    int tmp = x;
    x = -y;
    y = tmp;
  }
  int rotate() {
    tuple rotation = {rot%4, (rot%16)/4, (rot%64)/16};
    ++rot;
    if(rot == 64)
      rot = 0;
    if((get<0>(rotation) +1) % 4 == 0) {
      if((get<1>(rotation) +1) % 4 == 0) {
          rotateX();
          rotateY();
          rotateZ();
      } else {
        rotateX();
        rotateY();
      }
    } else {
      rotateX();
    }

    return rot;
  }

  bool operator< (coord c) {
    if(x != c.x)
      return x < c.x;
    if(y != c.y)
      return y < c.y;
    return z < c.z;
  }

  bool operator> (coord c) {
    if(x != c.x)
      return x > c.x;
    if(y != c.y)
      return y > c.y;
    return z > c.z;
  }

  bool operator== (coord c) {
    return x == c.x && y == c.y && z == c.z;
  }

  bool operator!= (coord c) {
    return !((*this) == c);
  }

  void operator+= (tuple<int,int,int> offset) {
    x += get<0>(offset);
    y += get<1>(offset);
    z += get<2>(offset);
  }

  void print() { printf("(%d,%d,%d)\n", x, y, z); }
};

struct Scanner {
  vector<coord> beacons;

  void clear() { beacons.clear(); }

  void add_beacon(int x, int y, int z) { beacons.emplace_back(x, y, z); }

  tuple<int, int, int> matching(Scanner& s) {
    tuple<int, int, vector<unsigned>> data;
    vector<unsigned> d1;
    vector<unsigned> d2;
    vector<unsigned> matching;

    for (unsigned i = 0; i < beacons.size(); ++i) {
      d1.clear();
      for (unsigned j = 0; j < s.beacons.size(); ++j) {
        d2.clear();
        matching.clear();
        transform(beacons.begin(), beacons.end(), back_inserter(d1),
                  [b = beacons[i]](coord c) { return b.dist(c); });
        transform(s.beacons.begin(), s.beacons.end(), back_inserter(d2),
                  [b = s.beacons[j]](coord c) { return b.dist(c); });

        sort(d1.begin(), d1.end());
        sort(d2.begin(), d2.end());
        set_intersection(d1.begin(), d1.end(), d2.begin(), d2.end(),
                         back_inserter(matching));
        if (matching.size() >= 12) {
          cout << "Matches" << endl;
          for(unsigned k = 0; k < 64; ++k) {
            int rotation = s.rotate();
            tuple<int,int,int> offset = {beacons[i].x-s.beacons[j].x, beacons[i].y-s.beacons[j].y, beacons[i].z-s.beacons[j].z};
            int n = num_equal(s, offset);
            cout << n << " Equal" << endl;
            if(n >=12) {
              cout << "Matches2" << endl;
              return {i, j, rotation};
            }
          }
          return {-1,-1,-1};
        }
      }
    }
    return {-1, -1, -1};
  }

  unsigned num_equal(Scanner s, tuple<int,int,int> offset) {
    int matching = 0;
    sort(beacons.begin(), beacons.end());
    s += offset;
    sort(s.beacons.begin(), s.beacons.end());
    for(auto c : beacons)
      for(auto b : s.beacons)
        if(c == b) {
          ++matching;
          break;
        }

    return matching;
  }

  void operator+= (tuple<int,int,int> offset) {
    for(coord& c : beacons) {
      c += offset;
    }
  }

  void merge(Scanner s, tuple<int,int,int> data) {
    for(int i = 0; i < get<2>(data); ++i) {
      s.rotate();
    }
    int i = get<0>(data);
    int j = get<1>(data);
    tuple<int,int,int> offset = {beacons[i].x-s.beacons[j].x, beacons[i].y-s.beacons[j].y, beacons[i].z-s.beacons[j].z};
    s += offset;
    sort(beacons.begin(), beacons.end());
    sort(s.beacons.begin(), s.beacons.end());
    set_difference(s.beacons.begin(), s.beacons.end(), beacons.begin(), beacons.end(), back_inserter(beacons));
  }
  int rotate() {
    int r;
    for(coord& c : beacons) {
      r = c.rotate();
    }
    cout << r << endl;
    return r;
  }
  void print(){
    for(coord c : beacons)
      c.print();
  }
};

static void parse_file(fstream &file, vector<Scanner> &scanners) {
  string line;
  Scanner s;
  int x, y, z;
  while (getline(file, line)) {
    if (line.substr(0, 3) == "---")
      continue;
    if (line.size() == 0) {
      scanners.push_back(s);
      s.clear();
      continue;
    }
    sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z);
    s.add_beacon(x, y, z);
  }
  scanners.push_back(s);
}

bool day19(long &p1, long &p2) {
  fstream file;
  string filename = "input/19";
  filename = "test";
  file.open(filename, ios::in);
  if (file.is_open()) {
    vector<Scanner> scanners;

    parse_file(file, scanners);
    file.close();
    //    assert(scanners[3].beacons[3].z == 481);

    scanners[0].print();
    cout << endl;
    for(unsigned i = 1; i < scanners.size(); ++i) {
      while(scanners[i].beacons[0] != scanners[0].beacons[0])
        scanners[i].rotate();
      scanners[i].print();
      cout << endl;
    }

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

#ifdef MAIN
int main() {
  long p1, p2;
  day19(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
