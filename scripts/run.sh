#!/bin/bash

set -e

cd $(dirname $(dirname "$0"))

if [ ! -x bin/listen ] || [ ! -x bin/doorman ]; then
  echo "Are you sure you installed this npm module?" >&2
  exit 1
fi

NODE=nodejs
if which node >/dev/null 2>&1; then
  NODE=node
fi

exec sudo $NODE start -- config.json
