#include <node.h>
#include <v8.h>
#include "../lib/OpenQueue.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

class QueueWrapper : public node::ObjectWrap {
 public:
  static void Init(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Queue"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "push", Push);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pop", Pop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);
    NODE_SET_PROTOTYPE_METHOD(tpl, "empty", Empty);
    NODE_SET_PROTOTYPE_METHOD(tpl, "full", Full);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "Queue"),
                 tpl->GetFunction());
  }

 private:
  explicit QueueWrapper(int maxsize = 0): queue(maxsize) {}

  ~QueueWrapper() {}

  static void New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    int maxsize = 0;
    if (args.Length() > 0) {
      maxsize = args[0]->Int32Value();
    }

    QueueWrapper* obj = new QueueWrapper(maxsize);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  }

  static void Push(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    QueueWrapper* obj = ObjectWrap::Unwrap<QueueWrapper>(args.Holder());
    T item = args[0]->ToObject();  // Convert the JavaScript value to the correct type
    int res = obj->queue.push(item);

    args.GetReturnValue().Set(v8::Number::New(isolate, res));
  }
  static void Pop(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    QueueWrapper* obj = ObjectWrap::Unwrap<QueueWrapper>(args.Holder());
    T item;
    int res = obj->queue.pop(item);

    if (res == 0) {
      args.GetReturnValue().Set(v8::Null(isolate));
    } else {
      args.GetReturnValue().Set(v8::Number::New(isolate, res));
    }
  }

  static void Size(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    QueueWrapper* obj = ObjectWrap::Unwrap<QueueWrapper>(args.Holder());
    int size = obj->queue.size();

    args.GetReturnValue().Set(v8::Number::New(isolate, size));
  }

  static void Empty(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    QueueWrapper* obj = ObjectWrap::Unwrap<QueueWrapper>(args.Holder());
    bool empty = obj->queue.empty();

    args.GetReturnValue().Set(v8::Boolean::New(isolate, empty));
  }

  static void Full(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    QueueWrapper* obj = ObjectWrap::Unwrap<QueueWrapper>(args.Holder());
    bool full = obj->queue.full();

    args.GetReturnValue().Set(v8::Boolean::New(isolate, full));
  }

  Queue<T> queue;

  static Persistent<Function> constructor;
};

Persistent<Function> QueueWrapper::constructor;

void Init(Local<Object> exports) {
  QueueWrapper::Init(exports);
}

NODE_MODULE(queue, Init)