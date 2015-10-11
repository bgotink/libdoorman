.PHONY: all clean

LIB_CXXSRC = $(wildcard doorman/*.cpp)

INCLUDE=-Idoorman
LIBS=-lwiringPi

CXXFLAGS=$(INCLUDE)
CCFLAGS=$(CXXFLAGS)

LIB_OBJS = $(addprefix build/, $(subst /,_,$(LIB_CXXSRC:.cpp=.o)))

all: bin/doorman dist/libdoorman.so dist/libdoorman.a

clean:
	@echo "Cleaning..."
	@rm -rf build/* bin/* dist/*

build/doorman_%.o: doorman/%.cpp
	@echo "Compiling" $<
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

build/main_%.o: doorman/main/%.cpp
	@echo "Compiling main file" $<
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

bin/%: build/main_%.o dist/libdoorman.a
	@echo "Creating binary" $@
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $+ -o $@

dist/libdoorman.a: $(LIB_OBJS)
	@echo "Linking static library" $@
	@ar rcs $@ $(LIB_OBJS)

dist/libdoorman.so: $(LIB_OBJS)
	@echo "Linking dynamic library" $@
	@$(CXX) -shared -Wl,-soname,libdoorman.so.1 -o dist/libdoorman.so.1.0.0  $(LIB_OBJS)
	@cd dist && ln -s libdoorman.so.1.0.0 libdoorman.so.1.0 && ln -s libdoorman.so.1.0 libdoorman.so.1 && ln -s libdoorman.so.1 libdoorman.so
