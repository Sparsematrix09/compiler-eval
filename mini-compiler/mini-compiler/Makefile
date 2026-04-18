# ============================================================
#  Mini-Compiler Makefile
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
TARGET   := compiler

SRCS := main.cpp lexer.cpp ast.cpp parser.cpp semantic.cpp codegen.cpp
OBJS := $(SRCS:.cpp=.o)

# ── Default target ───────────────────────────────────────────
.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# ── Pattern rule: compile each .cpp → .o ─────────────────────
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# ── Explicit header dependencies ─────────────────────────────
main.o:     main.cpp     lexer.h ast.h parser.h semantic.h codegen.h
lexer.o:    lexer.cpp    lexer.h
ast.o:      ast.cpp      ast.h
parser.o:   parser.cpp   parser.h lexer.h ast.h
semantic.o: semantic.cpp semantic.h ast.h
codegen.o:  codegen.cpp  codegen.h ast.h

# ── Convenience targets ───────────────────────────────────────
.PHONY: run
run: all
	./$(TARGET)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: rebuild
rebuild: clean all
