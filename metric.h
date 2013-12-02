#ifndef METRIC_H
#define METRIC_H

#include <map>
#include <string>
#include <vector>

#include "basic.h"

using namespace std;

// An abstract class for calling an face analyzing APIs.
// A subclass of Metric class should be implemented for each face analyzing SDK.
class Metric {
 public:
  // An empty constructor.
  // An override constructor should be implemented in the subclass.
  Metric();
  // The desctructor frees the memory used by private variables.
  ~Metric();

  // Call the APIs and saves the results to private variables.
  virtual void get_sdk_results(const vector<string>* image_path) = 0;
  virtual void get_sdk_landmarks(const vector<string>* image_path) = 0;
  virtual void get_sdk_detection(const vector<string>* image_path) = 0;
  virtual void get_sdk_attributes(const vector<string>* image_path) = 0;

  // Evaluate the SDK results by compareing the standard results and the results
  // generated by calling the SDK's APIs.
  // These methods can be overrided for specific SDK.
  bool metric_landmarks(const vector<PointSet>* std_landmarks,
                        string& metric_results);
  bool metric_detection(const vector<PointSet>* std_detection,
                        string& metric_results);
  bool metric_attributes(const vector<KeyValueMapper>* std_attributes,
                         string& metric_results);

 private:
  // These variables contains the results of the SDK.
  vector<PointSet>* landmarks_;
  vector<PointSet>* detection_;
  vector<KeyValueMapper>* attributes_;
};

#endif
