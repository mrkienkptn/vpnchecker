#ifndef IP_H
#define IP_H
#include <node.h>
#include <node_object_wrap.h>
#include <sstream>

using namespace std;
using node::ObjectWrap;
using v8::FunctionCallbackInfo;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

struct IPRange
{
  unsigned int start;
  unsigned int end;
};

class IpVPN : public ObjectWrap
{
private:
  string dataPath;
  unsigned int totalIp = 0;

public:
  IPRange *ips;
  static void Init(Local<Object> exports);
  static void New(const FunctionCallbackInfo<Value> &args);
  static void IsVPN(const FunctionCallbackInfo<Value> &args);
  IpVPN(string dataPath);
  void initArray();
  void preProcessData();
  unsigned int ipv4ToNumber(string);
  bool isVPN(string ip);
};
#endif
