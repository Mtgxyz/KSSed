#    KSSed - A level editor for Kirby Super Star/Kirby's Fun Pak and Kirby of the Stars Super Deluxe
#    Copyright (C) 2015  Morten Delenk
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
include nall/GNUmakefile
target = kssed
# compiler
flags += -I. -g -fno-wrapv -Werror #-O3
objects := libco

# profile-guided optimization mode
# pgo := instrument
# pgo := optimize
link += -lexhal
ifeq ($(pgo),instrument)
  flags += -fprofile-generate
  link += -lgcov
else ifeq ($(pgo),optimize)
  flags += -fprofile-use
endif

# platform
ifeq ($(platform),windows)
  ifeq ($(arch),x86)
    flags += -m32
    link += -m32
  endif
  ifeq ($(console),true)
    link += -mconsole
  else
    link += -mwindows
  endif
  link += -mthreads -luuid -lkernel32 -luser32 -lgdi32 -lcomctl32 -lcomdlg32 -lshell32 -lole32 -lws2_32
  link += -Wl,-enable-auto-import
  link += -Wl,-enable-runtime-pseudo-reloc
else ifeq ($(platform),macosx)
  flags += -march=native
else ifeq ($(platform),linux)
  #flags += -march=native -fopenmp
  link += -fopenmp
  link += -Wl,-export-dynamic
  link += -lX11 -lXext -ldl
else ifeq ($(platform),bsd)
  flags += -march=native -fopenmp
  link += -fopenmp
  link += -Wl,-export-dynamic
  link += -lX11 -lXext
else
  $(error unsupported platform.)
endif

ui := target-$(target)

# implicit rules
compile = \
  $(strip \
    $(if $(filter %.c,$<), \
      $(compiler) $(cflags) $(flags) $1 -c $< -o $@, \
      $(if $(filter %.cpp,$<), \
        $(compiler) $(cppflags) $(flags) $1 -c $< -o $@ \
      ) \
    ) \
  )

%.o: $<; $(call compile)

all: build;

obj/libco.o: libco/libco.c libco/*

include $(ui)/GNUmakefile
flags := $(flags) $(foreach o,$(call strupper,$(options)),-D$o)

# targets
clean:
	-@$(call delete,out/*)
	-@$(call delete,obj/*.o)
	-@$(call delete,obj/*.a)
	-@$(call delete,obj/*.so)
	-@$(call delete,obj/*.dylib)
	-@$(call delete,obj/*.dll)


sync:
ifeq ($(shell id -un),byuu)
	if [ -d ./libco ]; then rm -r ./libco; fi
	if [ -d ./nall ]; then rm -r ./nall; fi
	if [ -d ./ruby ]; then rm -r ./ruby; fi
	if [ -d ./hiro ]; then rm -r ./hiro; fi
	cp -r ../libco ./libco
	cp -r ../nall ./nall
	cp -r ../ruby ./ruby
	cp -r ../hiro ./hiro
	rm -r libco/doc
	rm -r libco/-test
	rm -r nall/-test
	rm -r ruby/-test
	rm -r hiro/-test
endif

help:;