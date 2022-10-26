#include <napi.h>
#include <stdlib.h>

#include <bitset>
#include <iostream>

#include "display/display.hpp"
#include "touch/touch.hpp"

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

  //  JS Number to EPD::DisplayMirror
  EPD::DisplayMirror mirror = EPD::DisplayMirror::NoMirror;
  if(info[2].As<Napi::Number>().Int32Value() & static_cast<uint8_t>(EPD::DisplayMirror::XMirror))
    mirror |= EPD::DisplayMirror::XMirror;
  if(info[2].As<Napi::Number>().Int32Value() & static_cast<uint8_t>(EPD::DisplayMirror::YMirror))
    mirror |= EPD::DisplayMirror::YMirror;

  // JS String to EPD::DisplayMode
  EPD::DisplayMode mode = EPD::DisplayMode::Full;
  if(std::string(info[3].As<Napi::String>()) == "Partial")
    mode = EPD::DisplayMode::Partial;

  EPD::DisplayConfig cfg(
    std::string(info[0].As<Napi::String>()).c_str(),
    info[1].As<Napi::Number>().Int32Value(),
    mirror,
    mode
  );

  EPD::Display::getInstance().init(cfg);

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

  //Get the ArrayBuffer and then check it for size
  Napi::ArrayBuffer buf = info[0].As<Napi::ArrayBuffer>();
  if (!EPD::Display::getInstance().writeBuffer(buf.Data(), buf.ByteLength())) {
    Napi::RangeError::New(env, "Arraybuffer not large enough to fill the image buffer")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  EPD::Display::getInstance().draw();

  return Napi::Boolean::New(env, true);
}

Napi::Value Clear(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  EPD::Display::getInstance().clear();

  return Napi::Boolean::New(env, true);
}

Napi::Value Sleep(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  //Clear the screen
  EPD::Display::getInstance().sleep();

  return Napi::Boolean::New(env, true);
}

Napi::Value Wake(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  //Initialize device to wake it from sleep
  EPD::Display::getInstance().wake();

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

  Touch::Touch::getInstance().init(std::string(info[0].As<Napi::String>()).c_str());

  return Napi::Boolean::New(env, true);
}

Napi::Value GetTouchEvents(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  std::vector<Touch::TouchEvent>* events = Touch::Touch::getInstance().getTouchEvents();
  int index = 0;

  Napi::Object jsEvent;
  Napi::Object retVal = Napi::Array::New(env);

  for(auto ev = events->begin(); ev != events->end(); ev++) {
    jsEvent = Napi::Object::New(env);

    switch (ev->type)
    {
    case Touch::EventType::TouchStart:
      jsEvent.Set("type", "TouchStart");
      break;
    case Touch::EventType::TouchMove:
      jsEvent.Set("type", "TouchMove");
      break;
    case Touch::EventType::TouchEnd:
      jsEvent.Set("type", "TouchEnd");
      break;

    default:
      break;
    }

    jsEvent.Set("id", ev->id);

    jsEvent.Set("x", ev->x);
    jsEvent.Set("y", ev->y);
    jsEvent.Set("size", ev->size);

    jsEvent.Set("deltaX", ev->deltaX);
    jsEvent.Set("deltaY", ev->deltaY);

    retVal.Set(index, jsEvent);
    index++;
  }

  Touch::Touch::getInstance().clearTouchEvents();

  return retVal;
}

Napi::Value Tick(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  //update the touch data
  Touch::Touch::getInstance().tick();

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
