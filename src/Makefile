SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)
LDFLAGS = $(shell pkg-config --libs gtkmm-2.4 gtkglextmm-1.2 lua) -llua -lpng
CPPFLAGS = $(shell pkg-config --cflags gtkmm-2.4 gtkglextmm-1.2 lua)
CXXFLAGS = $(CPPFLAGS) -W -Wall -g
CXX = g++
MAIN = raymond

all: $(MAIN)

debug: CXX += -DDEBUG -g
debug: $(MAIN)

depend: $(DEPENDS)

clean:
	rm -f *.o *.d $(MAIN)

$(MAIN): $(OBJECTS)
	@echo Creating $@...
	@$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	@echo Compiling $<...
	@$(CXX) -o $@ -c $(CXXFLAGS) $<

%.d: %.cpp
	@echo Building $@...
	@set -e; $(CC) -M $(CPPFLAGS) $< \
                  | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
                [ -s $@ ] || rm -f $@

include $(DEPENDS)
