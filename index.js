import bindings from 'bindings'
const display = bindings('shim');
import { Screen, Layer, DefaultTypes } from 'bmpui';
import menuScene from './scene.mjs';

function test() {
  const screen = new Screen(250, 122, true);
  screen.scene.addChild(Layer.initScene(menuScene, DefaultTypes));
  screen.render();

  display.clear();
  display.initDisplay('rpi_2in13', 0xff, 0, 'Full');
  display.drawImage(screen.getBuffer());
  display.sleep();
}

test();
