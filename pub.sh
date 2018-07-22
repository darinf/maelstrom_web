#!/bin/sh

outdir="out/pub"

rootfiles="out/Release/maelstrom.* web/* Maelstrom_Fonts Maelstrom_Sprites Maelstrom_Sounds Maelstrom-Scores icon.bmp"
images="Images/*"

make
mkdir -p $outdir

echo -n "copying files to $outdir "

for f in $rootfiles; do
  echo -n "."
  cp -f $f $outdir
done
mkdir -p "$outdir/Images"
chmod a+rx "$outdir/Images"
for f in $images; do
  echo -n "."
  cp -f $f "$outdir/Images"
done
chmod -R a+r $outdir/*
echo
