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
# By default we will build a Debug configuration using the GCC newlib toolcahin
# to override this, specify TOOLCHAIN=newlib|glibc or CONFIG=Debug|Release on
# the make command-line or in this file prior to including common.mk.  The
# toolchain we use by default will be the first valid one listed
VALID_TOOLCHAINS:=newlib glibc pnacl linux win

#CONFIG=Debug
CONFIG=Release


#
# Get pepper directory for toolchain and includes.
#
# If NACL_SDK_ROOT is not set, then assume it can be found relative to
# to this Makefile.
#
NACL_SDK_ROOT?=$(abspath $(CURDIR)/../../packages/nacl_sdk/pepper_31)
include $(NACL_SDK_ROOT)/tools/common.mk


#
# Target Name
#
# The base name of the final NEXE, also the name of the NMF file containing
# the mapping between architecture and actual NEXE.
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
  port.c \
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
	myerror.cpp \
  ppapi/ppb.cpp \
  ppapi/pp_resource_queue.cpp \
  ppapi/ppapi_main.cpp \
  $(NULL)

EXTRA_INC_PATHS=. netlogic screenlib maclib SDL ppapi


#
# List of libraries to link against.  Unlike some tools, the GCC and LLVM
# based tools require libraries to be specified in the correct order.  The
# order should be symbol reference followed by symbol definition, with direct
# sources to the link (object files) are left most.  In this case:
#    hello_world -> ppapi_main -> ppapi_cpp -> ppapi -> pthread -> libc
# Notice that libc is implied and come last through standard compiler/link
# switches.
#
# We break this list down into two parts, the set we need to rebuild (DEPS)
# and the set we do not.  This example does not havea any additional library
# dependencies.
#
DEPS=
LIBS=$(DEPS) ppapi pthread


#
# Use the library dependency macro for each dependency
#
$(foreach dep,$(DEPS),$(eval $(call DEPEND_RULE,$(dep))))

#
# Use the compile macro for each source.
#
$(foreach src,$(SOURCES),$(eval $(call COMPILE_RULE,$(src))))

#
# Use the link macro for this target on the list of sources.
#
$(eval $(call LINK_RULE,$(TARGET),$(SOURCES),$(LIBS),$(DEPS)))

#
# Specify the NMF to be created with no additional arugments.
#
$(eval $(call NMF_RULE,$(TARGET),))