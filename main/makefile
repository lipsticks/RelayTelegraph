ROOT:=../
BIN:=$(ROOT)/bin
ICONS:=$(BIN)/icons
MODULES:=$(ROOT)/modules
CONTROLS:=$(ROOT)/controls
BABAS:=c:/Programme/Games/BabasChess
CPPFLAGS:=-g -lgdi32 -lcomctl32 -Xlinker --subsystem=windows -L$(MODULES)/extern/regex -lregex

RelayTelegraph.BCPlugin: main.cpp resource.o lib.win.o app.parser.o lib.parser.o TourneyListView.o mainPane.o
	@g++ --shared -o $@ $^ $(CPPFLAGS)

resource.o: resource.rc tabicon.bmp
	@windres resource.rc resource.o

tabicon.bmp:
	@cp $(ICONS)/tabicon.bmp .

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

commit: RelayTelegraph.BCPlugin
	@cp RelayTelegraph.BCPlugin $(BABAS)/.
	@cp $(BIN)/regex2.dll $(BABAS)/.

clean:
	@rm -f *.BCPlugin
	@rm -f *.o
	@rm -f tabicon.bmp
