
TARGETS = bus_de_ligne arret

all: commands

commands: $(TARGETS)

bus_de_ligne: 
	gnatmake lanceur_bus.adb -largs -livy

arret:
	gnatmake arret.adb -largs -livy

clean:
	-rm -f $(TARGETS) *.o *.ali *~
