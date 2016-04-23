'use strict';

const through = require('through2');
const fork = require('pipe-iterators').fork;
const request = require('request');

const createDoorman = require('../utils/doorman');

function drop() {
  return through.obj(function (_, __, cb) {
    cb(null);
  });
}

function sendToIfttt(channel, song, ifttt) {
  const url = `https://maker.ifttt.com/trigger/${ifttt.event}/with/key/${ifttt.key}`;
  const hasSong = song && song > 0;
  channel = +channel;

  return through.obj(function (hit, _, cb) {
    if (hit.channel !== channel || (hasSong && song !== hit.song)) {
      // drop
      return cb(null);
    }

    console.log(`Sending ${ifttt.event} event to IFTTT for channel ${channel}${hasSong ? ` and song ${song}` : ''}`);
    request(url, function (error, response) {
      if (error) {
        console.log('Error while contacting IFTTT');
        return cb(error);
      }

      if (response.statusCode !== 200) {
        console.log(`Got HTTP response code ${response.statusCode} from IFTTT`);
        return cb(response.statusCode);
      }

      console.log(`Event ${ifttt.event} sent to IFTTT`);
      cb(null);
    });
  });
}

function ringBell(pin, channel, song, outputConfig) {
  const hasSong = song && song > 0;
  channel = +channel;

  const outputChannel = outputConfig.channel;
  const hasOutputSong = outputConfig.song && outputConfig.song > 0;

  const doorman = createDoorman(pin);

  return through.obj(function (hit, _, cb) {
    if (hit.channel !== channel || (hasSong && song !== hit.song)) {
      // drop
      return cb(null);
    }

    const songToTrigger = hasOutputSong ? outputConfig.song : hit.song;

    console.log(`Triggering song ${songToTrigger} on channel ${outputChannel} after receiving song ${hit.song} on channel ${channel}`);

    doorman.play(outputChannel, songToTrigger)
    .then(function () { cb(null); })
    .catch(function (err) { cb(err); });
  });
}

module.exports = function createHandleStream(config) {
  if (!config.channels) {
    return drop();
  }

  var channels = config.channels;

  if (!Array.isArray(channels)) {
    channels = [ channels ];
  }

  if (channels.length === 0) {
    return drop();
  }

  return fork(channels.map(function (channel) {
    const streams = [];

    if (channel.ifttt) {
      streams.push(sendToIfttt(channel.channel, channel.song, channel.ifttt));
    }

    if (channel.ringBell) {
      streams.push(ringBell(config.pins.write, channel.channel, channel.song, channel.ringBell));
    }

    if (streams.length) {
      return fork(streams);
    } else {
      return drop();
    }
  }));
}
