with Ivy; use Ivy;
with Bus_Cb; use Bus_Cb;

with Ada.Numerics.Discrete_Random;


with GNAT.Command_Line; use GNAT.Command_Line;
-- with Ada.Strings.Unbounded.Lists; use Ada.Strings.Unbounded.Lists;

with GNAT.OS_Lib;

with Text_io;
use Text_io;
with Ada.Calendar;
use Ada.Calendar;

package body bus_de_ligne is

task BUS is
		entry speedUp;
		entry speedDown;
		entry init(	bus_id : in integer;
				nb_passengers : in integer;
				bus_line_id : in integer;
				bus_line : in circuit);
		entry position(	nb : in integer);
		entry getPosition 
			  (	id : out integer;
				line : out integer;
				part : out integer;
				pos : out integer; 
				capacity : out integer);
		--entry sendPosition();
		entry getSpeed(cs : out natural);
		entry setSituation(situation : in boolean);
end BUS;

--BUS
task body BUS is

--Caracteristiques du bus
vitesse : integer := 0;
id_bus : integer;
passager : integer;
ligne : integer;
itineraire : circuit;

estInitialise : boolean := FALSE;

--Position du bus
nbCaseParcouru : integer := 0;
portion : integer;
nbCaseAParcourir :integer := 0;
cpt : integer :=0;	

--Historique du bus
lastStation : integer;
nextStation : integer;

--Incidents
situation : boolean := TRUE;

begin

	--Demarrage du bus
	put_line("Demarrage du bus");						

	while(true)
	loop

		select
			accept position(nb : in integer ) do
				nbCaseParcouru := nb;
			end;
			put_line("J'ai Attttttttrapppppppéééééé la position!!!!");
		or
			accept init(	bus_id : in integer;
					nb_passengers : in integer;
					bus_line_id : in integer;
					bus_line : in circuit) do

				id_bus := bus_id;
				passager := nb_passengers;
				ligne := bus_line_id;
				itineraire := bus_line;

				estInitialise := TRUE;
				put_line("INNNNNIIIITIALLLLLISEEEE");
			end init;
		or
			accept getPosition (	id : out integer;
						line : out integer;
						part : out integer;
						pos : out integer; 
						capacity : out integer) do
				id := id_bus;
				line := ligne;
				part := portion;
				pos := nbCaseParcouru; 
				capacity := passager;
			end getPosition;
		or
			--arret1.position(nbCaseAParcourir-nbCaseParcouru,vitesse);
			--le bus avance
			delay (1.0);

			if (estInitialise)
			then
	
				if (cpt < itineraire'length)
				then
					if (nbCaseParcouru = nbCaseAParcourir)
					then
						while(vitesse > 0)
						loop
							vitesse := vitesse -1;
							put("Nouvelle Vitesse: ");
							put_line(integer'image(vitesse));
						end loop;
						
						put_line("Bus en fin de route");

						cpt := cpt + 1;
						nbCaseParcouru := 0;
						portion := itineraire(cpt).num;
						nbCaseAParcourir := itineraire(cpt).length;
					else
		
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
						put_line(integer'image(vitesse));
						
						if (vitesse < (nbCaseAParcourir - nbCaseParcouru) and then vitesse < 50)
						then
							vitesse := vitesse + 5;
							put("Nouvelle Vitesse: ");
							put_line(integer'image(vitesse));
							
						elsif (vitesse >= (nbCaseAParcourir - nbCaseParcouru))
							then
								while(vitesse > (nbCaseAParcourir - nbCaseParcouru))
								loop
								vitesse := vitesse -1;
								
								put("Nouvelle Vitesse: ");
								put_line(integer'image(vitesse));
								
								end loop;
						end if;
					end if;
				else
					put_line("Le Bus a fini son itinéraire");
					estInitialise := FALSE;
					cpt := 0;
				end if;
			else 
				put_line("Le Bus attends ses instructions");
			end if;
			

		end select;
	end loop;
	

	
end BUS;


procedure position(nb : in integer) is
begin	
	BUS.position(nb);

end position;


procedure init(	bus_id : in integer;
		nb_passengers : in integer;
		bus_line_id : in integer;
		bus_line : in circuit) is


-- 	bus_id : integer;
-- 	nb_passengers : integer;
-- 	bus_line_id : integer;

-- 	id=1 passengers=50 line=2:1,100;2,200;3,300;4,100;

begin

	BUS.init(bus_id, nb_passengers, bus_line_id ,bus_line);
end init;



procedure start is

package Random_Id is new Ada.Numerics.Discrete_Random (natural);
use Random_Id;

--Generateur d'id
G : Generator;

--Id du bus
Id: String := "Bus_";
num_bus : natural; 

DataBus : Unbounded_String := To_Unbounded_String(Ivy.Default_Bus);
IvyBus : GNAT.OS_Lib.String_Access;

bus_num : integer;
bus_line : integer;
part : integer;
cur_pos : integer; 
capacity : integer;


--BODY
begin
	--Demarre le generateur
	Random_Id.Reset (G);

	--Genere un nombre au hazard
	num_bus := Random_Id.Random(G);

	put_line(Id);
	put_line(natural'image(num_bus));

	--Affichage de l'identifiant du bus	
	put("RAAAAANNNNNNNDOOOOMMMMM -> ");
	put_line(Id & natural'image(num_bus)(2..natural'image(num_bus)'LENGTH));

	IvyBus:= GNAT.OS_Lib.Getenv("IVYBUS");
	if IvyBus.all'length /= 0 then  --| La variable existe
	DataBus:= To_Unbounded_string(IvyBus.all);
	end if;

	--Identification sur IVY
	Ivy.Configure( AppName => Id & natural'image(num_bus)(2..natural'image(num_bus)'LENGTH),
		Ready   => "Connected",
		Bus     => To_String(DataBus));

	--Catch message
	delay(0.1);
	Ivy.BindMsg( 	MsgCallback => Bus_Cb.position'access,
			User_Data        => 0,
			Regexp      => To_String(To_Unbounded_String("^" &  Id &	 natural'image(num_bus)(2..natural'image(num_bus)'LENGTH) & " position=(.*)"))
		   );

	delay(0.1);
	Ivy.BindMsg( 	MsgCallback => Bus_Cb.init'access,
			User_Data        => 0,
			Regexp      => To_String(To_Unbounded_String("^" &  Id &	 natural'image(num_bus)(2..natural'image(num_bus)'LENGTH) & " id=([0-9]+) passengers=([0-9]+) line=([0-9]):(([0-9]+,[0-9]+;)*)"))
		   );

	--Contacte le CC pour l'informer du départ du bus
	delay(0.1);
	Ivy.SendMsg(Id & natural'image(num_bus)(2..natural'image(num_bus)'LENGTH) & " Start");

	while (true)
	loop
		delay(3.0);
		BUS.getposition(bus_num,bus_line,part,cur_pos,capacity);
		if (part /= 0 and then cur_pos /= 0)
		then
			Ivy.SendMsg(Id & natural'image(num_bus)(2..natural'image(num_bus)'LENGTH) & " id=" & integer'image(bus_num) & " line=" & integer'image(bus_line) & " pos=" & integer'image(part) & "," & integer'image(cur_pos) & " capacity=" & integer'image(capacity));
		end if;
	end loop;

end start;
end bus_de_ligne;