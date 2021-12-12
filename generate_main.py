#!/usr/bin/python3

import os

template = """
#include <chrono>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

{decl}

vector<function<bool(long&,long&)>> days = {{
  {days}
}};

int main()
{{
  long p1, p2;
  int i = 1;
  size_t total_time = 0;
  printf("%s\\t%15s\\t%15s\\t\\t%s\\n", "Day", "Part 1", "Part 2", "Time");
  const int runs = 50;
  for(auto d : days){{
    auto start = high_resolution_clock::now();
    for(int i = 0; i < runs; ++i)
      if(!d(p1, p2)){{
        cout << "Day " << i << " failed!" << endl;
      }}
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    total_time += duration.count()/runs;
    printf("%u\\t%15ld\\t%15ld\\t\\t%lu\\n", i, p1, p2, duration.count()/runs);
    ++i;
  }}
  cout << "Total time: " << total_time << "µs" << endl;
  return 0;
}}
"""
days = [int(f.split('.')[0][3:]) for f in os.listdir('.') if f[0:3] == "day" and f[-4:] == ".cpp"]
days = sorted(days)
days = ["day"+str(d) for d in days]
decl = ["bool {}(long& part1, long& part2);".format(d) for d in days]

f = open("main.cpp", "w")
f.write(template.format(decl = '\n'.join(decl), days = ',\n  '.join(days)))
f.close()
