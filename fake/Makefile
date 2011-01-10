
CXXFLAGS = -g -I../src 
LDFLAGS = 
LIBS = 

vpath %.hpp ../src
vpath %.cpp ../src

all: adapter

adapter: fake.o device_datum.o string_buffer.o client.o server.o adapter.o fake_adapter.o service.o logger.o condition.o
	g++ ${CXXFLAGS} ${LDFLAGS} -o adapter $+ ${LIBS}

device_datum.o: device_datum.cpp device_datum.hpp string_buffer.hpp

string_buffer.o: string_buffer.cpp string_buffer.hpp

client.o: client.cpp client.hpp

server.o: server.cpp server.hpp client.hpp

adapter.o: adapter.cpp adapter.hpp server.hpp client.hpp

fake.o: fake.cpp fake_adapter.hpp adapter.hpp

fake_adapter.o: fake_adapter.cpp fake_adapter.hpp adapter.hpp server.hpp client.hpp condition.hpp

condition.o: condition.cpp condition.hpp device_datum.hpp

logger.o: logger.cpp logger.hpp

service.o: service.cpp service.hpp
