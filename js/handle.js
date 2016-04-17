'use strict';

const pipeline = require('pipe-iterators').pipeline;

const inputStream = require('./streams/input');
const dedupeStream = require('./streams/dedupe');

const logStream = require('through2').obj(function (hit, _, cb) {
  console.log(JSON.stringify(hit));
  cb(null, hit);
});

process.stdin.pipe(
  pipeline(
    inputStream(),
    dedupeStream(),
    logStream
  ).on('error', console.error)
);
