#include <node.h>
#include <iostream>
#include <node_object_wrap.h>
#include "simple_object.h"

namespace demo {

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

Persistent<Function> SimpleObject::constructor_;

SimpleObject::SimpleObject() {
}

SimpleObject::~SimpleObject() {
}

void SimpleObject::LoadConstructor(Isolate* isolate) {
  HandleScope scope(isolate);
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "SimpleObject"));
  // 関数の場合は1以上である必要があるらしい
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor_.Reset(isolate, tpl->GetFunction());
}

// コンストラクタの実体（普通の関数）
void SimpleObject::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // new演算子による呼び出しの場合
  if (args.IsConstructCall()) {
    SimpleObject* obj = new SimpleObject();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
    return;
  }
}

void SimpleObject::CreateNewInstance(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  const unsigned argc = 0;
  Local<Value> argv[argc] = {};
  Local<Function> constructor = Local<Function>::New(isolate, constructor_);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> instance = constructor->NewInstance(context, argc, argv).ToLocalChecked();
  args.GetReturnValue().Set(instance);
}

}
