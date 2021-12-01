#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int count_inc(vector<int> data, int window_size)
{
  int num_inc = 0;
  for(int i = 0; i < data.size() - window_size; ++i)
    if(data[i] < data[i+window_size])
      ++num_inc;
  return num_inc;
}

int main()
{
  fstream file;
  file.open("input.txt", ios::in);
  if(file.is_open()) {
    string line;
    vector<int> data;
    while(getline(file, line))
      data.push_back(stoi(line));
    cout << "Depth increses " << count_inc(data, 1) << " times" << endl;
    cout << "Depth increses " << count_inc(data, 3) << " times with sliding window size 3" << endl;
  } else {
    cout << "Couldn't open file";
  }
}
