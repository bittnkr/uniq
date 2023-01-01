#include <node.h>
#include <v8.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Hello(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Hello, World!"));
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Hello);
}

NODE_MODULE(hello, Init)