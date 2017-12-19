#include <node.h>
#include <iostream>
#include <node_object_wrap.h>
#include "hello_stack.h"

namespace demo {

using v8::Context;
using v8::Exception;
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

Persistent<Function> HelloStack::constructor;

HelloStack::HelloStack() {
}

HelloStack::~HelloStack() {
}

void HelloStack::LoadConstructor(Isolate* isolate) {
  HandleScope scope(isolate);
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, Constructor);
  tpl->SetClassName(String::NewFromUtf8(isolate, "HelloStack"));
  // ラップ・オブジェクトなので1以上である必要がある。内部の実装まで見ていないが、おそらく
  // 内部にHelloStackへの参照を持っている
  // TODO: HelloStackはGC対象になっている？
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "push", Push);
  NODE_SET_PROTOTYPE_METHOD(tpl, "pop", Pop);

  HelloStack::constructor.Reset(isolate, tpl->GetFunction());
}

void HelloStack::Push(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  if (args.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments.")));
    return;
  }

  if (!args[0]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument #1 must be number.")));
    return;
  }

  double number = args[0]->NumberValue();

  HelloStack* obj = ObjectWrap::Unwrap<HelloStack>(args.Holder());
  obj->stack_.push_back(number);
}

void HelloStack::Pop(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  HelloStack* obj = ObjectWrap::Unwrap<HelloStack>(args.Holder());
  if (obj->stack_.size() == 0) {
    isolate->ThrowException(Exception::RangeError(String::NewFromUtf8(isolate, "Stack is empty.")));
    return;
  }
  double value = obj->stack_.back();
  args.GetReturnValue().Set(Number::New(isolate, value));
  obj->stack_.pop_back();
}

void HelloStack::Constructor(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // new演算子 or Local<Function>#NewInstanceによる呼び出しの場合
  if (args.IsConstructCall()) {
    HelloStack* obj = new HelloStack();
    Local<Object> that = args.This();

    that->Set(String::NewFromUtf8(isolate, "hello"), String::NewFromUtf8(isolate, "world"));

    obj->Wrap(that);
    args.GetReturnValue().Set(that);
    return;
  }
}

void HelloStack::CreateNewInstance(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  const unsigned argc = 0;
  Local<Value> argv[argc] = {};
  Local<Function> constructor = Local<Function>::New(isolate, HelloStack::constructor);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> instance = constructor->NewInstance(context, argc, argv).ToLocalChecked();
  args.GetReturnValue().Set(instance);
}

}  // namespace demo
