# TouchingEPaper.js

A minimal Node.js library for direct access to an ePaper display along with a simple event system for touch screens.

Features:

- Zero magic (This is a low-level library to use as a dependency for higer level libraries)
- Touch events
- 2 Dependencies

## Usage

This library currently only works with the Waveshare 2.13in Touch ePaper screen.

```js
import {initDisplay, drawImage, initTouch, tick, getTouchEvents} from 'touchingepaperjs'
//initalize the display
initDisplay('rpi_2in13', 0xff, 0, 'Full');
//draw arrayBuffer to the display
drawImage(arrayBuffer);

//initialize touch device
initTouch('rpi_2in13');
//tick should be called periodically ~16-33ms to poll for touch interactions
setTimout(tick, 16);
//retrieve accumulated touch events
let events = getTouchEvents();

/*
example event
{
  type: 'TouchStart',
  id: 0,
  x: 121,
  y: 22,
  size: 16,
  deltaX: 0,
  deltaY: 0
}
*/
```

## Contributions

Pull requests are welcome, and I will merge them when I have time. If the changes are major or breaking to the API (as simple as it is), please sumbit an issue to discuss the changes.
