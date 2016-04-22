#!/bin/bash

cd $(dirname "$0")

sleep 1
exec ./doorman "$@"
