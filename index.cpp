#include <nan.h>
#include <iostream>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define print(s) std::cout << s << std::endl
#define printHex(s) std::cout << std::hex << s << std::dec << std::endl

struct RGB {
  unsigned int R;
  unsigned int G;
  unsigned int B;
  unsigned int A;
  unsigned int count;
};

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
  auto avgData = new RGB[3][3];

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      RGB tmp_info = {};
      avgData[i][j] = tmp_info;
    }
  }

  for (int k = 0; k < length; k++) {
    int pixelIndex = k * n;
    int i = k / x;
    int j = k % x;

    int avgI = 3 * i / y;
    int avgJ = 3 * j / x;
    avgData[avgI][avgJ].count++;
    avgData[avgI][avgJ].R += data[pixelIndex];
    avgData[avgI][avgJ].G += data[pixelIndex + 1];
    avgData[avgI][avgJ].B += data[pixelIndex + 2];
    avgData[avgI][avgJ].A += data[pixelIndex + 3];
  }

  std::vector<char> newData;
  for (int i = 0; i < 3; i++) {
    print("I");
    print(i);
    for (int j = 0; j < 3; j++) {
      RGB tmp_info = avgData[i][j];
      print("Count");
      print(tmp_info.count);
      newData.push_back(tmp_info.R / tmp_info.count);
      newData.push_back(tmp_info.G / tmp_info.count);
      newData.push_back(tmp_info.B / tmp_info.count);
      newData.push_back(tmp_info.A / tmp_info.count);
      print((int)tmp_info.R);
    }
  }


  int status = stbi_write_jpg("./images/test.jpg", 3, 3, n, newData.data(), 80);
  print(status);

  if (status == 0) {
    // todo
    print("Writing status was 0. Exiting...");
    return;
  }

  stbi_image_free(data);

  // for (int i = 0; i < 3; i++) {
  //   delete avgData[i];
  // }
  // delete [] avgData;
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, invert);
}

NODE_MODULE(addon, Initialize);
