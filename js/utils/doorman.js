'use strict';

const exec = require('child_process').execFile;
const path = require('path');

var actionsQueue = Promise.resolve();

function takeAction(fn) {
  const result = actionsQueue.then(fn);
  actionsQueue = result.catch(function () {});

  return result;
}

const DOORMAN_EXEC = path.resolve(__dirname, '../../bin/doorman');

function doorman(args, options) {
  return new Promise(function (resolve, reject) {
    exec(DOORMAN_EXEC, args || [], options || {}, function (error, stdout, stderr) {
      if (error) {
        console.error(String(stderr));
        console.error(error);
        return reject(error);
      }

      console.log(String(stdout));
      console.error(String(stderr));
      resolve();
    });
  });
}

exports.play = function playSong(channel, song) {
  return takeAction(function () {
    console.log(`Playing song ${song} on channel ${channel}`);
    return doorman([ 'write', channel, song ]);
  });
}
