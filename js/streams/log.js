'use strict';

const through = require('through2');

module.exports = function () {
  return through(function (obj, _, cb) {
    console.log(JSON.stringify(obj));
    cb(null, obj);
  });
};
