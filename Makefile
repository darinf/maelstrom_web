# vim:ts=4 sw=4 sts=4 noet

# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

#
# GNU Make based build file.  For details on GNU Make see:
#   http://www.gnu.org/software/make/manual/make.html
#
#


#
# Default configuration
#

#CONFIG=Debug
CONFIG=Release


include common.mk


#
# Target Name
#
TARGET=maelstrom

#
# List of sources to compile
#
SOURCES= \
	checksum.cpp \
	controls.cpp \
	dialog.cpp \
	fastrand.cpp \
	init.cpp \
	load.cpp \
	main.cpp \
	netscore.cpp \
	rect.cpp \
	scores.cpp \
	netlogic/about.cpp \
	netlogic/blit.cpp \
	netlogic/game.cpp \
	netlogic/logic.cpp \
	netlogic/make.cpp \
	netlogic/netplay.cpp \
	netlogic/object.cpp \
	netlogic/objects.cpp \
	netlogic/player.cpp \
	maclib/Mac_FontServ.cpp \
	maclib/Mac_Resource.cpp \
	maclib/Mac_Sound.cpp \
	maclib/Mac_Wave.cpp \
	screenlib/pixel.cpp \
	screenlib/SDL_FrameBuf.cpp \
	myerror.cpp \
	SDL/SDL_audio.cpp \
	SDL/SDL_endian.cpp \
	SDL/SDL_event.cpp \
	SDL/SDL_init.cpp \
	SDL/SDL_mutex.cpp \
	SDL/SDL_net.cpp \
	SDL/SDL_rwops.cpp \
	SDL/SDL_surface.cpp \
	SDL/SDL_thread.cpp \
	SDL/SDL_timer.cpp \
	SDL/SDL_wm.cpp \
	$(NULL)

JS_LIBS= \
	jslib/canvas.js \
	$(NULL)

INC_PATHS=. netlogic screenlib maclib SDL


#
# List of libraries to link against.  Unlike some tools, the GCC and LLVM based
# tools require libraries to be specified in the correct order.  The order
# should be symbol reference followed by symbol definition, with direct sources
# to the link (object files) are left most.  Notice that libc is implied and
# come last through standard compiler/link switches.
#
# We break this list down into two parts, the set we need to rebuild (DEPS)
# and the set we do not.  This example does not havea any additional library
# dependencies.
#
DEPS=
LIBS=


#
# Use the link macro for this target on the list of sources.
#
$(eval $(call LINK_RULE,$(TARGET),$(SOURCES),$(LIBS),$(DEPS)))

#
# Use the library dependency macro for each dependency
#
$(foreach dep,$(DEPS),$(eval $(call DEPEND_RULE,$(dep))))

#
# Use the compile macro for each source.
#
$(foreach src,$(SOURCES),$(eval $(call COMPILE_RULE,$(src))))
