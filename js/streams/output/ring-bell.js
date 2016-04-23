'use strict';

const through = require('through2');

const createDoorman = require('../../utils/doorman');

exports =
module.exports = function ringBell(channelCfg, config) {
  if ((typeof channelCfg.ringBell) !== 'object') {
    channelCfg.ringBell = { channel: channelCfg.ringBell };
  }

  const hasSong = channelCfg.song && channelCfg.song > 0;
  const channel = +channelCfg.channel;

  const outputChannel = channelCfg.ringBell.channel;
  const hasOutputSong = channelCfg.ringBell.song && channelCfg.ringBell.song > 0;

  const doorman = createDoorman(config.pins.write);

  return through.obj(function (hit, _, cb) {
    if (hit.channel !== channel || (hasSong && channelCfg.song !== hit.song)) {
      // drop
      return cb(null);
    }

    const songToTrigger = hasOutputSong ? channelCfg.ringBell.song : hit.song;

    console.log(`Triggering song ${songToTrigger} on channel ${outputChannel} after receiving song ${hit.song} on channel ${channel}`);

    doorman.play(outputChannel, songToTrigger)
      .then(function () { cb(null); })
      .catch(function (err) { cb(err); });
  });
};

exports.check = function hasRingBellCfg(channelCfg) {
  if (!channelCfg.ringBell) {
    return false;
  }

  if (((typeof channelCfg.ringBell) !== 'object') && isNaN(channelCfg.ringBell)) {
    console.warn('ringBell configuration should be object or number (channel id)');
    return false;
  }

  if ((typeof channelCfg.ringBell) !== 'object') {
    const channel = +channelCfg.ringBell;

    if (channel < 1 || channel > 16) {
      console.warn(`Invalid channel: ${channel}, expected value between 1 and 16`);
      return false;
    }

    return true;
  }

  if (isNaN(channelCfg.ringBell.channel) || channelCfg.ringBell.channel < 1
        || channelCfg.ringBell.channel > 16) {
    console.warn(`Invalid channel: ${channel}, expected value between 1 and 16`);
    return false;
  }

  if (channelCfg.ringBell.song && (isNaN(channelCfg.ringBell.song) || channelCfg.ringBell.song > 3)) {
    console.warn(`Invalid song: ${song}, expected undefined, below 1 or 1, 2 or 3`);
    return false;
  }

  return true;
};
