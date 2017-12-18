#include <iostream>
#include <node.h>
#include "simple_object.h"

namespace demo {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void CreateNewInstance(const FunctionCallbackInfo<Value>& args) {
  SimpleObject::CreateNewInstance(args);
}

void Init(Local<Object> exports, Local<Object> module) {
  SimpleObject::LoadConstructor(exports->GetIsolate());

  NODE_SET_METHOD(module, "exports", CreateNewInstance);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init);

}  // namespace demo
