MODULES:=../../../modules
CONTROLS:=../../../controls
CPPFLAGS:=-g -lgdi32 -lcomctl32 -Xlinker --subsystem=windows

main.exe: main.cpp resource.o lib.win.o mainPane.o
	@g++ -o $@ $^ $(CPPFLAGS)

resource.o: resource.rc
	@windres resource.rc resource.o

lib.win.o: $(MODULES)/lib/win/win.cpp
	@g++ -c -o $@ $^

mainPane.o: $(CONTROLS)/mainPane/mainPane.cpp
	@g++ -c -o $@ $^

run: main.exe
	@main.exe

clean:
	@rm -f main.exe
	@rm -f *.o
