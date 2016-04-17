#!/bin/bash

set -e

cd $(dirname $(dirname "$0"))

if [ ! -x bin/listen ]; then
  echo "making bin/listen" >&2
  make bin/listen
fi

NODE=nodejs
if which node >/dev/null 2>&1; then
  NODE=node
fi

#cd js; npm install; cd -

sudo bin/listen | $NODE js/handle
