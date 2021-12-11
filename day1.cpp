#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
static int count_inc(vector<int> data, int window_size)
{
  int num_inc = 0;
  for(unsigned i = 0; i < data.size() - window_size; ++i)
    if(data[i] < data[i+window_size])
      ++num_inc;
  return num_inc;
}

bool day1(long& part1, long& part2)
{
  fstream file;
  file.open("input/1", ios::in);
  if(file.is_open()) {
    string line;
    vector<int> data;

    while(getline(file, line))
      data.push_back(stoi(line));

    part1 = count_inc(data, 1);
    part2 = count_inc(data, 3);
  } else {
    cout << "Couldn't open file";
    return false;
  }
  return true;
}
