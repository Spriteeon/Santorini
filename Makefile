CXXFLAGS=-std=c++14
CFLAGS=-g -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -pthread
CPPFLAGS=
LDFLAGS=
LDLIBS=
LIBS=
CPP=g++

all: main server

main: builder.o entity.o game.o main.o message.o network.o receiver.o tile.o util.o world.o
	$(CPP) $(CXXFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

server: util.o server.o
	$(CPP) $(CXXFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

clean:
	\rm -f *.o main server
