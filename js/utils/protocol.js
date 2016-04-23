'use strict';
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

Object.freeze(songs);

exports.songs = songs;
exports.channels = channels;
