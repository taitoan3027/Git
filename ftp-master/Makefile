CROSS_COMPILE	:= arm-linux-gnueabihf-
CFLAGS=-c -Wall
LDFLAGS=-lm
SOURCES=server.c handles.c client.c client.h /home/taitoan3027/Documents/ADAM3600SDK_1.2.1_20160519/DCTag/include/DCTag.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ftp
CPU_FLAG		:= -mcpu=cortex-a8

ifndef INC_DIR
INC_DIR			:= -I. -I"$(shell pwd)"
endif

ifndef LIB_DIR
LIB_DIR			:= -L"$(shell pwd)"
endif

$(EXECUTABLE): $(OBJECTS) 
	$(CROSS_COMPILE)gcc  $(CPU_FLAG) $(OBJECTS)  -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) -o $@

.cpp.o:
	$(CROSS_COMPILE)gcc $(CFLAGS) -lAPAL -lDCTag $(INC_DIR) $(LIB_DIR) $< -o $@

clean: 
	rm -rf *.o ftp
