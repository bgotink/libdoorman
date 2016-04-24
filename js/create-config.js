'use strict';

const fs = require('fs');
const inquirer = require('inquirer');

function promisify(obj, fn) {
  const args = Array.prototype.slice.call(arguments, 2);

  if (typeof fn !== 'function') {
    fn = obj[fn];
  }

  return new Promise(function(resolve, reject) {
    args.push(function (err, result) {
      if (err) {
        reject(err);
      }

      resolve(result);
    });

    fn.apply(obj, args);
  });
}

function when(variable) {
  if (arguments.length === 1) {
    return function (answers) {
      return answers[variable];
    };
  }

  const variables = Array.prototype.slice.call(arguments);
  return function (answers) {
    return variables.every(function (variable) {
      return answers[variable];
    });
  };
}

const validators = {
  pin: function (pin) {
    if (isNaN(pin)) {
      return "Pin must be a number";
    }

    if (pin < 0) {
      return "Pin must be positive";
    }

    if (pin > 63) {
      return "Pin must be lower than 64";
    }

    return true;
  },
  channel: function (channel) {
    if (isNaN(channel)) {
      return "Channel must be a number";
    }

    if (channel < 1 || channel > 16) {
      return "Channel must be 1-16";
    }

    return true;
  },
  song: function (song) {
    if (isNaN(song)) {
      return "Sound must be a number";
    }

    if (song < 1 || song > 3) {
      return "Sound must be 1, 2 or 3";
    }

    return true;
  }
};

const filters = {
  number: function (input) {
    return +input;
  }
};

const questions = {
  pins: [{
    name: 'read',
    type: 'input',
    message: 'What is the GPIO pin to read from?',
    validate: validators.pin,
    filter: filters.number
  }, {
    name: 'write',
    type: 'input',
    message: 'What is the GPIO pin to write to?',
    validate: validators.pin,
    filter: filters.number
  }],

  moreChannels: [{
    name: 'more',
    type: 'confirm',
    default: false,
    message: 'Do you want to add more channels?'
  }],

  channel: [{
    name: 'channel',
    type: 'input',
    message: 'What channel do you want to listen to?',
    validate: validators.channel,
    filter: filters.number
  }, {
    name: 'hasSong',
    type: 'confirm',
    default: true,
    message: 'Do you want to listen for a specific sound?'
  }, {
    name: 'song',
    type: 'input',
    message: 'What sound do you want to listen to?',
    validate: validators.song,
    filter: filters.number,
    when: when('hasSong')
  },

  {
    name: 'hasIfttt',
    type: 'confirm',
    default: false,
    message: 'Do you want to send a signal to IFTTT?'
  }, {
    name: 'ifttt_key',
    type: 'input',
    message: 'Enter your IFTTT Maker key:',
    when: when('hasIfttt')
  }, {
    name: 'ifttt_event',
    type: 'input',
    message: 'Enter your IFTTT event name:',
    when: when('hasIfttt')
  },

  {
    name: 'hasRingBell',
    type: 'confirm',
    default: false,
    message: 'Do you want to ring a bell?'
  }, {
    name: 'ringBell_channel',
    type: 'input',
    message: 'What channel do you want to ring your bell on?',
    validate: validators.channel,
    filter: filters.number,
    when: when('hasRingBell')
  }, {
    name: 'ringBell_hasSong',
    type: 'confirm',
    default: true,
    message: 'Do you want to specify a sound? (if not, the sound received will be replayed)'
  }, {
    name: 'ringBell_song',
    type: 'input',
    message: 'What sound do you want to play?',
    validate: validators.song,
    filter: filters.number,
    when: when('hasRingBell', 'ringBell_hasSong')
  }]
};

function askChannelQuestions(obj) {
  return inquirer.prompt(questions.channel)
    .then(function (answers) {
      const channel = {};

      channel.channel = answers.channel;

      if (answers.hasSong) {
        channel.song = answers.song;
      }

      if (answers.hasIfttt) {
        channel.ifttt = {
          key: answers.ifttt_key,
          event: answers.ifttt_event
        };
      }

      if (answers.hasRingBell) {
        channel.ringBell = {
          channel: answers.ringBell_channel
        };

        if (answers.ringBell_hasSong) {
          channel.ringBell.song = answers.ringBell_song;
        }
      }

      obj.channels.push(channel);
      return obj;
    })
    .then(function (obj) {
      return inquirer.prompt(questions.moreChannels)
        .then(function (answers) {
          if (!answers.more) {
            return obj;
          }

          return askChannelQuestions(obj);
        });
    });
}

module.exports = function createConfig(path) {
  return promisify(fs, 'readFile', path)
    .then(function () {
      return Promise.reject(`File ${path} exists, please delete the file before passing it to --init`);
    }, function (err) {
      if (err.code === 'ENOENT') {
        return;
      }

      return Promise.reject(err);
    })
    .then(function () {
      return inquirer.prompt(questions.pins);
    })
    .then(function (pins) {
      return {
        pins: pins,
        channels: []
      };
    })
    .then(askChannelQuestions)
    .then(function (obj) {
      return promisify(fs, 'writeFile', path, JSON.stringify(obj, null, 2));
    });
}
