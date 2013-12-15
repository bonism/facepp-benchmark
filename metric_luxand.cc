#include "metric_luxand.h"

#include <assert.h>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

#include "basic.h"
#include "SDK/Luxand/include/LuxandFaceSDK.h"

namespace {

const bool kDetermineFaceRotationAngle = false;
const bool kHandleArbitraryRotations = false;
const int kInternalResizeWidth = 384;
const string kLuxandFaceSdkKey = "BITeFoJYb3xbVpDkF9psrtssvN7YhpfwdLG+JLgsy0u5eY5nqjmJB6ge//PkpgW8N8B2kSZhE/GjZnAuPsQxbaQ7LOV/H1PaJFCg2ecsauYWQPWQJlUsQdiRvVNgFiKoxpqpWxXCZRpsi9OhG72FZrUfcy+s3ZxcoAKbJVWscq8=";

}  // namespace

MetricLuxand::MetricLuxand() {
  FSDK_ActivateLibrary(kLuxandFaceSdkKey.c_str());
  
  assert(FSDK_ActivateLibrary(kLuxandFaceSdkKey.c_str()) == FSDKE_OK
      && "Error: Activate Luxand Face SDK failed!\n");
  assert(FSDK_Initialize("") == FSDKE_OK
      && "Error: Initialize Luxand Face SDK failed!\n");
  FSDK_SetFaceDetectionParameters(kHandleArbitraryRotations,
                                  kDetermineFaceRotationAngle,
                                  kInternalResizeWidth);
}

void MetricLuxand::get_sdk_landmarks(const vector<string>& image_path) {
  for (int i = 0; i < image_path.size(); ++i) {
    HImage imageHandle;
    assert(FSDK_LoadImageFromFile(&imageHandle, image_path[i].c_str()) == FSDKE_OK
        && "Error: Luxand loads file error!\n");
    FSDK_Features facialFeatures;
    FSDK_DetectFacialFeatures(imageHandle, &facialFeatures);
    PointSet point_set;
    for (int j = 0; j < FSDK_FACIAL_FEATURE_COUNT; ++j) {
      point_set.push_back(Point(facialFeatures[j].x, facialFeatures[j].y));
    }
    landmarks_.push_back(point_set);
  }
}

void MetricLuxand::get_sdk_detection(const vector<string>& image_path) {
  /*for (int i = 0; i < image_path.size(); ++i) {
    HImage imageHandle;
    assert(FSDK_LoadImageFromFile(&imageHandle, image_path[i].c_str()) == FSDKE_OK
        && "Error: Luxand loads file error!\n");
    TFacePosition facePosition;
    Point point[4];
    if (FSDK_DetectFace(imageHandle, &facePosition) != FSDKE_OK) {
      printf("Warning: No faces detected.\n"); 
    } else {
      float left = facePosition.xc - facePosition.w / 2.0;
      float top = facePosition.yc - facePosition.w / 2.0;
      float right = facePosition.xc + facePosition.w / 2.0;
      float bottom = facePosition.yc + facePosition.w / 2.0;
      point[0] = Point(left, top);
      point[1] = Point(left, bottom);
      point[2] = Point(right, top);
      point[3] = Point(right, bottom);
    }
    PointSet point_set;
    for (int j = 0; j < 4; ++j) {
      point_set.push_back(point[j]);
    }
    detection_.push_back(point_set);
  }*/
}

void MetricLuxand::get_sdk_attributes(const vector<string>& image_path) {
}
