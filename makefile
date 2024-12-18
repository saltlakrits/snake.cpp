CC=g++
OUT = snake
CFLAGS=-Isrc/
SRCDIR = src/
#OBJDIR = .obj/
CPPFILES = $(wildcard $(SRCDIR)*.cpp)
OBJS = $(CPPFILES:.cpp=.o)

LDLIBS = -lfmt -lncursesw
#@mkdir -p .obj

main: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT) $(LDLIBS)
	@printf "\n === Running program ===\n"
	#@./$(OUT)
	@rm -rf $(SRCDIR)*.o
