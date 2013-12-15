#include <cstdio>
#include <iostream>
using namespace std;

#include "basic.h"
#include "benchmark.h"
#include "metric_luxand.h"

int main() {
  Benchmark bm("./Database/Helen_partial.json");
  MetricLuxand ml;
  bm.set_metric(&ml);
  string output;
  bm.run_benchmark(output);
  cout << "Main's output: "<< output << endl;
  bm.release();
  return 0;
}
