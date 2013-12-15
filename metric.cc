#include "metric.h"

#include <assert.h>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "basic.h"

namespace {

#define sqr(x) ((x)*(x))

const string kIndent = "  ";

template <typename T>
void free_memory(T& t) {
  T tmp;
  t.swap(tmp);
}

string SimpleFtoa(float flt) {
  std::ostringstream strs;
  strs << flt;
  return strs.str();
}

void Rectangle(const vector<Point>& points, float* x_min, float* y_min,
    float* x_max, float* y_max) {
  assert(points.size() == 4
      && "Error: Point set should contains a rectangle.");
  vector<float> x, y;
  for (int i = 0; i < 4; ++i) {
    x.push_back(points[i].x);
    y.push_back(points[i].y);
  }
  assert((x.size() == 2 && y.size() == 2)
      && "Error: Point set should contains a rectangle.");
  *x_min = min(x[0], x[1]);
  *x_max = max(x[0], x[1]);
  *y_min = min(y[0], y[1]);
  *y_max = max(y[0], y[1]);
}

bool compare_attribute_value(const KeyValueMapper& std_attr,
    const KeyValueMapper& attr, const string& label) {
  if (std_attr.count(label) && attr.count(label)) {
    return std_attr.find(label)->second != attr.find(label)->second;
  }
}

float get_distance(const Point& p1, const Point& p2) {
  return sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
}

float compare_landmarks_value(const PointSet& std_land, const PointSet land) {
  if (std_land.size() == 0) return 0;
  float x_min = FLT_MAX, x_max = FLT_MIN;
  float cumulative_distance = 0;
  for (int i = 0; i < std_land.size(); ++i) {
    x_min = min(x_min, std_land[i].x);
    x_max = max(x_max, std_land[i].x);
    float min_distance = FLT_MAX;
    for (int j = 0; j < land.size(); ++j) {
      min_distance = min(min_distance, get_distance(std_land[i], land[j]));
    }
    cumulative_distance += min_distance;
  }
  return cumulative_distance / (x_max - x_min);
}

}  // namespace

Metric::Metric() {
}

Metric::~Metric() {
  free_memory(landmarks_);
  free_memory(detection_);
  free_memory(attributes_);
}

void Metric::get_sdk_results(const vector<string>& image_path) {
  get_sdk_landmarks(image_path);
  get_sdk_detection(image_path);
  get_sdk_attributes(image_path);
}

bool Metric::metric(const vector<PointSet>& std_landmarks,
                    const vector<PointSet>& std_detection,
                    const vector<KeyValueMapper>& std_attributes,
                    string& metric_results) {
  string landmark_results;
  metric_landmarks(std_landmarks, landmark_results);
  string detection_results;
  metric_detection(std_detection, detection_results);
  string attributes_results;
  metric_attributes(std_attributes, attributes_results);
  metric_results =
      "Lanmark_results:\n" + landmark_results +
      "Detection_results:\n" + detection_results +
      "Attributes_results:\n" + attributes_results;
}

bool Metric::metric_landmarks(const vector<PointSet>& std_landmarks,
                              string& metric_results) {
  assert(std_landmarks.size() == landmarks_.size()
      && "std_landmarks.size() != landmarks_.size()");
  metric_results = "";
  float average_error = 0;
  for (int i = 0; i < std_landmarks.size(); ++i) {
    float error = compare_landmarks_value(std_landmarks[i], landmarks_[i]);
    //metric_results += kIndent + kIndent + SimpleFtoa(error) + "\n";
    average_error += error;
  }
  average_error /= std_landmarks.size();
  metric_results += kIndent + "Average landmark error: " + SimpleFtoa(average_error) + "\n";
}

bool Metric::metric_detection(const vector<PointSet>& std_detection,
                              string& metric_results) {
  assert(std_detection.size() == detection_.size()
      && "std_detection.size() != detection_.size()");
  metric_results = "";
  float average_ratio = 0;
  for (int i = 0; i < std_detection.size(); ++i) {
    float x1min, y1min, x1max, y1max;
    float x2min, y2min, x2max, y2max;
    Rectangle(std_detection[i], &x1min, &y1min, &x1max, &y1max);
    Rectangle(detection_[i], &x2min, &y2min, &x2max, &y2max);
    float x_min = max(x1min, x2min), y_min = max(y1min, y2min);
    float x_max = min(x1max, x2max), y_max = min(y1max, y2max);

    float intersection_area = 0;
    if (x_min < x_max && y_min < y_max) {
      intersection_area = (x_max - x_min) * (y_max - y_min);
    }
    float combine_area = (x1max - x1min) * (y1max - y1min) +
        (x2max - x2min) * (y2max - y2min) - intersection_area;
    float ratio = intersection_area / combine_area;
    //metric_results += kIndent + kIndent + SimpleFtoa(ratio) + "\n";
    average_ratio += ratio;
  }
  average_ratio /= std_detection.size();
  metric_results += kIndent + "Average ratio: " + SimpleFtoa(average_ratio) + "\n";
}

bool Metric::metric_attributes(const vector<KeyValueMapper>& std_attributes,
                               string& metric_results) {
  assert(std_attributes.size() == attributes_.size()
      && "std_attributes.size() != attributes_.size()");
  metric_results = "";
  float gender_ratio = 0, age_ratio = 0, race_ratio = 0, glass_ratio = 0;
  for (int i = 0; i < std_attributes.size(); ++i) {
    gender_ratio += compare_attribute_value(std_attributes[i],
                                            attributes_[i],
                                            kGenderLabel);
    age_ratio += compare_attribute_value(std_attributes[i],
                                         attributes_[i],
                                         kAgeLabel);
    race_ratio += compare_attribute_value(std_attributes[i],
                                          attributes_[i],
                                          kRaceLabel);
    glass_ratio += compare_attribute_value(std_attributes[i],
                                           attributes_[i],
                                           kGlassLabel);
  }
  gender_ratio /= std_attributes.size();
  age_ratio /= std_attributes.size();
  race_ratio /= std_attributes.size();
  glass_ratio /= std_attributes.size();
  metric_results +=
      kIndent + "Gender false predict: " + SimpleFtoa(gender_ratio) + "\n" +
      kIndent + "Age false predict: " + SimpleFtoa(age_ratio) + "\n" +
      kIndent + "Race false predict: " + SimpleFtoa(race_ratio) + "\n" +
      kIndent + "Glass false predict: " + SimpleFtoa(glass_ratio) + "\n";
}
