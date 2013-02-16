ROOT:=../../..
MODULES:=$(ROOT)/modules
BIN:=$(ROOT)/bin

test.exe: test.cpp app.parser.o lib.parser.o regex2.dll
	@g++ -o $@ test.cpp app.parser.o lib.parser.o -L$(MODULES)/extern/regex -lregex

app.parser.o: $(MODULES)/app/parser/parser.cpp $(MODULES)/app/parser/parser.h
	@g++ -c -o $@ $(MODULES)/app/parser/parser.cpp

lib.parser.o: $(MODULES)/lib/parser/parser.cpp $(MODULES)/lib/parser/parser.h
	@g++ -c -o $@ $(MODULES)/lib/parser/parser.cpp

regex2.dll:
	@cp $(BIN)/regex2.dll .

run: test.exe
	@./$^

clean:
	@rm -f *.exe
	@rm -f *.o
	@rm -f *.dll
