#ifndef SIMPLE_OBJECT_H
#define SIMPLE_OBJECT_H

#include <node.h>
#include <node_object_wrap.h>

namespace demo {

class SimpleObject : public node::ObjectWrap {
 public:
  static void LoadConstructor(v8::Isolate* isolate);
  static void CreateNewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);

 private:
  explicit SimpleObject();
  ~SimpleObject();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static v8::Persistent<v8::Function> constructor;
};

}

#endif
