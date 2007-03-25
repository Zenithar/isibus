
TARGETS = lanceur_bus lanceur_arret

all: clean commands

commands: $(TARGETS)

lanceur_bus: 
	gnatmake lanceur_bus.adb -largs -livy

lanceur_arret:
	gnatmake lanceur_arret.adb -largs -livy

clean:
	-rm -f $(TARGETS) *.o *.ali *~
