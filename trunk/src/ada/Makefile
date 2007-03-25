
TARGETS = lanceur_bus arret

all: commands

commands: $(TARGETS)

lanceur_bus: 
	gnatmake lanceur_bus.adb -largs -livy

arret:
	gnatmake arret.adb -largs -livy

clean:
	-rm -f $(TARGETS) *.o *.ali *~
