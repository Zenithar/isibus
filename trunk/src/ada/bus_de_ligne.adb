with Text_io;
use Text_io;
with Ada.Calendar;
use Ada.Calendar;


procedure bus_de_ligne is

--Declarations
maxStation : constant integer := 7;
type carte is array(1..3,1..maxStation) of integer;
type busdeligne is record
	num : natural;
end record;

task type BUS (num : natural ; li : natural) is
		entry speedUp;
		entry speedDown;
		--entry setArret(ar : in arret);
		entry position(nb : out integer);
		--entry getLastStation(num : out integer);
		--entry getNextStation(num : out integer);
		entry getSpeed(cs : out natural);
		entry setSituation(situation : in boolean);
end BUS;

--BUS
task body BUS is

	--Caract�istiques du bus
	vitesse : natural := 0;
	id_bus : natural := num;
	ligne : natural := li;

	--Position du bus
	nbCaseParcouru : integer :=0;
	nbCaseAParcourir :integer := 120;
		

	--Historique du bus
	lastStation : integer;
	nextStation : integer;

	--Incidents
	situation : boolean := TRUE;


	begin
		put_line("Demarrage du bus");
		while(nbCaseParcouru < nbCaseAParcourir)
		loop							

				--arret1.position(nbCaseAParcourir-nbCaseParcouru,vitesse);

				--le bus avance
				delay (1.0);

				--Affichage
				put("Ancienne valeur de nbCaseParcouru: ");
				put(integer'image(nbCaseParcouru));
				put("/");
				put_line(integer'image(nbCaseAParcourir));
				
				nbCaseParcouru := nbCaseParcouru + vitesse;
				
				--Affichage
				put("Nouvelle valeur de nbCaseParcouru: ");
				put(integer'image(nbCaseParcouru));
				put("/");
				put_line(integer'image(nbCaseAParcourir));
				
				
				put("Ancienne Vitesse: ");
				put_line(natural'image(vitesse));
				
				if (vitesse < (nbCaseAParcourir - nbCaseParcouru) and then vitesse <= 50)
				then
					vitesse := vitesse + 5;
					put("Nouvelle Vitesse: ");
					put_line(natural'image(vitesse));
					
				else if (vitesse >= (nbCaseAParcourir - nbCaseParcouru))
					then
						while(vitesse > (nbCaseAParcourir - nbCaseParcouru))
						loop
						vitesse := vitesse -1;
						
						put("Nouvelle Vitesse: ");
						put_line(natural'image(vitesse));
						
						end loop;
					end if;
				end if;

		end loop;
		
		while(vitesse > 0)
		loop
			vitesse := vitesse -1;
			put("Nouvelle Vitesse: ");
			put_line(natural'image(vitesse));
		end loop;
		
		put_line("Bus arrivee a destination");
		
		delay (10.0);

	
	end BUS;




i : integer;
bus1 : BUS(1,1);
--arret1 : arret;
plan : carte ;


--BODY
begin
		i := 0;
end bus_de_ligne;