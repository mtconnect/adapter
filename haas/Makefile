CXXFLAGS = -g -I../src -I../include
LDFLAGS = 
LIBS=

vpath %.hpp ../src ../include
vpath %.cpp ../src

all: adapter

adapter: haas.o device_datum.o string_buffer.o string_array.o client.o server.o adapter.o haas_adapter.o serial.o haas_serial.o
	g++ ${CXXFLAGS} ${LDFLAGS} -o adapter $+ ${LIBS}

device_datum.o: device_datum.cpp device_datum.hpp string_buffer.hpp

string_buffer.o: string_buffer.cpp string_buffer.hpp

client.o: client.cpp client.hpp

server.o: server.cpp server.hpp client.hpp

adapter.o: adapter.cpp adapter.hpp server.hpp client.hpp

haas.o: haas.cpp haas_adapter.hpp adapter.hpp

haas_adapter.o: haas_adapter.cpp haas_adapter.hpp adapter.hpp server.hpp client.hpp haas_serial.hpp serial.hpp

string_array.o: string_array.cpp string_array.hpp

serial.o: serial.cpp serial.hpp serial.unix.cpp

haas_serial.o: haas_serial.cpp haas_serial.hpp serial.hpp
