'use strict';

const through = require('through2');
const split = require('split2');
const pipeline = require('pipe-iterators').pipeline;

// see PROTOCOL.md
const channels = Object.freeze([
  4,
  10,
  12,
  2,
  6,
  8,
  14,
  16,
  5,
  11,
  13,
  3,
  7,
  9,
  15,
  1
]);

const songs = Array(256);

songs.fill(-1);
songs[bitsToInt([ 1, 1, 1, 1, 0, 1, 1, 1 ])] = 1;
songs[bitsToInt([ 1, 1, 1, 1, 1, 0, 1, 1 ])] = 2;
songs[bitsToInt([ 1, 1, 1, 1, 1, 1, 0, 1 ])] = 3;

function bitsToInt(bits) {
  if (bits.length === 1) {
    return +bits[0];
  }

  return 2 * bitsToInt(bits.slice(0, -1)) + (+bits[bits.length - 1]);
}

function createInputParser() {
  return through.obj(function (line, _, cb) {
    cb(null, {
      channel: channels[bitsToInt(line.slice(0, 4))],
      song: songs[bitsToInt(line.slice(4))]
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
