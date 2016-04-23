'use strict';

const path = require('path');
const fs = require('fs');

const doorman = require('.');

const argv = require('yargs')
  .usage('Usage: $0 <path/to/config.json>')

  .demand(1)

  .help('h')
  .alias('h', 'help')

  .argv;

const configPath = path.resolve(process.cwd(), argv._[0]);

fs.readFile(configPath, function (err, contents) {
  if (err) {
    console.error(err);
    process.exit(1);
    return;
  }

  doorman(JSON.parse(config));
});
