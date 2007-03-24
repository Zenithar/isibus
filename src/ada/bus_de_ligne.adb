with Ivy; use Ivy;
with Bus_Cb; use Bus_Cb;

with Ada.Numerics.Discrete_Random;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;

with GNAT.Command_Line; use GNAT.Command_Line;
with GNAT.OS_Lib;

with Text_io;
use Text_io;
with Ada.Calendar;
use Ada.Calendar;

package body bus_de_ligne is

task BUS is
		entry speedUp;
		entry speedDown;
		--entry setLigne(	app : AppClientPtr_T; 
		--		user_data : UserData_T; 
		--		argc:C_Int_T;
		--		argv:C_Char_Etoile_Etoile_T
		--);
		--entry setArret(ar : in arret);
		entry position(	nb : in integer);
		--entry sendPosition();
		--entry getLastStation(num : out integer);
		--entry getNextStation(num : out integer);
		entry getSpeed(cs : out natural);
		entry setSituation(situation : in boolean);
end BUS;

--BUS
task body BUS is

--Caracteristiques du bus
vitesse : natural := 0;
id_bus : natural;
ligne : natural;

--Position du bus
nbCaseParcouru : integer :=0;
nbCaseAParcourir :integer := 600;
	

--Historique du bus
lastStation : integer;
nextStation : integer;

--Incidents
situation : boolean := TRUE;

begin

	--Demarrage du bus
	put_line("Demarrage du bus");

	while(nbCaseParcouru < nbCaseAParcourir)
	loop							

		select
			accept position(nb : in integer ) do
				nbCaseParcouru := nb;
				put_line("J'ai Attttttttrapppppppéééééé la position!!!!");
			end; 
		else

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
		end select;

		

	end loop;
	
	while(vitesse > 0)
	loop
		vitesse := vitesse -1;
		put("Nouvelle Vitesse: ");
		put_line(natural'image(vitesse));
	end loop;
	
	put_line("Bus arrivee a destination");
	
end BUS;

procedure position(nb : in integer) is
begin	
	BUS.position(nb);

end position;

procedure start is

package Random_Id is new Ada.Numerics.Discrete_Random (natural);
use Random_Id;


--Declarations
maxStation : constant integer := 7;
type carte is array(1..3,1..maxStation) of integer;

type busdeligne is record
	num : natural;
end record;

i : integer;
--arret1 : arret;

--Generateur d'id
G : Generator;

--Id du bus
Id: String := "Bus_";
num_bus : natural; 

DataBus : Unbounded_String := To_Unbounded_String(Ivy.Default_Bus);
IvyBus : GNAT.OS_Lib.String_Access;


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

	--Contacte le CC pour l'informer du départ du bus
	delay(0.1);
	Ivy.SendMsg(Id & natural'image(num_bus)(2..natural'image(num_bus)'LENGTH) & " Start");

	while (true)
	loop
		delay(3.0);
		Ivy.SendMsg(Id & natural'image(num_bus)(2..natural'image(num_bus)'LENGTH) & " Position !!!");
	end loop;

end start;
end bus_de_ligne;