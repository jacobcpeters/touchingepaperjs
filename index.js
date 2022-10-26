import bindings from 'bindings'
const display = bindings('shim');

export default display;
export const {
  initDisplay,
  drawImage,
  clear,
  sleep,
  wake,
  initTouch,
  getTouchEvents,
  tick
} = display;
