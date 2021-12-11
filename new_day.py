#!/usr/bin/python3

import os
import subprocess

template = """
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

static void parse_file(fstream& file, vector<string>& lines)
{{
  string line;
  while(getline(file, line)) {{
    lines.push_back(line);
  }}
}}

bool day{day}(long& p1, long& p2)
{{
  fstream file;
  file.open("input/{day}", ios::in);
  if(file.is_open()) {{
    vector<string> lines;

    parse_file(file, lines);
    file.close();

  }} else {{
    cout << "Couldn't open file" << endl;
    return false;
  }}
  return true;
}}
"""
days = [int(f.split('.')[0][3:]) for f in os.listdir('.') if f[0:3] == "day" and f[-4:] == ".cpp"]
new_day = max(days) + 1

subprocess.run(["./download", str(new_day)])

f = open("day{}.cpp".format(new_day), "w")
f.write(template.format(day = new_day))
f.close()

print("Created day{}.cpp".format(new_day))
