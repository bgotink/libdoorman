#!/bin/bash

cd $(dirname $(dirname "$0"))

sleep 1
exec bin/doorman "$@"
