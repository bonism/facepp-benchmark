#ifndef METRIC_LUXAND_H
#define METRIC_LUXAND_H

#include "metric.h"

class MetricLuxand : public Metric {
 public:
  MetricLuxand();

  void get_sdk_landmarks(const vector<string>& image_path);
  void get_sdk_detection(const vector<string>& image_path);
  void get_sdk_attributes(const vector<string>& image_path);
};

#endif  // METRIC_LUXAND_H
