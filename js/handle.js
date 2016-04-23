'use strict';

const through = require('through2');
const pipeline = require('pipe-iterators').pipeline;

const inputParserStream = require('./streams/input-parser');
const dedupeStream = require('./streams/dedupe');
const handleInputStream = require('./streams/handle-input');

const logStream = require('through2').obj(function (hit, _, cb) {
  console.log(JSON.stringify(hit));
  cb(null, hit);
});

let config = null;

try {
  config = require('./config.json');
} catch (e) {
  console.error('Create config.json (use config.json.base as template) to configure how to handle the input');
}

process.stdin.pipe(
  pipeline(
    inputParserStream(),
    dedupeStream(),
    logStream,
    config ? handleInputStream(config) : through.obj()
  ).on('error', console.error)
);
