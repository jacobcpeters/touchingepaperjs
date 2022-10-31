#include <napi.h>

Napi::Value InitDisplay(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 4) {
    Napi::TypeError::New(env, "Not enough arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::New(env, true);
}

Napi::Value DrawImage(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "drawImageFull needs an Arraybuffer")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsArrayBuffer()) {
    Napi::TypeError::New(env, "drawImageFull needs an Arraybuffer")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::New(env, true);
}

Napi::Value Clear(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();


  return Napi::Boolean::New(env, true);
}

Napi::Value Sleep(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();


  return Napi::Boolean::New(env, true);
}

Napi::Value Wake(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();


  return Napi::Boolean::New(env, true);
}

Napi::Value InitTouch(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Not enough arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }


  return Napi::Boolean::New(env, true);
}

Napi::Value GetTouchEvents(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  Napi::Object retVal = Napi::Array::New(env);

  return retVal;
}

Napi::Value Tick(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  return Napi::Boolean::New(env, true);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  //initialize the epaper device

  exports.Set(Napi::String::New(env, "initDisplay"), Napi::Function::New(env, InitDisplay));
  exports.Set(Napi::String::New(env, "drawImage"), Napi::Function::New(env, DrawImage));
  exports.Set(Napi::String::New(env, "clear"), Napi::Function::New(env, Clear));
  exports.Set(Napi::String::New(env, "sleep"), Napi::Function::New(env, Sleep));
  exports.Set(Napi::String::New(env, "wake"), Napi::Function::New(env, Wake));

  exports.Set(Napi::String::New(env, "initTouch"), Napi::Function::New(env, InitTouch));
  exports.Set(Napi::String::New(env, "getTouchEvents"), Napi::Function::New(env, GetTouchEvents));
  exports.Set(Napi::String::New(env, "tick"), Napi::Function::New(env, Tick));

  return exports;
}

NODE_API_MODULE(shim, Init)
