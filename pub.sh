#!/bin/sh

outdir="/var/www/html/wasm/maelstrom"

inputs="out/Release/maelstrom.* web/*"

make
for f in $inputs; do
  echo "cp -f $f $outdir"
  cp -f $f $outdir
done
chmod a+r $outdir/*
