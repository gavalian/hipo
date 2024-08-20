#!/usr/bin/env bash

[ $# -ne 1 ] && exit 1
runner=$1

case "$runner" in
  'ubuntu-latest')
    python -m pip install meson ninja
    ;;
  'macos-latest')
    brew install meson ninja tree
    # install fortran compiler:
    brew install gcc
    brew unlink gcc
    brew link gcc
    ;;
  *)
    exit 1
    ;;
esac
