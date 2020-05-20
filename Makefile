CXXFLAGS=-std=c++14
CFLAGS=-g -lsfml-graphics -lsfml-window -lsfml-system
CPPFLAGS=
LDFLAGS=
LDLIBS=
LIBS=
CPP=g++

main: main.o entity.o tile.o builder.o world.o game.o
	$(CPP) $(CXXFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

clean:
	\rm -f *.o main
