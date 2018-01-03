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

NAN_METHOD(invert) {

  // Typecheck the first (and only) argument from function call
  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Argument must be a string");
    return;
  }

  // First argument from JavaScript, should be a string
  Nan::Utf8String fileNameParam(info[0]->ToString());

  // Cast to std::string
  auto fileName = std::string(*fileNameParam);

  // Read image data by using stbi library
  // x, y -- dimensions in pixels
  // n    -- number of components (usually: red, green, blue, alpha)
  int x, y, n;
  unsigned char *data = stbi_load(fileName.c_str(), &x, &y, &n, 0);

  // Do not break if malformatted image
  if (data == NULL) {
    print("Image format not supported. Skipping this file...");
    return;
  }

  // number of pixels
  const unsigned int length = x * y;

  // first two dimensions are raster matrix
  // third dimension represents components of a single pixel (eg. rgba)
  int avgData[THUMB_SIZE][THUMB_SIZE][n] = {{{}}};

  // count of pixels (corresponds to avgData)
  int avgCount[THUMB_SIZE][THUMB_SIZE] = {{}};

  // Set everything to zero
  for (int i = 0; i < THUMB_SIZE; i++) {
    for (int j = 0; j < THUMB_SIZE; j++) {
      avgCount[i][j] = 0;
      for (int k = 0; k < n; k++) {
        avgData[i][j][k] = 0;
      }
    }
  }

  // Main logic
  // k goes through all pixels (loop length is number of pixels)
  for (int k = 0; k < length; k++) {
    // "pointer" to start of pixel data (rgba...) in "data"
    int pixelIndex = k * n;

    // position of pixel in raster matrix
    int i = k / x;
    int j = k % x;

    // we need to divide rows and columns in THUMB_SIZE equal areas
    int avgI = THUMB_SIZE * i / y;
    int avgJ = THUMB_SIZE * j / x;

    // we keep count of number of pixels in each area
    avgCount[avgI][avgJ]++;

    // we keep sum of each component for every area
    for (int componentIndex = 0; componentIndex < n; componentIndex++) {
      avgData[avgI][avgJ][componentIndex] += data[pixelIndex + componentIndex];
    }
  }

  // data for new (resized) image where we push sum/count (=avg)
  std::vector<char> newData;
  for (int i = 0; i < THUMB_SIZE; i++) {
    for (int j = 0; j < THUMB_SIZE; j++) {
      for (int k = 0; k < n; k++) {
        newData.push_back(avgData[i][j][k] / avgCount[i][j]);
      }
    }
  }

  // Save resized image into a folder next to folder of images
  std::string resizedFileName = "../resized/" + fileName;
  int status = stbi_write_jpg(resizedFileName.c_str(), THUMB_SIZE, THUMB_SIZE, n, newData.data(), 80);

  if (status == 0) {
    print("There was an error saving the image. Skipping...");
    return;
  }

  stbi_image_free(data);
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, invert);
}

NODE_MODULE(addon, Initialize);
