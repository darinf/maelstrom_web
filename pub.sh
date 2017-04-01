#!/bin/sh

outdir="/var/www/html/wasm/maelstrom"

rootfiles="out/Release/maelstrom.* web/* Maelstrom_Fonts Maelstrom_Sprites Maelstrom_Sounds Maelstrom-Scores icon.bmp"
images="Images/*"

make
for f in $rootfiles; do
  echo "cp -f $f $outdir"
  cp -f $f $outdir
done
mkdir -p "$outdir/Images"
chmod a+rx "$outdir/Images"
for f in $images; do
  echo "cp -f $f $outdir/Images"
  cp -f $f "$outdir/Images"
done
chmod -R a+r $outdir/*
