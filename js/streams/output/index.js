'use strict';

const through = require('through2');
const fork = require('pipe-iterators').fork;

const outputHandlers = Object.freeze([
  require('./ifttt'),
  require('./ring-bell')
]);

function drop() {
  return through.obj(function (_, __, cb) {
    cb(null);
  });
}

module.exports = function createHandleStream(config) {
  if (!config.channels) {
    return drop();
  }

  var channels = config.channels;

  if (!Array.isArray(channels)) {
    channels = [ channels ];
  }

  if (channels.length === 0) {
    return drop();
  }

  return fork(channels.map(function (channel) {
    const streams = [];

    outputHandlers.forEach(function (handler) {
      if (handler.check(channel)) {
        streams.push(handler(channel, config));
      }
    });

    if (streams.length) {
      return fork(streams);
    } else {
      return drop();
    }
  }));
}
