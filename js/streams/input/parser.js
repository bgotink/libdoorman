'use strict';

const through = require('through2');
const split = require('split2');
const pipeline = require('pipe-iterators').pipeline;

const protocol = require('../../utils/protocol');

function createInputParser() {
  return through.obj(function (line, _, cb) {
    if (line.length !== 12 || !line.match(/^[10]*$/)) {
      return cb(null);
    }

    cb(null, {
      channel: protocol.channel(line.slice(0, 4)),
      song: protocol.song(line.slice(4))
    });
  });
}

function createInvalidSongFilter() {
  return through.obj(function (hit, _, cb) {
    if (hit.song > 0) {
      this.push(hit);
    }

    cb(null);
  });
}

module.exports = function () {
  return pipeline(
    split(),
    createInputParser(),
    createInvalidSongFilter()
  );
};
