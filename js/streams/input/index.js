'use strict';

const listen = require('./listen');
const parser = require('./parser');

const tail = require('pipe-iterators').tail;

module.exports = function (config) {
  return tail(
    listen(config),
    parser()
  );
};
