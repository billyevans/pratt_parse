default: all

include Makefile.opts

SOURCES_LIB = symbol_table.cpp parse_state.cpp
SOURCES_EXAMPLE = example.cpp

OBJECTS_LIB = $(SOURCES_LIB:.cpp=.o)

LIB = libpratt_parse.a
EXAMPLE = example

OBJECTS_CXX += $(OBJECTS_LIB)
TMPS += $(OBJECTS_LIB)
TARGETS += $(LIB)
TARGETS += $(EXAMPLE)

$(LIB): $(OBJECTS_LIB)
	ar crs $(@) $(^)

$(EXAMPLE): $(LIB) $(SOURCES_EXAMPLE)
	$(CXX) $(CXXFLAGS) -o $(@) $(^)

$(OBJECTS_C): $(OBJECTS_CXX): $(TARGETS) $(TMPS): Makefile.opts

$(OBJECTS_C): %.o: %.c
	$(CC) -c $(CFLAGS) -o $(@) $(<)

$(OBJECTS_CXX): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $(@) $(<)

all: $(TARGETS)

clean:; @rm -f $(TARGETS) $(TMPS) deps/*.d

.PHONY: default all clean
