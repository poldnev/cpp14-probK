.PHONY: all clean test

CC = clang++
CFLAGS = -std=c++1y -O3 -Weverything -Wno-c++98-compat -Wno-missing-prototypes -Wno-weak-vtables -Wno-padded -Wno-exit-time-destructors -Wno-global-constructors
LDFLAGS = -s
MAIN_CFILES = process_table.cpp expression_table.cpp utils.cpp coordinate.cpp sparse_table.cpp expression.cpp
MAIN_HFILES = expression_table.h utils.h coordinate.h sparse_table.h expression.h logger.h
MAIN_OBJECTS = $(MAIN_CFILES:.cpp=.o)
TEST_CFILES = unit_test.cpp test_sparse_table.cpp test_make_lexem_pointer.cpp test_parse_expression.cpp test_read_text_table.cpp test_calculate_parsed_table.cpp
TEST_HFILES = unit_test.h
TEST_OBJECTS = $(TEST_CFILES:.cpp=.o)
CFILES = $(MAIN_CFILES) $(TEST_CFILES)
HFILES = $(MAIN_HFILES) $(TEST_HFILES)
OBJECTS = $(MAIN_OBJECTS) $(TEST_OBJECTS)

MAIN_TARGET = process_table
TEST_TARGETS = test_sparse_table test_make_lexem_pointer test_parse_expression test_read_text_table test_calculate_parsed_table

all: $(MAIN_TARGET) run_unit_test

clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGETS) *.o run_unit_test
    
process_table: $(MAIN_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@
    
run_unit_test: $(TEST_TARGETS)
	./generate_test "$^"
    
test_sparse_table: test_sparse_table.o unit_test.o coordinate.o sparse_table.o
	$(CC) $(LDFLAGS) $^ -o $@
    
test_make_lexem_pointer: test_make_lexem_pointer.o unit_test.o utils.o coordinate.o expression.o
	$(CC) $(LDFLAGS) $^ -o $@
    
test_parse_expression: test_parse_expression.o unit_test.o utils.o coordinate.o expression.o
	$(CC) $(LDFLAGS) $^ -o $@
    
test_read_text_table: test_read_text_table.o unit_test.o utils.o coordinate.o sparse_table.o expression.o expression_table.o
	$(CC) $(LDFLAGS) $^ -o $@
    
test_calculate_parsed_table: test_calculate_parsed_table.o unit_test.o utils.o coordinate.o sparse_table.o expression.o expression_table.o
	$(CC) $(LDFLAGS) $^ -o $@
   
%.o : %.cpp
	$(CC) -c $(CFLAGS) $<
    
include deps.make
deps.make : $(CFILES) $(HFILES)
	$(CC) -MM -std=c++1y $(CFILES) > deps.make
