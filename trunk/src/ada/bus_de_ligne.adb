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
		entry setSpeed ( change : in integer );
		entry init(	bus_id : in integer;
				nb_passengers : in integer;
				bus_line_id : in integer;
				bus_line : in circuit;
				nb_road : in integer);
		entry position(	nb : in integer);
		entry getPosition 
			  (	id : out integer;
				line : out integer;
				part : out integer;
				pos : out integer; 
				capacity : out integer;
				speed : out integer);
		entry setNextStop ( 
				Station_Id : in integer;
				Station_pos : in integer);
		--entry sendPosition();
-- 		entry getSpeed(cs : out natural);
-- 		entry setSituation(situation : in boolean);
end BUS;

--BUS
task body BUS is

--Caracteristiques du bus
vitesse : integer := 0;
sens : integer := 1;

id_bus : integer;
passager : integer;
ligne : integer;
itineraire : circuit;

--Position du bus
nbCaseParcouru : integer := 0;
portion : integer;
nbCaseAParcourir :integer := 0;
cpt : integer :=0;	

nb_roads : integer;
road_stop : boolean := false;
nextStationId : integer;
nextStationPos : integer;

noStop : boolean := TRUE;



begin

	--Demarrage du bus
	put_line("Demarrage du bus");						

	delay(0.1);

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
					bus_line : in circuit;
					nb_road : in integer) do

				id_bus := bus_id;
				passager := nb_passengers;
				ligne := bus_line_id;
				itineraire := bus_line;
				nb_roads := nb_road;
				situation := 0;
				estInitialise := TRUE;
				put_line("INNNNNIIIITIALLLLLISEEEE");
			end init;
		or
			accept getPosition (	id : out integer;
						line : out integer;
						part : out integer;
						pos : out integer; 
						capacity : out integer;
						speed : out integer) do
				id := id_bus;
				line := ligne;
				part := portion;
				pos := nbCaseParcouru; 
				capacity := passager;
				speed := vitesse;
			end getPosition;
		or
			accept setSpeed ( change : in integer ) do
				put_line("Speed Control");
				if (change > 0)
				then
					for I in 1..change
					loop
						if (vitesse+1 > 14)--50km/h ~ 14m/S
						then
							exit;
						else
						vitesse := vitesse + 1;
						put("Nouvelle Vitesse: ");
						put_line(integer'image(vitesse));
						end if;
					end loop;
				elsif (change < 0)
				then
					for I in 1..-change
					loop
						if (vitesse-1 < 0)
						then
							exit;
						else
							vitesse := vitesse - 1;
							put("Nouvelle Vitesse: ");
							put_line(integer'image(vitesse));
						end if;
					end loop;
				end if;
			end setSpeed;
		or
			--arret1.position(nbCaseAParcourir-nbCaseParcouru,vitesse);
			--le bus avance
			delay (1.0);

			if (estInitialise)
			then
	
				if (cpt <= nb_roads)
				then
					if (road_stop = false)
					then
						if ((nbCaseParcouru + vitesse) = nbCaseAParcourir)
						then	
							put_line("Bus en fin de route");
							cpt := cpt + 1;

							nbCaseParcouru := 0;
							portion := itineraire(cpt).num;
							nbCaseAParcourir := itineraire(cpt).length;
	
							Ivy.SendMsg("Bus id="&integer'image(id_bus)
							&" line="&integer'image(ligne)
							&" pos="&integer'image(portion)
							&","&integer'image(nbCaseParcouru)
							&" capacity="&integer'image(passager)
							&" speed="&integer'image(vitesse)
							&" status="&integer'image(situation));
	
							select
								when noStop =>
								accept setNextStop ( 
									Station_Id : in integer;
									Station_pos : in integer)do
								nextStationId := Station_Id;
								nextStationPos := Station_pos;
								road_stop := TRUE;
								noStop := FALSE;
								end setNextStop;
							or
								delay(0.2);
							end select;
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
							
 							if (vitesse < (nbCaseAParcourir - nbCaseParcouru) and then vitesse < 14)--50km/h ~ 14m/S
							then
								vitesse := vitesse + 1 * sens;
								put("Nouvelle Vitesse: ");
								put_line(integer'image(vitesse));
								
							elsif (vitesse > (nbCaseAParcourir - nbCaseParcouru))
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
						if (nbCaseParcouru = nextStationPos)
						then
							while(vitesse > 0)
							loop
								vitesse := vitesse -1;
								put("Nouvelle Vitesse: ");
								put_line(integer'image(vitesse));
							end loop;
							
							situation := 1;

							put("Bus arrivee a la station");
							put_line(integer'image(nextStationId));

							Ivy.SendMsg("Bus id="&integer'image(id_bus)
							&" line="&integer'image(ligne)
							&" pos="&integer'image(portion)
							&","&integer'image(nbCaseParcouru)
							&" capacity="&integer'image(passager)
							&" speed="&integer'image(vitesse)
							&" status="&integer'image(situation));
							road_stop := FALSE;
							noStop := TRUE;

							situation := 0;

							
						else
			
							--Affichage
							put("Ancienne valeur de nbCaseParcouru: ");
							put(integer'image(nbCaseParcouru));
							put("/");
							put_line(integer'image(nextStationPos));
							
							nbCaseParcouru := nbCaseParcouru + vitesse;
							
							--Affichage
							put("Nouvelle valeur de nbCaseParcouru: ");
							put(integer'image(nbCaseParcouru));
							put("/");
							put_line(integer'image(nextStationPos));
							
							
							put("Ancienne Vitesse: ");
							put_line(integer'image(vitesse));
							
							if (vitesse < (nextStationPos - nbCaseParcouru) and then vitesse < 14)--50km/h ~ 14m/S
							then
								vitesse := vitesse + 1 * sens;
								put("Nouvelle Vitesse: ");
								put_line(integer'image(vitesse));
								
							elsif (vitesse >= (nextStationPos - nbCaseParcouru))
								then
									while(vitesse > (nextStationPos - nbCaseParcouru))
									loop
									vitesse := vitesse -1;
									
									put("Nouvelle Vitesse: ");
									put_line(integer'image(vitesse));
									
									end loop;
							end if;
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
		bus_line : in circuit;
		nb_road : in integer) is

begin

	BUS.init(bus_id, nb_passengers, bus_line_id ,bus_line,nb_road);
end init;

procedure nextStop ( 	nextStationId : in integer;
			pos : in integer) is

begin
	BUS.setNextStop(nextStationId,pos);
end nextStop;

procedure Speed (	change : in integer)is
begin
	BUS.setSpeed(change);
end Speed;


procedure start is

package Random_Id is new Ada.Numerics.Discrete_Random (natural);
use Random_Id;

--Generateur d'id
G : Generator;

DataBus : Unbounded_String := To_Unbounded_String(Ivy.Default_Bus);
IvyBus : GNAT.OS_Lib.String_Access;

bus_num : integer;
line : integer;
part : integer;
pos : integer; 
capacity : integer;
speed : integer;

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

-- 	Station id= 5 bus_id= 1 len= 100"
	while (not estInitialise)
	loop
		null;
	end loop;

	BUS.getposition(bus_num,line,part,pos,capacity,speed);
	put_line(integer'image(bus_num));
	put_line(To_String(To_Unbounded_String("^Station id= ([0-9]+) bus_id="& integer'image(bus_num) &" len= ([0-9]+)")));
	Ivy.BindMsg( 	MsgCallback => Bus_Cb.nextStop'access,
			User_Data        => 0,
			Regexp      => To_String(To_Unbounded_String("^Station id= ([0-9]+) bus_id="& integer'image(bus_num) &" len= ([0-9]+)"))
	);

	delay(0.1);
-- 	Bus id= 1 speed=+20
	Ivy.BindMsg( 	MsgCallback => Bus_Cb.speed'access,
			User_Data        => 0,
			Regexp      => To_String(To_Unbounded_String("^Bus id="& integer'image(bus_num) &" speed=([+|-][0-9]+)"))
		);

	while (true)
	loop
		delay(3.0);
		BUS.getposition(bus_num,line,part,pos,capacity,speed);
		if (part /= 0 and then pos /= 0)
		then
		Ivy.SendMsg("Bus id="&integer'image(bus_num)&" line="&integer'image(line)&" pos="&integer'image(part)&","&integer'image(pos)&" capacity="&integer'image(capacity)&" speed="&integer'image(speed)&" status="&integer'image(situation));
		end if;
	end loop;

end start;
end bus_de_ligne;