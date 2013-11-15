JAVA=javac
FLAGS=-Xlint
sources = $(wildcard *.java)
clases = $(sources:.java=.class)

all: $(classes)

clean :
	rm -f *.class

%.java : %.class
	$(JAVA) $(FLAGS) $<
