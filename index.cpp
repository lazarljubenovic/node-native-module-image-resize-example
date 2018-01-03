#include <nan.h>
#include <iostream>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define THUMB_SIZE 100

#define print(s) std::cout << s << std::endl
#define printHex(s) std::cout << std::hex << s << std::dec << std::endl

const int maxValue = 10;
int numberOfCalls = 0;

NAN_METHOD(invert) {
  Nan::Utf8String fileNameParam(info[0]->ToString());
  auto fileName = std::string(*fileNameParam);
  print(fileName);

  int x, y, n;
  unsigned char *data = stbi_load(fileName.c_str(), &x, &y, &n, 0);

  if (data == NULL) {
    // todo
    print("Data not read successfully. Exiting...");
    return;
  }

  const unsigned int length = x * y;
  int avgData[THUMB_SIZE][THUMB_SIZE][n] = {{{}}};
  int avgCount[THUMB_SIZE][THUMB_SIZE] = {{}};

  for (int i = 0; i < THUMB_SIZE; i++) {
    for (int j = 0; j < THUMB_SIZE; j++) {
      for (int k = 0; k < n; k++) {
        avgData[i][j][k] = 0;
      }
    }
  }

  for (int i = 0; i < THUMB_SIZE; i++) {
    for (int j = 0; j < THUMB_SIZE; j++) {
      avgCount[i][j] = 0;
    }
  }

  print(avgData[0][0][0]);
  print(avgData[10][11][2]);
  print(avgData[THUMB_SIZE - 1][THUMB_SIZE - 1][n - 1]);

  for (int k = 0; k < length; k++) {
    int pixelIndex = k * n;
    int i = k / x;
    int j = k % x;

    int avgI = THUMB_SIZE * i / y;
    int avgJ = THUMB_SIZE * j / x;

    avgCount[avgI][avgJ]++;
    for (int componentIndex = 0; componentIndex < n; componentIndex++) {
      avgData[avgI][avgJ][componentIndex] += data[pixelIndex + componentIndex];
    }
  }

  std::vector<char> newData;
  for (int i = 0; i < THUMB_SIZE; i++) {
    for (int j = 0; j < THUMB_SIZE; j++) {
      for (int k = 0; k < n; k++) {
        newData.push_back(avgData[i][j][k] / avgCount[i][j]);
      }
    }
  }

  std::string invertedFileName = "../invert/" + fileName;
  int status = stbi_write_jpg(invertedFileName.c_str(), THUMB_SIZE, THUMB_SIZE, n, newData.data(), 80);
  // print(status);

  if (status == 0) {
    // todo
    print("Writing status was 0. Exiting...");
    return;
  }

  stbi_image_free(data);
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, invert);
}

NODE_MODULE(addon, Initialize);
