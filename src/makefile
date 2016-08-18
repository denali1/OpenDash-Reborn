#
# update this variable to wherever you installed the OpenXDK libraries
#
PREFIX = /usr/local/openxdk

OBJS = Environment.o MenuNode.o SubMenuNode.o FileMenu.o DialogBox.o XBEMenu.o SkinMenu.o BootXBE.o LoadSkin.o FileAction.o XboxAction.o LaunchDVD.o LoadKey.o Resources.o Resource.o FontResource.o ImageResource.o Element.o Elements.o Generic.o Setting.o XboxInfo.o QuestionBox.o Script.o Controller.o HotKey.o OpenDash.o

CPP = i386-pc-xbox-g++
CXBE = cxbe

SDLFLAGS = -DENABLE_XBOX -DDISABLE_CDROM -DBUILD=`cat build.txt`
CPP_FLAGS = -c -g -O2 -nostdlib -fno-builtin -fno-exceptions -mno-cygwin -march=i386 $(SDLFLAGS)
INCLUDE  = -I${PREFIX}/i386-pc-xbox/include -I${PREFIX}/include -I${PREFIX}/include/SDL

CPPLINK = -nostdlib
ALIGN = -Wl,--file-alignment,0x20 -Wl,--section-alignment,0x20 
SHARED = -shared
ENTRYPOINT = -Wl,--entry,_WinMainCRTStartup 
STRIP = -Wl,--strip-all

LD_FLAGS = $(CPPLINK) $(ALIGN) $(SHARED) $(ENTRYPOINT) $(STRIP)
LD_DIRS = -L${PREFIX}/i386-pc-xbox/lib -L${PREFIX}/lib 
LD_LIBS  = $(LD_DIRS) -lSDL_ttf -lfreetype2 -lSDL_image -lSDL -ljpeg -lpng -lz -lxml2 -lm -lopenxdk -lhal -lc -lhal -lc -lusb -lc -lxboxkrnl -lc -lhal -lxboxkrnl -lhal -lopenxdk -lstdc++ -lgcc

LD_TEST_LIBS = $(LD_DIRS) -lc -lg -lm -lopenxdk -lhal -lc -lhal -lc -lopenxdk -lusb -lxboxkrnl -lc -lstdc++ -lgcc


all: \
	OpenDash.exe

.cpp.o:
	$(CPP) -c $< $(CPP_FLAGS) $(INCLUDE)

OpenDash.exe: $(OBJS)
	./increment.sh
	$(CPP) -o $@ $(OBJS) $(LD_LIBS) $(LD_FLAGS) 
	$(CXBE) -TITLE:"OpenDash" -DUMPINFO:"OpenDash.cxbe" -OUT:"OpenDash.xbe" $@
	
testxinfo.exe: testxinfo.o XboxInfo.o MenuNode.o
	$(CPP) -o $@ testxinfo.o XboxInfo.o MenuNode.o $(LD_TEST_LIBS) $(LD_FLAGS) 
	$(CXBE) -TITLE:"Test XboxInfo" -DUMPINFO:"testxinfo.cxbe" -OUT:"testxinfo.xbe" $@

testController.exe: testController.o Controller.o
	$(CPP) -o $@ testController.o Controller.o -lSDL $(LD_TEST_LIBS) $(LD_FLAGS) 
	$(CXBE) -TITLE:"Test Controller" -DUMPINFO:"testController.cxbe" -OUT:"testController.xbe" $@

testKey.exe: testKey.o
	$(CPP) -o $@ testKey.o -lSDL $(LD_TEST_LIBS) $(LD_FLAGS) 
	$(CXBE) -TITLE:"Test Key" -DUMPINFO:"testKey.cxbe" -OUT:"testKey.xbe" $@

clean: 
	rm -f *.o *.exe *.dll *.xbe *.cxbe


