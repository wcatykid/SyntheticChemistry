OB_INC=../openbabel-2.3.2/include/
OB_LIB_DIR=../build/src/
OB_LIB=openbabel

IDIR =./
CC=g++
OPT=-O1
CFLAGS= $(OPT) -I$(IDIR) -I$(OB_INC) -L$(OB_LIB_DIR) -l$(OB_LIB)
#
ODIR=./obj
LDIR =../lib 

DEPS = BitGraph.h \
	EdgeAggregator.h \
	EdgeAnnotation.h \
	HyperEdge.h \
	HyperEdgeMultiMap.h \
	HyperGraph.h \
	HyperNode.h \
	Instantiator.h \
	Linker.h \
	Molecule.h \
	PebblerHyperEdge.h \
	PebblerHyperGraph.h \
	PebblerHyperNode.h \
	Rigid.h \
	Utilities.h \
	Atom.h \
	Bond.h

_OBJ = Atom.o \
	BitGraph.o \
	Instantiator.o \
	Linker.o\
	Main.o \
	Molecule.o \
	Rigid.o \
	Utilities.o \
	Atom.o \
	Bond.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

synth: $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core synth.exe synth.exe.stackdump $(INCDIR)/*~
