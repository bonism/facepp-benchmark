// This header defines the basic data types used during benchmarking.

#ifndef BASIC_H
#define BASIC_H

#include <map>
#include <string>
#include <vector>
using namespace std;

/********************************* Data Types *********************************/

const string kGenderLabel = "gender";
const string kAgeLabel = "age";
const string kRaceLabel = "race";
const string kGlassLabel = "glass";

struct Point {
  float x, y;
};

typedef vector<Point> PointSet;

typedef map<string, string> KeyValueMapper;

#endif
