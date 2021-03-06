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

function bitsToInt(bits) {
  if (bits.length === 1) {
    return +bits[0];
  }

  return 2 * bitsToInt(bits.slice(0, -1)) + (+bits[bits.length - 1]);
}

exports.song = function getSong(bits) {
  return songs[typeof bits === 'number' ? bits : bitsToInt(bits)];
};

exports.channel = function getChannel(bits) {
  return channels[typeof bits === 'number' ? bits : bitsToInt(bits)];
}
