# Compiler settings
CC = clang -std=c2x
CFLAGS = -Wall -Wextra -g -Isrc

# Source files for the main program
SOURCES = src/main.c src/lexer.c src/parser.c src/ast.c src/value.c

# Object files for the main program
OBJECTS = $(SOURCES:.c=.o)

# Executable name for the main program
EXECUTABLE = chimp

# Test source files for lexer tests
TEST_LEXER_SOURCES = tests/test_lexer.c tests/test_helper_functions.c src/lexer.c 

# Object files for lexer tests
TEST_LEXER_OBJECTS = $(TEST_LEXER_SOURCES:.c=.o)

# Test executable name for lexer tests
TEST_LEXER_EXECUTABLE = test_lexer

# Test source files for parser tests
TEST_PARSER_SOURCES = tests/test_parser.c tests/test_helper_functions.c src/lexer.c src/parser.c src/ast.c

# Object files for parser tests
TEST_PARSER_OBJECTS = $(TEST_PARSER_SOURCES:.c=.o)

# Test executable name for parser tests
TEST_PARSER_EXECUTABLE = test_parser

# Test source files for parser tests
TEST_VALUE_SOURCES = tests/test_value.c 

# Object files for parser tests
TEST_VALUE_OBJECTS = $(TEST_VALUE_SOURCES:.c=.o)

# Test executable name for parser tests
TEST_VALUE_EXECUTABLE = test_value

# Build the main executable
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Compile object files for the main program
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile object files for the lexer tests
tests/%.o: tests/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Build lexer tests
$(TEST_LEXER_EXECUTABLE): $(TEST_LEXER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(TEST_LEXER_OBJECTS)

# Build parser tests
$(TEST_PARSER_EXECUTABLE): $(TEST_PARSER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(TEST_PARSER_OBJECTS)

# Build value tests
$(TEST_VALUE_EXECUTABLE): $(TEST_VALUE_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(TEST_VALUE_OBJECTS)

# Run lexer tests
run_test_lexer: $(TEST_LEXER_EXECUTABLE)
	./$(TEST_LEXER_EXECUTABLE)

# Run parser tests
run_test_parser: $(TEST_PARSER_EXECUTABLE)
	./$(TEST_PARSER_EXECUTABLE)

# Run value tests
run_test_value: $(TEST_VALUE_EXECUTABLE)
	./$(TEST_VALUE_EXECUTABLE)

# Build and run all tests
test: $(TEST_LEXER_EXECUTABLE) $(TEST_PARSER_EXECUTABLE) $(TEST_VALUE_EXECUTABLE)
	./$(TEST_LEXER_EXECUTABLE)
	./$(TEST_PARSER_EXECUTABLE)
	./$(TEST_VALUE_EXECUTABLE)

# Clean up build artifacts
clean:
	rm -f $(EXECUTABLE) $(OBJECTS) $(TEST_LEXER_OBJECTS) $(TEST_PARSER_OBJECTS) $(TEST_LEXER_EXECUTABLE) $(TEST_PARSER_EXECUTABLE) $(TEST_VALUE_EXECUTABLE)

# Mark targets as phony
.PHONY: all clean test run_test_lexer run_test_parser run_test_value
