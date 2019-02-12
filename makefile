CC=cc
CXX=gcc
RM=rm -f
LDLIBS=$(zsh root-config --libs) -lncurses

SRCS=main.c
OBJS=$(subst .cc,.o,$(SRCS))

all: lazydrafting

lazydrafting: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LDLIBS) -o lazydrafting

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) -rf *.dSYM lazydrafting

distclean: clean
	$(RM) *~ .depend

include .depend
