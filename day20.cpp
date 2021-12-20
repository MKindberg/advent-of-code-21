#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;
using Image = vector<string>;

static void parse_file(fstream &file, string &enhancement, Image &image) {
  string line;
  getline(file, enhancement);
  replace(enhancement.begin(), enhancement.end(), '.', '0');
  replace(enhancement.begin(), enhancement.end(), '#', '1');

  getline(file, line); // empty line
  while (getline(file, line)) {
    replace(line.begin(), line.end(), '.', '0');
    replace(line.begin(), line.end(), '#', '1');
    image.push_back(line);
  }
}

static char get(const vector<string> &image, int i, int j, char outside) {
  if (i < 0 || j < 0 || i >= (int)image.size() || j >= (int)image[0].size())
    return outside;
  return image[i][j];
}

static string concat_neighbours(const Image &image, int i, int j, char outside) {
  string num;
  for (int ii = -1; ii < 2; ++ii) {
    for (int jj = -1; jj < 2; ++jj) {
      num += get(image, i + ii - 1, j + jj - 1, outside);
    }
  }
  return num;
}

static void enhance(const string &enhancement, Image &image, unsigned times) {
  vector<string> enhanced;
  for (unsigned t = 0; t < times; ++t) {
    for (unsigned i = 0; i < image.size() + 2; ++i) {
      string row = "";
      for (unsigned j = 0; j < image[0].size() + 2; ++j) {
        string num = concat_neighbours(image, i - 1, j - 1, (t % 2) + '0');
        row += enhancement[stoi(num, nullptr, 2)];
      }
      enhanced.push_back(row);
    }
    image = std::move(enhanced);
    enhanced.clear();
  }
}

bool day20(long &p1, long &p2) {
  fstream file;
  string filename = "input/20";
  file.open(filename, ios::in);
  if (file.is_open()) {
    string enhancement;
    vector<string> image;

    parse_file(file, enhancement, image);
    file.close();
    enhance(enhancement, image, 2);
    p1 = transform_reduce(
        image.begin(), image.end(), 0, plus<int>(),
        [](auto row) { return count(row.begin(), row.end(), '1'); });

    enhance(enhancement, image, 48);
    p2 = transform_reduce(
        image.begin(), image.end(), 0, plus<int>(),
        [](auto row) { return count(row.begin(), row.end(), '1'); });

  } else {
    cout << "Couldn't open file" << endl;
    return false;
  }
  return true;
}

#ifdef MAIN
int main() {
  long p1, p2;
  day20(p1, p2);
  cout << p1 << endl;
  cout << p2 << endl;
  return 0;
}
#endif
