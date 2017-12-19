#include <iostream>
#include <string>
#include <gmpxx.h>
#include <node.h>
#include <uv.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;
using v8::Persistent;
using v8::Function;
using v8::HandleScope;

namespace demo {

class AsyncWork {
  public:
    uv_work_t uv_request;
    Persistent<Function> callback;
    int number;
    std::string result;
};

std::string ComputeFibonacci(int number) {
  int i;
  mpz_class a, b, c;
  a = 1;
  b = 1;
  if (number == 1) return a.get_str();
  if (number == 2) return b.get_str();
  for (i = 0; i < number - 2; ++i) {
    c = a + b;
    a = b;
    b = c;
  }
  return c.get_str();
}

static void DoWork(uv_work_t *req) {
  AsyncWork *work = static_cast<AsyncWork *>(req->data);
  std::string result = ComputeFibonacci(work->number);
  work->result = result;
}

static void AfterWork(uv_work_t *req, int status) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  AsyncWork *work = static_cast<AsyncWork *>(req->data);
  Local<String> result = String::NewFromUtf8(isolate, work->result.c_str());

  const unsigned argc = 1;
  Local<Value> argv[argc] = {result};
  // TODO: PersistentのままではCallできない？
  Local<Function> cb = Local<Function>::New(isolate, work->callback);
  cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
  work->callback.Reset();

  delete work;
}

// uvのqueueに詰める
void ComputeFibonacciAsync(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  int number = (args[0]->NumberValue());
  Local<Function> cb_local = Local<Function>::Cast(args[1]);

  AsyncWork *work = new AsyncWork;
  work->uv_request.data = work;
  work->callback.Reset(isolate, cb_local);
  work->number = number;

  uv_queue_work(uv_default_loop(), &work->uv_request, (uv_work_cb)DoWork, (uv_after_work_cb)AfterWork);
}

void LoadAll(Local<Object> exports, Local<Object> module) {
  NODE_SET_METHOD(module, "exports", ComputeFibonacciAsync);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, LoadAll);
}  // namespace demo
