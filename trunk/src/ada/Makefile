
TARGETS = bus_de_ligne arret

all: commands

commands: $(TARGETS)

bus_de_ligne: 
	gnatmake bus_de_ligne.adb -largs -livy

arret:
	gnatmake arret.adb -largs -livy

clean:
	-rm -f $(TARGETS) *.o *.ali *~
