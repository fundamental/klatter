CXXFLAGS =  -Wall -Wextra -g -O2 -ffast-math -std=gnu++11
CXX      = g++
CFLAGS =  -Wall -Wextra -g -ffast-math
CC       = clang

all: klatter
	fltk-config --compile window.cpp

HEADERS =Param/Utterance.h
OBJECTS =main.cpp audio.cpp Param/Words.cpp Param/State.cpp DSP/Formant.cpp DSP/Source.cpp DSP/Util.cpp Param/Mapper.cpp

klatter: $(OBJECTS) $(HEADERS)
	$(CXX) $(CXXFLAGS) `pkg-config jack --libs --cflags` $(OBJECTS) -o klatter
