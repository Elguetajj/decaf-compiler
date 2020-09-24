compilerExe: ./build/obj/main.o ./build/obj/Automata.o ./build/obj/Regex.o
	g++ ./build/obj/main.o ./build/obj/Automata.o ./build/obj/Regex.o -o build/bin/decaf-compiler

./build/obj/main.o: ./build/obj/Automata.o ./build/obj/Regex.o
	g++ -c ./compiler/scanner/main.cpp -o ./build/obj/main.o

./build/obj/Automata.o:
	g++ -c ./compiler/scanner/Automata.cpp -o ./build/obj/Automata.o

./build/obj/Regex.o:
	g++ -c ./compiler/scanner/Regex.cpp -o ./build/obj/Regex.o

# suffix list
# .SUFFIXES:
# .SUFFIXES: .cpp .hpp .o

# # setup and dirs
# CC	= g++
# BINDIR	= ./build/bin/
# OBJDIR	= ./build/obj/
# SRCDIR	= ./compiler/

# # final executable name
# _BIN = compiler
# BIN = $(addprefix $(BINDIR),$(_BIN))


# # all object files will be stored in $(OBJDIR)
# SRC	= $(wildcard compiler/**/*.cpp)
# _OBJS	= $(patsubst %.cpp, %.o, $(SRC))
# OBJS	= $(addprefix $(OBJDIR), $(_OBJS))




# $(BIN): $(BINDIR) $(OBJS)
# 	$(CC) $(OFLAGS) $(OBJS) -o $(BIN)

# $(BINDIR):
# 	mkdir $(BINDIR)

# $(OBJS): $(OBJDIR) $(SRC)
# 	$(CC) $(CFLAGS) -c $(patsubst %.o, %.cpp, $@) -o $@ 

# $(OBJDIR):
# 	mkdir $(OBJDIR)
