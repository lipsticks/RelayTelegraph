ROOT:=../../..
BIN:=$(ROOT)/bin
ICONS:=$(BIN)/icons
MODULES:=$(ROOT)/modules
CONTROLS:=$(ROOT)/controls
CPPFLAGS:=-g -lgdi32 -lcomctl32 -Xlinker --subsystem=windows

main.exe: main.cpp resource.o lib.win.o lib.parser.o app.parser.o TourneyListView.o mainPane.o regex2.dll
	@g++ -o $@ $^ $(CPPFLAGS)

resource.o: resource.rc redeye.ico
	@windres resource.rc resource.o

redeye.ico:
	@cp $(ICONS)/redeye.ico .

TourneyListView.o: $(MODULES)/app/win/TourneyListView/TourneyListView.cpp
	@g++ -c -o $@ $^

app.parser.o: $(MODULES)/app/parser/parser.cpp
	@g++ -c -o $@ $^

lib.parser.o: $(MODULES)/lib/parser/parser.cpp
	@g++ -c -o $@ $^

lib.win.o: $(MODULES)/lib/win/win.cpp
	@g++ -c -o $@ $^

mainPane.o: $(CONTROLS)/mainPane/mainPane.cpp
	@g++ -c -o $@ $^

regex2.dll:
	@cp $(BIN)/regex2.dll .

run: main.exe
	@main.exe

clean:
	@rm -f main.exe
	@rm -f *.o
	@rm -f regex2.dll
	@rm -f redeye.ico
