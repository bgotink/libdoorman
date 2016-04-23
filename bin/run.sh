#!/bin/bash

set -e

cd $(dirname $(dirname "$0"))

if [ ! -x bin/listen ] || [ ! -x bin/doorman ]; then
  echo "making bin/listen and bin/doorman" >&2
  make bin/listen bin/doorman
fi

NODE=nodejs
if which node >/dev/null 2>&1; then
  NODE=node
fi

#cd js; npm install; cd -

sudo bin/listen 27 | sudo $NODE js/handle
