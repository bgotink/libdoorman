.PHONY: all clean

all: doorman

%.o: %.c
	$(CXX) $(CXXFLAGS) -o $@ $<

doorman: Doorman.o main.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi

clean:
	@rm *.o doorman
