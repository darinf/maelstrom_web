Maelstrom NaCl
==============

**OVERVIEW**

This is a port of Maelstrom 3.0.6 source to Pepper / NaCl.  The original source
comes from http://www.libsdl.org/projects/Maelstrom/source.html.

Athough Maelstrom uses SDL, and SDL has separately been ported to Pepper, I
chose not to use that.  Instead, I just developed a minimal implementation of
the parts of SDL used by Maelstrom.  I did this for educational purposes as I
wanted to learn what it would be like to do so.  As such this code is probably
not all that useful to anyone else.

**BUILDING**

To build, just run make from the root of the source directory.  You'll need
to have the NaCl SDK w/ Pepper 31 available.  See the Makefile for
where the path to Pepper 31 needs to be specified.

**RUNNING**

From the Pepper 31 directory, run the httpd server from the NaCl SDK like so:
<code>./examples/httpd -C '<path-to>/maelstrom_nacl' --no_dir_check</code>

Then, point your browser at http://localhost:5103/maelstrom.html.

You can also try loading http://darinf.github.io/maelstrom_nacl/maelstrom.html.
