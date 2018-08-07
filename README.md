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

To build, just run the <code>pub.sh</code> script from the root of the source
directory.  You'll need to have Emscripten setup and in your PATH.

**RUNNING**

From the <code>out/pub/</code> directory, run a httpd server like so:
<code>python -m SimpleHTTPServer 8000</code>

Then, point your browser at http://localhost:8000/maelstrom.html.

You can also try loading http://darinf.github.io/maelstrom_web/wasm/maelstrom.html.

(An older PNaCl version is also available here: http://darinf.github.io/maelstrom_web/pnacl/maelstrom.html.)

**PORTING NOTES**

The main challenge with porting the Maelstrom codebase to WebAssembly was
dealing with all of the sleep calls scattered throughout the codebase as well
as the nested run loops. The main event loop services the main menu, but when
you start a game, it enters another event loop. These event loops block until
completion. The code expects to synchronously poll and wait for input events.

To make this kind of code work without extensive re-writing meant moving it to
run on a Web Worker so that it would be reasonable for the code to block the
thread of execution. However, that was not enough. It was also necessary to
somehow provide input events to the blocked worker thread. To do that, it was
necessary to use the new SharedArrayBuffer API and corresponding Atomics API,
which provides Futex functionality.

The idea is to use SharedArrayBuffer to implement a FIFO queue for input events
and support blocking for new input by using <code>Atomics.wait()</code>. This
complexity is contained in the file <a href="web/pipe.js">pipe.js</a>, which
provides an implementation of a uni-directional pipe for streaming bytes and a
message pipe built on top of that to provide uni-directional streaming of
framed messages (byte arrays).

With the pipe primitive, it is possible to have the main page write input
events into the pipe, and then have the worker thread read from the other end
of the pipe, blocking as necessary until input is available.

This same pipe primitive is used for output as well. While it is possible for
<code>postMessage</code> to deliver events from the worker thread to the main
page, <code>postMessage</code> does not provide any means of applying back
pressure to the worker thread. The worker thread generates a lot of draw
commands (i.e., bitmaps to copy to the screen), and without some back pressure
the worker thread would use up a lot of memory very quickly and saturate the
main page with work.

By using a pipe for transmitting the draw commands, we are able to place some
back pressure on the worker thread in a reasonable fashion and also ensure that
memory gets reused (one SharedArrayBuffer allocation) for transporting the
bitmaps to be drawn.

Finally, for the moments when the game needs to sleep, that too can be solved
using SharedArrayBuffer. The sleep system call is implemented using a small
SharedArrayBuffer and a call to <code>Atomics.wait()</code> with a specified
timeout.

The last bit of work required to port this game involved hooking up audio.  The
game uses SDL, which runs a high priority background thread to transfer audio
samples to the output device. The game is designed with the idea that starting
audio playback simply means making this background thread aware that there are
new samples to copy. The game thread can then block in a busy loop with short
sleeps until the sound has been completely transfered.

I thought about using WebAssembly in full multi-threaded mode where a
SharedArrayBuffer would be used to allocate memory and multiple workers
would be used to simulate threads, but instead of going down that route just
for audio, I took the approach of simply commenting out these busy loops (of
which there were only a small handful). This avoided a lot of complexity,
although it would still be interested to go back and try one day.

Audio instead works by hooking the <code>PlaySound</code> calls and bypassing
the SDL API layer entirely. When sound is requested, the samples array (WAV
file format) is just transferred over <code>postMessage</code> to the main
page, where it is forwarded on to the WebAudio API. Easy enough, and for this
game, with its low fi audio, works fine.

**TODO**

Implement support for persisting game scores.
