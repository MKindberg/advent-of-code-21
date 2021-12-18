// runCmd: make 18
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

class Number {
  int num1, num2;
  Number *sub1, *sub2;
  int depth;

public:
  Number(Number* n) {
    num1 = n->num1;
    num2 = n->num2;
    if(num1 == -1)
      sub1 = new Number(n->sub1);
    if(num2 == -1)
      sub2 = new Number(n->sub2);
    depth = n->depth;
  }

  Number(int depth) {
    num1 = -1;
    num2 = -1;
    this->depth = depth;
  }

  Number(int depth, int n1, int n2) {
    num1 = n1;
    num2 = n2;
    this->depth = depth;
  }

  ~Number() {
    if (num1 == -1)
      delete sub1;
    if (num2 == -1)
      delete sub2;
  }

  void inc_depth() {
    ++depth;
    if(num1 == -1)
      sub1->inc_depth();
    if(num2 == -1)
      sub2->inc_depth();
  }

  int magnitude()
  {
    int m1 = num1;
    int m2 = num2;
    if(m1 == -1)
      m1 = sub1->magnitude();
    if(m2 == -1)
      m2 = sub2->magnitude();
    return m1*3 + m2*2;
  }

  void add(Number* n) {
    sub1 = new Number(this);
    sub2 = new Number(n);
    num1 = -1;
    num2 = -1;
    depth = -1;
    inc_depth();
    reduce();
  }

  int parse_num(string s) {
    int len = 0;
    assert(s[len++] == '[');
    if (s[len] == '[') {
      sub1 = new Number(depth + 1);
      len += sub1->parse_num(s.substr(len));
    } else {
      num1 = s[len++] - '0';
    }
    assert(s[len++] == ',');
    if (s[len] == '[') {
      sub2 = new Number(depth + 1);
      len += sub2->parse_num(s.substr(len));
    } else {
      num2 = s[len++] - '0';
    }
    assert(s[len++] == ']');
    return len;
  }

  void add_left(int n, bool force = false)
  {
    if(force && num1 == -1) {
      sub1->add_left(n, true);
    } else if(num1 == -1) {
      sub1->add_right(n, true);
    } else {
      num1 += n;
    }
  }
  void add_right(int n, bool force = false)
  {
    if(force && num2 == -1) {
      sub2->add_right(n, true);
    } else if(num2 == -1) {
      sub2->add_left(n, true);
    } else {
      num2 += n;
    }
  }

  bool explode(int &n1, int &n2) {
    if (depth == 4) {
      n1 = num1;
      n2 = num2;
      return true;
    }
    int left, right;
    if (num1 == -1) {
      if (sub1->explode(left, right)) {
        if(right != -1) {
          add_right(right);
          right = -1;
        }
        if(depth == 3) {
          num1 = 0;
          delete sub1;
          sub1 = nullptr;
        }
        n1 = left;
        n2 = right;
        return true;
      }
    }
    if (num2 == -1) {
      if (sub2->explode(left, right)) {
        if(left != -1) {
          add_left(left);
          left = -1;
        }
        if(depth == 3) {
          num2 = 0;
          delete sub2;
          sub2 = nullptr;
        }
        n1 = left;
        n2 = right;
        return true;
      }
    }
    return false;
  }

  bool split() {
    if (num1 > 9) {
      sub1 = new Number(depth + 1, num1 / 2, (num1 + 1) / 2);
      num1 = -1;
      return true;
    }
    if (num1 == -1)
      if (sub1->split())
        return true;

    if (num2 > 9) {
      sub2 = new Number(depth + 1, num2 / 2, (num2 + 1) / 2);
      num2 = -1;
      return true;
    }
    if (num2 == -1)
      if (sub2->split())
        return true;
    return false;
  }

  bool reduce1() {
    int left, right;
    return explode(left, right) || split();
  }
  void reduce() {
    while(reduce1());
  }

  void print() {
    /* printf("(%d)", depth); */
    cout << '[';
    if (num1 == -1)
      sub1->print();
    else
      cout << num1;
    cout << ',';
    if (num2 == -1)
      sub2->print();
    else
      cout << num2;
    cout << ']';
  }
};

static void parse_file(fstream &file, vector<Number *> &nums) {
  string line;
  for (unsigned i = 0; getline(file, line); ++i) {
    nums.push_back(new Number(0));
    nums[i]->parse_num(line);
  }
}

bool day18(long &p1, long &p2) {
  fstream file;
  string filename = "input/18";
  //filename = "test";
  file.open(filename, ios::in);
  if (file.is_open()) {
    p1 = 0;
    p2 = 0;
    vector<Number *> nums;

    parse_file(file, nums);
    file.close();


    Number* n = new Number(nums[0]);
    for (unsigned i = 1; i < nums.size(); ++i) {
      n->add(nums[i]);
    }
    n->print();
    cout << endl;
    p1 = n->magnitude();
    delete n;

    for(unsigned i = 0; i < nums.size(); ++i) {
      for(unsigned j = 0; j < nums.size(); ++j) {
        if(i != j) {
          n = new Number(nums[i]);
          n->add(nums[j]);
          int m = n->magnitude();
          if(p2 < m)
            p2 = m;
        }
      }
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
  day18(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
