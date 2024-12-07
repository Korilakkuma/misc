#!/bin/sh

which wasm-pack

if [ $? = "1" ]
then
  curl https://rustwasm.github.io/wasm-pack/installer/init.sh -sSf | sh
fi

wasm-pack build --target web
