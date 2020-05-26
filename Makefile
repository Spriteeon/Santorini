CXXFLAGS=-std=c++14
CFLAGS=-g -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -pthread
CPPFLAGS=
LDFLAGS=
LDLIBS=
LIBS=
CPP=g++

all: client server

client: builder.o entity.o game.o client.o message.o network.o receiver.o accepter.o tile.o util.o world.o
	$(CPP) $(CXXFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

server: util.o server.o accepter.o receiver.o message.o startServer.o
	$(CPP) $(CXXFLAGS) $(LIBS) $^ -o $@ $(CFLAGS)

clean:
	\rm -f *.o client server
