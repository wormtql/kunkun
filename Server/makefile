PKG1=`pkg-config --cflags gtk+-3.0`
PKG2=`pkg-config --libs gtk+-3.0`

server: server.cpp
	g++ $(PKG1) -o server server.cpp $(PKG2)

# server.o: server.cpp
# 	g++ -s $(PKG1) -o server.o server.cpp $(PKG2)