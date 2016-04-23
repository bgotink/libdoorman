'use strict';

const through = require('through2');
const pipeline = require('pipe-iterators').pipeline;

const inputStream = require('./streams/input');
const dedupeStream = require('./streams/dedupe');
const outputStream = require('./streams/output');

const logStream = through.obj(function (hit, _, cb) {
  console.log(JSON.stringify(hit));
  cb(null, hit);
});

let config = null;

try {
  config = require('../config.json');
} catch (e) {
  console.error('Create config.json (use config.json.base as template) to configure how to handle the input');
}

inputStream(config).pipe(
  pipeline(
    dedupeStream(),
    logStream,
    outputStream(config)
  ).on('error', console.error)
);
