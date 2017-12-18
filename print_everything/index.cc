#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <node.h>

namespace demo {

using v8::Array;
using v8::Context;
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void PrintArray(Local<Array> array, Isolate* isolate) {
  std::vector<double> vec;

  unsigned int length = array->Length();
  for (unsigned int i = 0; i < length; i++) {
    if (!array->Get(i)->IsNumber()) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument #3 must consist only of number.")));
    }
    vec.push_back((double)Local<Number>::Cast(array->Get(i))->NumberValue());
  }

  for (auto i = vec.begin(); i != vec.end(); ++i) {
    if (i == vec.end() - 1) {
      std::cout << *i;
      continue;
    }
    std::cout << *i << ", ";
  }
  std::cout << std::endl;
}

void PrintObject(Local<Object> object, Isolate* isolate) {
  std::map<std::string, std::string> mp;

  Local<Context> context = isolate->GetCurrentContext();
  Local<Array> props = object->GetOwnPropertyNames(context).ToLocalChecked();
  unsigned int length = props->Length();
  for(unsigned int i = 0; i < length; ++i) {
    Local<Value> local_key = props->Get(i);
    Local<Value> local_val = object->Get(context, local_key).ToLocalChecked();
    if (!local_key->IsString() || !local_val->IsString()) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument #4 must have type of { [key: string]: string; }.")));      
    }
    std::string key = *String::Utf8Value(local_key);
    std::string val = *String::Utf8Value(local_val);
    mp[key] = val;
  }

  for(auto it = mp.begin(); it != mp.end(); ++it) {
    std::cout << it->first << ": " << it->second << std::endl;
  }
}

void PrintEverything(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  HandleScope scope(isolate);

  if (args.Length() < 2) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments.")));
    return;
  }

  if (!args[0]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument #1 must be number.")));
    return;
  }

  if (!args[1]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument #2 must be string.")));
    return;
  }

  if (!args[2]->IsArray()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument #3 must be array.")));
    return;
  }

  if (!args[3]->IsObject()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument #4 must be object.")));
    return;
  }
  
  double number = args[0]->NumberValue();
  String::Utf8Value string(args[1]);
  std::string cpp_string = std::string(*string);
  
  std::cout << number << ", " << cpp_string << std::endl;

  Local<Array> array = Local<Array>::Cast(args[2]);
  PrintArray(array, isolate);
  std::cout << std::endl;

  Local<Object> object = Local<Object>::Cast(args[3]);
  PrintObject(object, isolate);
}


void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "printEverything", PrintEverything);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init)

}
