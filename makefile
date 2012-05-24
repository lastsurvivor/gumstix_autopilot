#
# Makefile for 'autoPilot1.0'.
#
# Type 'make' or 'make autoPilot1.0' to create the binary.
# Type 'make clean' or 'make clear' to delete all temporaries.
# Type 'make run' to execute the binary.
# Type 'make debug' to debug the binary using gdb(1).
#

# build target specs
CC = g++
CFLAGS = -O3 
OUT_DIR = release
LIBS = -lpthread

# first target entry is the target invoked when typing 'make'
default: autoPilot1.0

autoPilot1.0: $(OUT_DIR)/gumstix_autopilot.cpp.o $(OUT_DIR)/SystemThreads.cpp.o $(OUT_DIR)/Utility.cpp.o
	@echo -n 'Linking autoPilot1.0... '
	@$(CC) $(CFLAGS) -o autoPilot1.0 $(OUT_DIR)/gumstix_autopilot.cpp.o $(OUT_DIR)/SystemThreads.cpp.o $(OUT_DIR)/Utility.cpp.o $(LIBS)
	@echo Done.

$(OUT_DIR)/gumstix_autopilot.cpp.o: gumstix_autopilot.cpp SystemStatus.h \
 SystemConfig.h SharedMemory.h SystemThreads.h Utility.h
	@echo -n 'Compiling gumstix_autopilot.cpp... '
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/gumstix_autopilot.cpp.o -c gumstix_autopilot.cpp
	@echo Done.

$(OUT_DIR)/SystemThreads.cpp.o: SystemThreads.cpp SystemThreads.h
	@echo -n 'Compiling SystemThreads.cpp... '
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/SystemThreads.cpp.o -c SystemThreads.cpp
	@echo Done.

$(OUT_DIR)/Utility.cpp.o: Utility.cpp Utility.h
	@echo -n 'Compiling Utility.cpp... '
	@$(CC) $(CFLAGS) -o $(OUT_DIR)/Utility.cpp.o -c Utility.cpp
	@echo Done.

run:
	./autoPilot1.0 

debug:
	gdb ./autoPilot1.0

clean:
	@echo -n 'Removing all temporary binaries... '
	@rm -f autoPilot1.0 $(OUT_DIR)/*.o
	@echo Done.

clear:
	@echo -n 'Removing all temporary binaries... '
	@rm -f autoPilot1.0 $(OUT_DIR)/*.o
	@echo Done.

