CC          := gcc
CFLAGS      := -Wall
LDFLAGS     := -Wall
SOURCES     := $(wildcard *.c)
	# pliki zrodlowe
MAINOBJECTS := $(subst .c,.o,$(shell grep -l main $(SOURCES)))
	# pliki obiektowe, ktore zawieraja definicje funkcji main
ALL         := $(subst .o,,$(MAINOBJECTS))
	# pliki wykonywalne (powstaja ze zrodel zawierajacych definicje main)
DEPENDS     := $(subst .c,.d,$(SOURCES))
	# makefiles dla kazdego z plikow zrodlowych
ALLOBJECTS  := $(subst .c,.o,$(SOURCES))
	# wszystkie pliki obiektowe
OBJECTS	    := $(filter-out $(MAINOBJECTS),$(ALLOBJECTS))
	# pliki obiektowe, ktore nie zawieraja definicji main


all: $(DEPENDS) $(ALL)

$(DEPENDS) : %.d : %.c
	$(CC) -MM $< > $@
	@echo -e: "\t"$(CC) $(CFLAGS) $< >> $@

$(ALL) : % : %.o $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

-include $(DEPENDS)

clean:
	-rm -f *.o $(ALL) $(ALLOBJECTS) $(DEPENDS)