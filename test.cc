#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "node.h"
#include "ipvpn.h"

using namespace std;
using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::ObjectTemplate;
using v8::String;
using v8::Value;

void IpVPN::Init(Local<Object> exports) {
Isolate *isolate = exports->GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  Local<ObjectTemplate> addon_data_tpl = ObjectTemplate::New(isolate);
  addon_data_tpl->SetInternalFieldCount(1); // 1 field for the MyObject::New()
  Local<Object> addon_data =
      addon_data_tpl->NewInstance(context).ToLocalChecked();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New, addon_data);
  tpl->SetClassName(String::NewFromUtf8(isolate, "IpVPN").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "isVPN", IsVPN);

  Local<Function> constructor = tpl->GetFunction(context).ToLocalChecked();
  addon_data->SetInternalField(0, constructor);
  exports->Set(context, String::NewFromUtf8(isolate, "IpVPN").ToLocalChecked(),
               constructor)
      .FromJust();
}

unsigned int IpUtils::ipv4ToNumber(string ip)
{
  unsigned int rs = 0;
  short part;
  short leftPart = 4;
  istringstream ipstream(ip);
  while (ipstream >> part)
  {
    rs |= (part << (--leftPart * 8));
    if (ipstream.peek() == '.')
    {
      ipstream.ignore();
    }
  }
  return rs;
}

void IpUtils::initArray()
{
  cout << "init array" << endl;
  ifstream file(dataPath);
  if (!file.is_open())
  {
    cerr << "Load data failed\n";
    return;
  }
  string line;
  int i = 0;
  int count = 0;
  while (getline(file, line))
  {
    int start;
    int end;
    istringstream lineStream(line);
    string prop;

    getline(lineStream, prop, ',');
    start = stoul(prop.erase(0, 1));
    getline(lineStream, prop, ',');
    end = stoul(prop.erase(0, 1));

    for (int ip = start; ip <= end; ip++)
    {
      count++;
    }
  }
  cout << "count" << count;
  totalIp = count;
  unsigned int *t = new unsigned int[count];
  ips = t;
  file.close();
}

void IpUtils::preProcessData()
{
  cout << "preprocess data" << endl;
  ifstream file(dataPath);
  if (!file.is_open())
  {
    cerr << "Load data failed\n";
    return;
  }
  string line;
  int i = 0;
  while (getline(file, line))
  {
    int start;
    int end;
    istringstream lineStream(line);
    string prop;

    getline(lineStream, prop, ',');
    start = stoul(prop.erase(0, 1));
    getline(lineStream, prop, ',');
    end = stoul(prop.erase(0, 1));

    for (int ip = start; ip <= end; ip++)
    {
      ips[i++] = ip;
    }
  }
  cout << "start sort" << endl;
  sort_heap(ips, ips + totalIp);
  cout << "end sort" << endl;
  file.close();
}

unsigned int *IpUtils::getIps()
{
  return ips;
}

bool IpUtils::getIp(unsigned int target)
{
  return binary_search(ips, ips + totalIp, target);
}
