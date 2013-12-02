#include "benchmark.h"

#include <string>
#include <vector>

#include "jsonsl.h"

using namespace std;

namespace {

template <typename T>
void free_memory(T& t) {
  T tmp;
  t.swap(tmp);
}

}

Benchmark::Benchmark(const string& input_json) {
  init(input_json);
}

Benchmark::~Benchmark() {
  release();
}

void Benchmark::init(const string& input_config) {
  
}

void Benchmark::release() {
  delete metric_;

  free_memory(*image_paths_);
  delete image_paths_;

  free_memory(*landmarks_);
  delete landmarks_;

  free_memory(*detection_);
  delete detection_;

  free_memory(*attributes_);
  delete attributes_;
}

void Benchmark::set_metric(Metric* metric) {
  metric_ = metric;
}

bool Benchmark::run_benchmark(const string& output_results) {
  vector<string>* image_path;
  vector<PointSet>* landmarks_;
  vector<PointSet>* detection_;
  vector<KeyValueMapper>* attributes_;
}

vector<PointSet>* Benchmark::get_landmarks() {
  return landmarks_;
}

void Benchmark::set_landmarks(vector<PointSet>* landmarks) {
  landmarks_ = landmarks;
}

vector<PointSet>* Benchmark::get_detection() {
  return detection_;
}

void Benchmark::set_detection(vector<PointSet>* detection) {
  detection_ = detection;
}

vector<KeyValueMapper>* Benchmark::get_attributes() {
  return attributes_;
}

void Benchmark::set_attributes(vector<KeyValueMapper>* attributes) {
  attributes_ = attributes;
}
