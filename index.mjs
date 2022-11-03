import bindings from 'bindings'
const display = bindings('shim');

export default EPaperDisplay;
const {
  initDisplay,
  drawImage,
  clear,
  sleep,
  wake,
  initTouch,
  getTouchEvents,
  tick
} = display;

class EPaperDisplay {
  constructor(dispType, {clearColor = 0xff, dispMirror = EPaperDisplay.Mirror.none, dispMode = 'Partial'}) {
    initDisplay(dispType, clearColor, dispMirror, dispMode);
    clear();

    initTouch(dispType);
  }

  drawImage(buffer) {
    drawImage(buffer);
  }

  clear() {
    clear();
  }

  sleep() {
    sleep();
  }

  wake() {
    wake();
  }

  getTouchEvents() {
    getTouchEvents();
  }

  tick() {
    tick();
  }

  static Mirror = {
    none: 0,
    x: 1,
    y: 2
  }
}
