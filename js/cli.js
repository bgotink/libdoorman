#!/usr/bin/env node
'use strict';

const path = require('path');
const fs = require('fs');

const doorman = require('.');
const createConfig = require('./create-config');

const argv = require('yargs')
  .usage('Usage: $0 [options] <path/to/config.json>')

  .demand(1)

  .help('h')
  .alias('h', 'help')

  .boolean('init')
  .describe('init', 'Create the configuration')

  .argv;

const configPath = path.resolve(process.cwd(), argv._[0]);

if (argv.init) {
  return createConfig(configPath).catch(function (e) {
    console.error(e && e.stack ? e.stack : e);
    process.exit(1);
  });
}

fs.readFile(configPath, function (err, contents) {
  if (err) {
    console.error(err);
    process.exit(1);
    return;
  }

  doorman(JSON.parse(String(contents)));
});
