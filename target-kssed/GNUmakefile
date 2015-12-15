name := kssed
ui_objects := ui-kssed ui-config ui-mainwin ui-rom ui-room
ui_objects += ruby hiro
ui_objects += $(if $(call streq,$(platform),windows),ui-resource)
ifeq ($(platform),windows)
	ruby += video.direct3d video.wgl video.directdraw video.gdi
	ruby += audio.xaudio2 audio.directsound
	ruby += input.windows
else ifeq ($(platform),macosx)
	ruby += video.cgl
	ruby += audio.openal
	ruby += input.carbon
else ifeq ($(platform),linux)
	ruby += video.glx video.xv video.xshm video.sdl
	ruby += audio.alsa audio.openal audio.oss audio.pulseaudio audio.ao
	ruby += input.udev input.sdl input.xlib
else ifeq ($(platform),bsd)
	ruby += video.glx video.xv video.xshm video.sdl
	ruby += audio.openal audio.oss
	ruby += input.sdl input.xlib
endif
include ruby/GNUmakefile
link += $(rubylink)
include hiro/GNUmakefile
link += $(hirolink)
objects := $(ui_objects) $(objects)
objects := $(patsubst %,obj/%.o,$(objects))
obj/ruby.o: ruby/ruby.cpp $(call rwildcard,ruby/)
	$(compiler) $(rubyflags) -c $< -o $@
obj/hiro.o: hiro/hiro.cpp $(call rwildcard,hiro/)
	$(compiler) $(hiroflags) -c $< -o $@
obj/ui-kssed.o: $(ui)/kssed.cpp $(call rwildcard,$(ui)/)
obj/ui-config.o: $(ui)/configuration/config.cpp $(call rwildcard,$(ui)/)
obj/ui-mainwin.o: $(ui)/mainwin/mainwindow.cpp $(call rwildcard,$(ui)/)
obj/ui-rom.o: $(ui)/rom/rom.cpp $(call rwildcard,$(ui)/)
obj/ui-rom.o: $(ui)/sdx/room.cpp $(call rwildcard,$(ui)/)
obj/ui-resource.o:
	windres data/resource.rc obj/ui-resource.o
build: $(objects)
	$(strip $(compiler) -o out/$(name) $(objects) $(link))
	
install:
ifeq ($(shell id -un),root)
	$(error "make install should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
else
	mkdir -p $(prefix)/bin/
	mkdir -p $(prefix)/share/icons/
	mkdir -p $(prefix)/$(name)/
	cp out/$(name) $(prefix)/bin/$(name)
endif

uninstall:
ifeq ($(shell id -un),root)
	$(error "make uninstall should not be run as root")
else ifeq ($(platform),windows)
else ifeq ($(platform),macosx)
else
	if [ -f $(prefix)/bin/$(name) ]; then rm $(prefix)/bin/$(name); fi
	if [ -f $(prefix)/share/icons/$(name).png ]; then rm $(prefix)/share/icons/$(name).png; fi
endif