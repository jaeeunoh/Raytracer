ROOT     := .
TARGETS  := raytracer
CXXFLAGS := `/home/curtsinger/bin/sdl2-config --cflags` -g -O2 --std=c++11 -I/home/curtsinger/include
LDFLAGS  := `/home/curtsinger/bin/sdl2-config --libs` -lpthread

include $(ROOT)/common.mk
