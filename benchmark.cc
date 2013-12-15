#include "benchmark.h"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "json.h"

using namespace std;

namespace {

template <typename T>
void free_memory(T& t) {
  T tmp;
  t.swap(tmp);
}

void get_point_set(json_value* value, PointSet& point_set) {
  assert(value->type == json_array && "Error: Expect json_array.");
  point_set.clear();
  for (int k = 0; k < value->u.array.length; ++k) {
    if (value->u.array.values[k]->type != json_array ||
        value->u.array.values[k]->u.array.length != 2 ||
        value->u.array.values[k]->u.array.values[0]->type != json_double ||
        value->u.array.values[k]->u.array.values[1]->type != json_double) {
      printf("Error: Invalid coordinate founded.\n");
      return;
    }
    Point point;
    point.x = value->u.array.values[k]->u.array.values[0]->u.dbl;
    point.y = value->u.array.values[k]->u.array.values[1]->u.dbl;
    point_set.push_back(point);
  }
}

void get_key_value_mapper(json_value* value, KeyValueMapper& mapper) {
  assert(value->type == json_array && "Error: Expect json_array.");
  mapper.clear();
  for (int k = 0; k < value->u.array.length; ++k) {
    assert(value->u.array.values[k]->type == json_object
        && "Error: Invalid key_value pair founded.\n");
    for (int l = 0; l < value->u.array.values[k]->u.object.length; ++l) {
      assert(value->u.array.values[k]->u.object.values[l].value->type == json_string
          && "Error: Invalid value founded.\n");
      string key = value->u.array.values[k]->u.object.values[l].name;
      string val = value->u.array.values[k]->u.object.values[l].value->u.string.ptr;
      mapper[key] = val;
    }
  }
}

}  // namespace

Benchmark::Benchmark(const string& input_json) {
  init(input_json);
}

Benchmark::~Benchmark() {
  release();
}

void Benchmark::init(const string& input_config) {
  // Read JSON
  ifstream json_file(input_config.c_str());
  assert(json_file.is_open());
  string content, line;
  while (getline(json_file, line)) {
    content += line;
  }
  json_file.close();
  // Parse JSON
  json_value* data = json_parse(content.c_str(), content.size());
  assert(data != NULL && "Error: nullptr!\n");
  assert(data->type == json_array
      && "Error: The input data should be a json_array!\n");
  for (int i = 0; i < data->u.array.length; ++i) {
    assert(data->u.array.values[i]->type == json_object
        && "Error: Each input data should be a json_object!\n");
    for (int j = 0; j < data->u.array.values[i]->u.object.length; ++j) {
      string label = data->u.array.values[i]->u.object.values[j].name;
      json_value* value = (data->u.array.values[i]->u.object.values[j].value);
      if (label == "picture_path") {
        assert(value->type == json_string
            && "Error: Expect json_string as picture_path!\n");
        image_paths_.push_back(value->u.string.ptr);
      } else if (label == "landmarks") {
        PointSet point_set;
        get_point_set(value, point_set);
        landmarks_.push_back(point_set);
      } else if (label == "detection") {
        PointSet point_set;
        get_point_set(value, point_set);
        detection_.push_back(point_set);
      } else if (label == "attributes") {
        KeyValueMapper mapper;
        get_key_value_mapper(value, mapper);
        attributes_.push_back(mapper);
      } else {
        printf("Warning: Unsupported JSON label %s\n", label.c_str());
      }
    }
  }
}

void Benchmark::release() {
  free_memory(image_paths_);
  free_memory(landmarks_);
  free_memory(detection_);
  free_memory(attributes_);
}

void Benchmark::set_metric(Metric* metric) {
  metric_ = metric;
}

bool Benchmark::run_benchmark(string& output_results) {
  metric_->get_sdk_results(image_paths_);
  return metric_->metric(landmarks_, detection_, attributes_, output_results);
}

vector<PointSet>* Benchmark::get_landmarks() {
  return &landmarks_;
}

void Benchmark::set_landmarks(vector<PointSet>* landmarks) {
  landmarks_ = *landmarks;
}

vector<PointSet>* Benchmark::get_detection() {
  return &detection_;
}

void Benchmark::set_detection(vector<PointSet>* detection) {
  detection_ = *detection;
}

vector<KeyValueMapper>* Benchmark::get_attributes() {
  return &attributes_;
}

void Benchmark::set_attributes(vector<KeyValueMapper>* attributes) {
  attributes_ = *attributes;
}
