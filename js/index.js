'use strict';

const pipeline = require('pipe-iterators').pipeline;

const inputStream = require('./streams/input');
const dedupeStream = require('./streams/dedupe');
const logStream = require('./streams/log');
const outputStream = require('./streams/output');

module.exports = function doorman(config) {
  return inputStream(config).pipe(
    pipeline(
      dedupeStream(),
      logStream,
      outputStream(config)
    ).on('error', console.error)
  );
};
