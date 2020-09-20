CXX=g++
CXXFLAGS=-I. -g
OBJ=io_handler.o parser.o extractor.o

%.o: %.c 
	$(CXX) -c -o $@ $< $(CXXFLAGS)

extractor: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f *.o *_EXTRACTED.CSV extractor
