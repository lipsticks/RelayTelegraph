ROOT:=../..
BIN:=$(ROOT)/bin
ICONS:=$(BIN)/icons
MODULES:=$(ROOT)/modules
CPPFLAGS:=-g -lgdi32 -lcomctl32 -Xlinker --subsystem=windows

main.exe: main.cpp resource.o lib.win.o regex2.dll RelayTelegraph.BCPlugin
	@g++ -o $@ $^ $(CPPFLAGS)

resource.o: resource.rc redeye.ico
	@windres resource.rc resource.o

redeye.ico:
	@cp $(ICONS)/redeye.ico .

lib.win.o: $(MODULES)/lib/win/win.cpp
	@g++ -c -o $@ $^

regex2.dll:
	@cp $(BIN)/regex2.dll .

RelayTelegraph.BCPlugin:
	@make -C $(ROOT)/main
	@cp $(ROOT)/main/RelayTelegraph.BCPlugin .

run: main.exe
	@main.exe

clean:
	@make clean -C $(ROOT)/main
	@rm -f main.exe
	@rm -f RelayTelegraph.BCPlugin
	@rm -f *.o
	@rm -f regex2.dll
	@rm -f redeye.ico
