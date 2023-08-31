#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "node.h"
#include "ipvpn.h"

using namespace std;
using v8::Boolean;
using v8::Context;
using v8::Exception;
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
using v8::Boolean;

void IpVPN::Init(Local<Object> exports)
{
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

void IpVPN::New(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  if (args.IsConstructCall())
  {
    if (!args[0]->IsString())
    {
      isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate,
                              "Data file path must be a string")
              .ToLocalChecked()));
      return;
    }
    String::Utf8Value str(isolate, args[0]);
    std::string cppStr(*str);
    IpVPN *ipVPN = new IpVPN(cppStr);
    ipVPN->initArray();
    ipVPN->preProcessData();
    ipVPN->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  }
  else
  {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = {args[0]};
    Local<Function> cons = args.Data().As<Object>()->GetInternalField(0).As<Function>();
    Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void IpVPN::IsVPN(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  IpVPN *ipVPN = ObjectWrap::Unwrap<IpVPN>(args.Holder());
  String::Utf8Value v8IpString(isolate, args[0]);
  string ipString(*v8IpString);
  bool isVPN = ipVPN->isVPN(ipString);
  args.GetReturnValue().Set(Boolean::New(isolate, isVPN));
}
