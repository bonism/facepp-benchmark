#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <map>
#include <string>
#include <vector>

#include "basic.h"
#include "metric.h"

using namespace std;

// The main class for benchmarking.
class Benchmark {
 public:
  // The constructor calls init() to initialize.
  Benchmark(const string& input_json);
  // The destructor frees memory used by private variables.
  ~Benchmark();

  // Load data from database according to input configuration file.
  void init(const string& input_config);
  // Release data from database.
  void release();

  // Set the private metric class.
  void set_metric(Metric* metric);

  // Benchmarking one face analyzing SDK using the given metric.
  bool run_benchmark(string& output_results);

  // Return the private landmarks variable.
  vector<PointSet>* get_landmarks();
  // Set the private landmarks variable.
  void set_landmarks(vector<PointSet>* landmarks);

  // Return the private detection variable.
  vector<PointSet>* get_detection();
  // Set the private detection variable.
  void set_detection(vector<PointSet>* detection);

  // Return the private attributes variable.
  vector<KeyValueMapper>* get_attributes();
  // Set the private attributes variable.
  void set_attributes(vector<KeyValueMapper>* attributes);

 private:
  // A subclass of Metric class should be implemented.
  Metric* metric_;

  // The file paths to all images. List by order.
  vector<string> image_paths_;

  // Standard information of the images. List by order.
  vector<PointSet> landmarks_;
  vector<PointSet> detection_;
  vector<KeyValueMapper> attributes_;
};

#endif  // BENCHNARK_H
