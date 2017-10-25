# compile define
CC = g++
AR = ar rc
CPPFLAGS = -std=c++11 -m64 -DLINUX_PLT -g 
INC_DIR = -I.
LIB_DIR = -L.

# build define
LIBS = -lpthread
COMMON_OBJS = log.o thread.o buffer.o
TARGETS = ft_log


.phony : all clean
all: $(TARGETS)
	@echo "Make Done!"

# .o
%.o : %.cpp
	$(CC) $(CPPFLAGS) $(INC_DIR) -c $< -o $@

ft_log : ft_log.o $(COMMON_OBJS)
	$(CC) $(CPPFLAGS) $(INC_DIR) -o $@ $^ $(LIB_DIR) $(LIBS)

clean:
	@rm -rf *.o
	@echo "Clean Done!"

