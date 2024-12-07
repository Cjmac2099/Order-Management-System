CXXFLAGS = -g3 -O0 -Werror -pedantic -Wall -std=c++17

ODIR = obj
_OBJ= main.o warehouse.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJ))

HEDR= warehouse.hpp

assignment6: $(OBJS)
	$(CXX) $(CXXFLAGS) -lm -o $@ $^

$(ODIR)/%.o: %.cpp $(HEDR) | $(ODIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(ODIR):
	mkdir -p $@

clean:
	rm -f $(OBJS) assignment6 warehouse.log

zip:
	tar -c -f assignment6.tar main.cpp warehouse.cpp

