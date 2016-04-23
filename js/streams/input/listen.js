'use strict';

const spawn = require('child_process').spawn;
const path = require('path');

const LISTEN_EXEC = path.resolve(__dirname, '../../../bin/listen');

module.exports = function (config) {
  const child = spawn(LISTEN_EXEC, [ config.pins.read ], {
    stdio: [ 'ignore', 'pipe', 'inherit' ]
  });

  return child.stdout;
};
