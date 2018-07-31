Maelstrom Web
=============

**OVERVIEW**

This is a port of Maelstrom 3.0.6 source to Web Assembly.  The original source
comes from http://www.libsdl.org/projects/Maelstrom/source.html.

Athough Maelstrom uses SDL, and there have been efforts to port SDL to asm.js
and perhaps Web Assembly, this project does not leverage those efforts.
Instead, this project includes a minimal implementation of the parts of SDL
used by Maelstrom.  I did this for educational purposes as I wanted to learn
what it would be like to do so.  As such this code is probably not all that
useful to anyone else.

**BUILDING**

To build, just run the pub.sh script from the root of the source directory.
You'll need to have Emscripten setup and in your PATH.

**RUNNING**

From the out/pub/ directory, run a httpd server like so:
<code>python -m SimpleHTTPServer 8000</code>

Then, point your browser at http://localhost:8000/maelstrom.html.

You can also try loading http://darinf.github.io/maelstrom_web/maelstrom.html.
