
CXXFLAGS = -g -I../src -DTHREADED
LDFLAGS = 
LIBS = -lportaudio

vpath %.hpp ../src
vpath %.cpp ../src

all: audio

audio: audio.o device_datum.o string_buffer.o client.o server.o adapter.o audio_adapter.o \
	service.o logger.o condition.o time_series.o
	g++ ${CXXFLAGS} ${LDFLAGS} -o audio $+ ${LIBS}

device_datum.o: device_datum.cpp device_datum.hpp string_buffer.hpp

string_buffer.o: string_buffer.cpp string_buffer.hpp

client.o: client.cpp client.hpp

server.o: server.cpp server.hpp client.hpp

adapter.o: adapter.cpp adapter.hpp server.hpp client.hpp

audio.o: audio.cpp audio_adapter.hpp adapter.hpp

audio_adapter.o: audio_adapter.cpp audio_adapter.hpp adapter.hpp server.hpp client.hpp condition.hpp

condition.o: condition.cpp condition.hpp device_datum.hpp

logger.o: logger.cpp logger.hpp

service.o: service.cpp service.hpp

time_series.o: time_series.cpp time_series.hpp 
