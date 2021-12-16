
#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <numeric>
#include <cassert>

using namespace std;

struct packet{
  int version;
  int type;
  long value;
  vector<packet> content;
};

static void parse_file(fstream& file, string& bits)
{
  string line;
  getline(file, line);
  for(unsigned c = 0; c < line.size(); ++c) {
    int i = stoi(line.substr(c, 1), nullptr, 16);
    bits += bitset<4>(i).to_string();
  }
}

int get_packet(const string& bits, int start, vector<packet>& packets)
{
  int len = 0;
  packet p;
  p.version = stoi(bits.substr(start + len, 3), nullptr, 2);
  len += 3;
  p.type = stoi(bits.substr(start + len, 3), nullptr, 2);
  len += 3;
  if(p.type == 4) {
    int cont = 0;
    string s;
    do {
      cont = stoi(bits.substr(start + len, 1), nullptr, 2);
      len += 1;
      s += bits.substr(start + len, 4);
      len += 4;
    } while(cont == 1);
    p.value = stol(s, nullptr, 2);
  } else {
    int lengthId = stoi(bits.substr(start + len, 1), nullptr, 2);
    len += 1;
    if(lengthId == 0) {
      int end = len + 15 + stoi(bits.substr(start + len, 15), nullptr, 2);
      len += 15;
      while(len < end) {
        len += get_packet(bits, start+len, p.content);
      }
    } else {
      int num = stoi(bits.substr(start + len, 11), nullptr, 2);
      len += 11;
      for(unsigned n = 0; n < num; ++n) {
        len += get_packet(bits, start+len, p.content);
      }
    }
  }
  packets.push_back(p);
  return len;
}

int count_versions(const vector<packet>& packets)
{
  int count = 0;
  for(packet p : packets) {
    count += p.version;
    if(p.type != 4)
      count += count_versions(p.content);
  }
  return count;
}

void get_packets(const string& bits, vector<packet>& packets)
{
  int min_packet_len = 11;
  for(unsigned i = 0; i < bits.size()-min_packet_len;) {
    i += get_packet(bits, i, packets);
    if(i%4 != 0) {
      i += 4-(i%4);
    }
  }
}

long parse_packet(packet p, int level = 0)
{
  vector<long> vals;
  for(packet pp : p.content)
    vals.push_back(parse_packet(pp, level+1));
  switch(p.type) {
    case 0:
      return accumulate(vals.begin(), vals.end(), 0L, plus<long>());
    case 1:
      return accumulate(vals.begin(), vals.end(), 1L, multiplies<long>());
    case 2:
      return *min_element(vals.begin(), vals.end());
    case 3:
      return *max_element(vals.begin(), vals.end());
    case 4:
      return p.value;
    case 5:
      return vals[0] > vals[1];
    case 6:
      return vals[0] < vals[1];
    case 7:
      return vals[0] == vals[1];
  }
  cout << "Fail" << endl;
  return -1;
}

bool day16(long& p1, long& p2)
{
  fstream file;
  string filename = "input/16";
  //filename = "test";
  file.open(filename, ios::in);
  if(file.is_open()) {
    string bits;
    vector<packet> packets;

    parse_file(file, bits);
    file.close();

    get_packets(bits, packets);
    p1 = count_versions(packets);

    p2 = parse_packet(packets[0]);

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
  day16(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
