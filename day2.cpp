#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
  fstream file;
  file.open("input.txt", ios::in);
  if(file.is_open()) {
    string line;
    int depth1 = 0;
    int depth2 = 0;
    int horizontal = 0;
    int aim = 0;
    while(getline(file, line)) {
      size_t i = line.find(' ');
      if(line.substr(0, i) == "forward"){
        horizontal += stoi(line.substr(i+1, line.length()));
        depth2 += aim * stoi(line.substr(i+1, line.length()));
      } else if(line.substr(0, i) == "down") {
        depth1 += stoi(line.substr(i+1, line.length()));
        aim += stoi(line.substr(i+1, line.length()));
      }
      else if(line.substr(0, i) == "up") {
        depth1 -= stoi(line.substr(i+1, line.length()));
        aim -= stoi(line.substr(i+1, line.length()));
      }
    }
    cout << depth1*horizontal << endl;
    cout << depth2*horizontal << endl;

  } else {
    cout << "Couldn't open file";
  }
}
