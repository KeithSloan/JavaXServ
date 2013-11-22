JAVA=javac
FLAGS=-Xlint -d classes
sources = $(wildcard *.java)
classDir = classes
classList = $(sources:.java=.class)
fullList = $(addprefix $(classDir)/, $(classList))

all: JavaXServ.jar

printVars:
	@echo $(fullList)
 
JavaXServ.jar: $(fullList)
	jar cvfm  JavaXServ.jar  Manifest -C classes .

clean :
	rm -f JavaXServ.jar
	rm -f $(classDir)/*.class

$(classDir)/%.class : %.java
	$(JAVA) $(FLAGS) $<
