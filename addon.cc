#include <node.h>
#include "ipvpn.h"

void InitAll(v8::Local<v8::Object> exports) {
  IpVPN::Init(exports);
};

NODE_MODULE(NODE_GYP_MODULE_NAME, InitAll)
