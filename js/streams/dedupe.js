'use strict';

const through = require('through2');

const TIMEOUT = 100 /* ms */;

module.exports = function createDedupeStream() {
  const lastTimes = Array(16);
  lastTimes.fill(Date.now());

  return through.obj(function (hit, _, cb) {
    const lastTime = lastTimes[hit.channel - 1];
    const now = lastTimes[hit.channel - 1] = Date.now();

    if ((now - lastTime) > TIMEOUT) {
      this.push(hit);
    }

    cb(null);
  });
};
