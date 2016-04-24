'use strict';

const moment = require('moment');

function always() {
  return true;
}

function negate(fn) {
  return function (now) {
    return !fn(now);
  };
}

function combine(functions) {
  return function (now) {
    return functions.some(function (fn) {
      return fn(now);
    });
  };
}

function isAfter(startHour, startMinutes) {
  startHour = +startHour;

  if (startMinutes || startMinutes === 0 || startMinutes === '0') {
    startMinutes = +startMinutes;

    return function (now) {
      const hour = now.hour();
      console.log(startHour, startMinutes);
      return hour > startHour || (hour === startHour && now.minutes() >= startMinutes);
    };
  }

  return function (now) {
    return now.hour() >= startHour;
  };
}

function matchHours(startHour, startMinutes, endHour, endMinutes) {
  const afterStart = isAfter(startHour, startMinutes);
  const afterEnd = isAfter(endHour, endMinutes);

  return function (now) {
    return afterStart(now) && !afterEnd(now);
  };
}

function matchFormat(config) {
  let match;
  config = config.replace(/\s/g, '');

  // match all day
  if (config.match(/^(always|all day)$/)) {
    return always;
  }

  if (config.match(/^(never|not)$/)) {
    return negate(always);
  }

  // hour regex: /(?:([01]?\d|2[0-3])(?::([0-5]\d))?)/

  // match from a certain moment
  // e.g. "7:15 -"
  if (match = config.match(/^(?:([01]?\d|2[0-3])(?::([0-5]\d))?)-$/)) {
    return isAfter(match[1], match[2]);
  }

  // match until a certain moment
  // e.g. "- 22:45"
  if (match = config.match(/^-(?:([01]?\d|2[0-3])(?::([0-5]\d))?)$/)) {
    return negate(isAfter(match[1], match[2]));
  }

  // match between two moments
  // e.g. "7 - 22:45"
  if (match = config.match(/^(?:([01]?\d|2[0-3])(?::([0-5]\d))?)-(?:([01]?\d|2[0-3])(?::([0-5]\d))?)$/)) {
    let isAfterStart = isAfter(match[1], match[2]);
    let isAfterEnd = isAfter(match[3], match[4]);

    if (isAfterStart(moment(`${match[3]}:${match[4]}`, 'HH:mm'))) {
      // e.g. 7-22
      return function (now) {
        return isAfterStart(now) && !isAfterEnd(now);
      }
    }

    // e.g. 22-7

    return function (now) {
      return isAfterStart(now) || !isAfterEnd(now);
    }
  }

  // match more than one
  // e.g. "7 - 9, 18 - 19"
  if (config.indexOf(',') !== -1) {
    return combine(config.split(',').map(matchFormat));
  }

  throw new Error(`Invalid format: ${config}`);
}

const daysMapping = [
  { key: 'week', days: [ 1, 2, 3, 4, 5 ] },
  { key: 'weekend', days: [ 0, 6 ] },
  { key: 'monday', days: [ 1 ] },
  { key: 'tuesday', days: [ 2 ] },
  { key: 'wednesday', days: [ 3 ] },
  { key: 'thursday', days: [ 4 ] },
  { key: 'friday', days: [ 5 ] },
  { key: 'saturday', days: [ 6 ] },
  { key: 'sunday', days: [ 0 ] }
];

exports =
module.exports = function createDateRange(config) {
  if (!config) {
    return always();
  }

  if (typeof config === 'string') {
    const matcher = matchFormat(config);

    return function (now) {
      return matcher(now || moment());
    }
  }

  const dayMatchers = Array(7);
  dayMatchers.fill(always);

  daysMapping.forEach(function (map) {
    if (!config[map.key]) {
      return;
    }

    const matcher = matchFormat(config[map.key]);

    map.days.forEach(function (day) {
      console.log(day, config[map.key]);
      dayMatchers[day] = matcher;
    });
  });

  return function (now) {
    now = now || moment();
    console.log(now.day());

    return dayMatchers[now.day()](now);
  };
};
