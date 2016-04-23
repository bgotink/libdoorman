'use strict';

const through = require('through2');
const request = require('request');

exports =
module.exports = function sendToIfttt(channelCfg) {
  const url = `https://maker.ifttt.com/trigger/${channelCfg.ifttt.event}/with/key/${channelCfg.ifttt.key}`;
  const hasSong = channelCfg.song && channelCfg.song > 0;
  var channel = +channelCfg.channel;

  return through.obj(function (hit, _, cb) {
    if (hit.channel !== channel || (hasSong && channelCfg.song !== hit.song)) {
      // drop
      return cb(null);
    }

    console.log(`Sending ${channelCfg.ifttt.event} event to IFTTT for channel ${channel}${hasSong ? ` and song ${channelCfg.song}` : ''}`);
    request(url, function (error, response) {
      if (error) {
        console.log('Error while contacting IFTTT');
        return cb(error);
      }

      if (response.statusCode !== 200) {
        console.log(`Got HTTP response code ${response.statusCode} from IFTTT`);
        return cb(response.statusCode);
      }

      console.log(`Event ${channelCfg.ifttt.event} sent to IFTTT`);
      cb(null);
    });
  });
};

exports.check = function hasIftttConfig(channelCfg) {
  if (!channelCfg.ifttt) {
    return false;
  }

  if ((typeof channelCfg.ifttt) !== 'object') {
    console.warn(`Expected IFTTT config to be an object, got ${typeof channelCfg.ifttt}`);
    return false;
  }

  const iftttCfg = channelCfg.ifttt;

  if ((typeof iftttCfg.event) !== 'string') {
    console.warn(`Expected IFTTT event string but got ${typeof iftttCfg.event}`);
    return false;
  }

  if ((typeof iftttCfg.key) !== 'string') {
    console.warn(`Expected IFTTT key string but got ${typeof iftttCfg.key}`);
    return false;
  }

  return true;
};
