CXX=g++
CXXFLAGS=-I.
OBJ=io_handler.o parser.o extractor.o
TESTOBJ=io_handler.o parser.o

TESTDIR=tests
TESTS= test00

%.o: %.c 
	$(CXX) -c -o $@ $< $(CXXFLAGS)

extractor: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(TESTS): $(TESTOBJ)
	$(CXX) $(CXXFLAGS) -o $(TESTDIR)/$@.o -c $(TESTDIR)/$@.cpp 
	$(CXX) $(CXXFLAGS) -o $(TESTDIR)/$@ $^ $(TESTDIR)/$@.o 

.PHONY: clean

clean:
	rm -f *.o $(TESTDIR)/*.o $(TESTDIR)/$(TESTS) extractor
