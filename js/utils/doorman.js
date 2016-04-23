'use strict';

const spawn = require('child_process').spawn;
const path = require('path');

var actionsQueue = Promise.resolve();

function takeAction(fn) {
  const result = actionsQueue.then(fn);
  actionsQueue = result.catch(function () {});

  return result;
}

const DOORMAN_EXEC = path.resolve(__dirname, '../../bin/wrap_doorman.sh');

function doorman(args) {
  if (arguments.length > 1 && !Array.isArray(args)) {
    args = Array.prototype.slice.call(arguments);
  }

  return new Promise(function (resolve, reject) {
    const child = spawn(DOORMAN_EXEC, args || [], {
      detached: true,
      stdio: [ 'ignore', 1, 2 ]
    });

    child.on('close', function (code) {
      if (code !== 0) {
        return reject();
      }

      resolve();
    });
  });
}

module.exports = function createDoorman(pin) {
  return  {
    play: function play(channel, song) {
      return takeAction(function () {
        console.log(`Playing song ${song} on channel ${channel}`);
        return doorman(pin, 'write', channel, song).catch(function () {
          return Promise.reject(`Error when playing song ${song} on channel ${channel}`);
        });;
      });
    }
  };
};
