#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static unsigned flip_bits(unsigned n, unsigned l)
{
  return ~n & ((1 << l) - 1);
}

static char most_common(const vector<string>& numbers, int bit)
{
  int count = 0;
  for(unsigned i = 0; i < numbers.size(); ++i)
    if(numbers[i][bit] == '1')
      ++count;
    else
      --count;
  if(count >= 0)
    return '1';
  return '0';
}

static int part1(const vector<string>& numbers)
{
  int size = numbers[0].length();
  string n = "";
  for(int i = 0; i < size; ++i)
    n += most_common(numbers, i);
  unsigned gamma = stoi(n, nullptr, 2);
  unsigned epsilon = flip_bits(gamma, size);

  return gamma * epsilon;
}

static int part2(const vector<string>& numbers)
{
  int size = numbers[0].length();

  vector<string> oxygen = numbers;
  vector<string> co2 = numbers;

  for(int i = 0; i < size; ++i)
  {
    if(oxygen.size() > 1) {
    char m = most_common(oxygen, i);
    auto e = remove_if(begin(oxygen), end(oxygen), [m, i](string s){return s[i] != m;});
    oxygen.erase(e, end(oxygen));
    }

    if(co2.size() > 1) {
      char m = most_common(co2, i);
      auto e = remove_if(begin(co2), end(co2), [m, i](string s){return s[i] == m;});
      co2.erase(e, end(co2));
    }
  }

  assert(oxygen.size() == 1);
  assert(co2.size() == 1);

  return stoi(oxygen[0], nullptr, 2) * stoi(co2[0], nullptr, 2);
}

bool day3(long& p1, long& p2)
{
  fstream file;
  file.open("input/3", ios::in);
  if(file.is_open()) {
    string line;
    vector<string> numbers;
    while(getline(file, line)) {
      numbers.push_back(line);
    }

    p1 = part1(numbers);

    p2 = part2(numbers);

  } else {
    cout << "Couldn't open file";
    return false;
  }
  return true;
}
