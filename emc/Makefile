CXXFLAGS = -g -I/usr/include/emc2 -I../src 
LDFLAGS =
LIBS=-lemc -lnml -lemcini

vpath %.hpp ../src ../include
vpath %.cpp ../src

all: adapter

adapter: emc.o device_datum.o string_buffer.o string_array.o client.o server.o adapter.o emc_adapter.o logger.o service.o
	g++ ${CXXFLAGS} ${LDFLAGS} -o adapter $+ ${LIBS}

device_datum.o: device_datum.cpp device_datum.hpp string_buffer.hpp

string_buffer.o: string_buffer.cpp string_buffer.hpp

client.o: client.cpp client.hpp

server.o: server.cpp server.hpp client.hpp

adapter.o: adapter.cpp adapter.hpp server.hpp client.hpp

emc.o: emc.cpp emc_adapter.hpp adapter.hpp

emc_adapter.o: emc_adapter.cpp emc_adapter.hpp adapter.hpp server.hpp client.hpp

string_array.o: string_array.cpp string_array.hpp

logger.o: logger.cpp logger.hpp

service.o: service.cpp