
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <ctype.h>

using namespace std;

using paths=map<string,vector<string>>;

static void parse_file(fstream& file, paths& map)
{
  string line;
  while(getline(file, line)) {
    string from,to;
    unsigned idx = line.find('-');
    from = line.substr(0, idx);
    to = line.substr(idx+1);
    if(from != "end" && to != "start") {
      if(map.find(from) == map.end()) {
        pair<string,vector<string>> p = {from, {to}};
        map.insert(p);
      } else {
        map[from].push_back(to);
      }
    }
    if(from != "start" && to != "end") {
      if(map.find(to) == map.end()) {
        pair<string,vector<string>> p = {to, {from}};
        map.insert(p);
      } else {
        map[to].push_back(from);
      }
    }
  }
}

static pair<long,long> find_path(const paths& map, bool twice, vector<string> forbidden, const string& start)
{
  if(start == "end") {
    return {1, 1};
  }

  pair<long,long> res = {0, 0};

  if(islower(start[0]))
    forbidden.push_back(start);

  for(const string& s : map.at(start)) {
    bool t = twice;
    bool f = find(forbidden.cbegin(), forbidden.cend(), s) == forbidden.cend();
    if(twice || f) {
      t &= f;
      pair<long,long> p = find_path(map, t, forbidden, s);
      if(twice)
        res.first += p.first;
      res.second += p.second;
    }
  }
  return res;
}

static pair<long,long> find_paths(const paths& map)
{
  vector<string> forbidden;
  return find_path(map, true, forbidden, "start");
}

bool day12(long& p1, long& p2)
{
  fstream file;
  file.open("input/12", ios::in);
  if(file.is_open()) {
    paths map;

    parse_file(file, map);
    file.close();

    tie(p1, p2) = find_paths(map);

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
  day12(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
