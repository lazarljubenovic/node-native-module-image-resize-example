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

const int maxValue = 10;
int numberOfCalls = 0;

NAN_METHOD(invert) {
  Nan::Utf8String fileNameParam(info[0]->ToString());
  auto fileName = std::string(*fileNameParam);
  print(fileName);

  int x, y, n;
  unsigned char *data = stbi_load(fileName.c_str(), &x, &y, &n, 0);
  const unsigned int length = x * y;

  std::vector<char> newData;
  for (int i = 0; i < length; i++) {
    newData.push_back(0xFF - data[n * i]);
    newData.push_back(0xFF - data[n * i + 1]);
    newData.push_back(0xFF - data[n * i + 2]);
    newData.push_back(data[n * i + 3]);
  }

  int status = stbi_write_jpg("./images/test.jpg", x, y, n, newData.data(), 80);
  print(status);
  // auto retVal = Nan::CopyBuffer((char *) newData.data(), newData.size()).ToLocalChecked();
  // info.GetReturnValue().Set(retVal);

  stbi_image_free(data);
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, invert);
}

NODE_MODULE(addon, Initialize);
