with Ivy;
with Arret_Cb; use Arret_Cb;

with Ada.Numerics.Discrete_Random;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;

with GNAT.Command_Line; use GNAT.Command_Line;
with GNAT.OS_Lib;

with Text_io;
use Text_io;
with Ada.Calendar;
use Ada.Calendar;

package body arret is


	-- DECLARATION OBJET PROTEGE: arret de bus
	protected bus_stop is
	
		ENTRY position	(id : natural ; distance : integer ;cs : integer);
		ENTRY init (id : integer ; route : integer ; bus_line : listeBus ; pos_len : integer);
		--ENTRY remove 	(id : natural);
	
		function calc_time return boolean;
		function isInit return boolean;
	
		procedure maj;
	
		private

		Station_id : integer;
		road : integer;
		bus : listeBus;
		len : integer;

		isInitialized : boolean := FALSE;

		dist : integer;
		speed : integer;
	
		liste_attente : bus_attendu;
		nb_bus_passant_par_l_arret : integer := 0;
	
	end bus_stop;
	
	-- CORPS OBJET PROTEGE: arret de bus
	protected body bus_stop is
		
		function isInit return boolean is 
		begin
			return isInitialized;
		end isInit;
		
		procedure maj is
	
		i : integer := 0;
		dist_temp : integer;
	
		begin
	
		while ( i < nb_bus_passant_par_l_arret)
		loop
			dist_temp := liste_attente(i).dist - liste_attente(i).speed;
			
			if (dist_temp < 0 )
			then
				dist_temp := 0;
			end if;		
	
			liste_attente(i).dist := dist_temp;
			i := i + 1;
		end loop;
	
		end maj;
	
		function calc_time return boolean is 
		i : integer := 0;
		timeleft : integer;
		begin
		
		while ( i < nb_bus_passant_par_l_arret)
		loop
			put("Bus numero: ");
			put(natural'image(liste_attente(i).bus_id));
			put(" arrive dans: ");
			--Calcul du temps
			timeleft := liste_attente(i).dist / liste_attente(i).speed;
			put(integer'image(timeleft));
			put_line(" secondes");
			--Incrementation compteur de boucle
			i := i + 1;
		end loop;
	
		return true;
		end calc_time;	
	
		--ENTRY remove (id : natural) when nb_bus_passant_par_l_arret /= 0 is
	
		--begin
	
		--end remove;
	
		ENTRY position(id : natural ; distance : integer ;cs : integer) when TRUE is
		
		temp : a_bus;
		flag : integer := 0;	
		cpt : integer := 0;
	
		begin
	
		-- recherche si le bus est deja inscrit
		while ( ( cpt < nb_bus_passant_par_l_arret ) and then ( cpt <= MAX_BUS ) and then (flag = 0))
		loop
			-- si il est inscrit on mémorise son emplacement
			if (liste_attente(cpt).bus_id = id)
			then
			flag := 1;
			end if;
			
			cpt := cpt + 1;
		end loop;
	
		-- si le bus est deja inscrit
		if ( flag = 1 )
		then
			temp.bus_id := id;
			temp.dist := distance;
			temp.speed := cs;
			liste_attente(cpt-1) := temp;
		-- sinon si il n'est pas inscrit mais qu'il reste des place
		elsif ( cpt <= MAX_BUS )
		then
			temp.bus_id := id;
			temp.dist := distance;
			temp.speed := cs;
			liste_attente(nb_bus_passant_par_l_arret) := temp;
			nb_bus_passant_par_l_arret := nb_bus_passant_par_l_arret + 1;	
		-- il ne reste plus de places...
		else
			put_line("Station plaine!!!");
		end if;
	
		end position;

		ENTRY init (	id : in integer;
				route : in integer;
				bus_line : in listeBus;
				pos_len : in integer) when TRUE is
		begin
			
		Station_id := id;
		road := route;
		bus := bus_line;
		len := pos_len;
		
		isInitialized := TRUE;

		end init;	

	end bus_stop;

	procedure start is			
	
	--Def du package utilise pour la generation d'id automatique
	package Random_Id is new Ada.Numerics.Discrete_Random (natural);
	use Random_Id;
	
	-- DECLARATION POUR LE CORPS DE LA PROCEDURE
	b : boolean;
	
	id : natural := 24;
	d : integer := 50;
	s : integer := 30;
	
	--Generateur d'id
	G : Generator;
	
	--Id du bus
	Id_station: String := "Station_";
	num_station : natural;
	
	DataBus : Unbounded_String := To_Unbounded_String(Ivy.Default_Bus);
	IvyBus : GNAT.OS_Lib.String_Access;
	
	begin
	
		--Demarre le generateur
		Random_Id.Reset (G);
	
		--Genere un nombre au hazard
		num_station := Random_Id.Random(G);
	
		put_line(Id_station);
		put_line(natural'image(num_station));
	
		--Affichage de l'identifiant du bus	
		put("RAAAAANNNNNNNDOOOOMMMMM -> ");
		put_line(Id_station & natural'image(num_station)(2..natural'image(num_station)'LENGTH));
	
		IvyBus:= GNAT.OS_Lib.Getenv("IVYBUS");
		if IvyBus.all'length /= 0 then  --| La variable existe
		DataBus := To_Unbounded_string(IvyBus.all);
		end if;
	
		--Identification sur IVY
		Ivy.Configure( AppName => Id_station & natural'image(num_station)(2..natural'image(num_station)'LENGTH),
			Ready   => "Connected",
			Bus     => To_String(DataBus));
	
-- 		Station_([0-9]+) id=([0-9]+) road=([0-9]+) lines=([0-9]+(,[0-9]+)*) len=([0-9]+)
		delay(0.1);
		Ivy.BindMsg( 	MsgCallback => Arret_Cb.init'access,
		User_Data        => 0,
		Regexp      => To_String(To_Unbounded_String("^" &  Id_station &	 natural'image(num_station)(2..natural'image(num_station)'LENGTH) & " id=([0-9]+) road=([0-9]+) lines=([0-9]+(,[0-9]+)*) len=([0-9]+)"))
		);
		

		while(true)
		loop

			if (bus_stop.isInit)
			then
				--Tests
				put_line("");
				
				delay(1.0);
				bus_stop.maj;
				bus_stop.position(id , d , s);
				b := bus_stop.calc_time;
				put_line("");
				
				delay(1.0);
				bus_stop.maj;
				bus_stop.position(1 , 300 , 50);
				b := bus_stop.calc_time;
				put_line("");
				
				delay(1.0);
				bus_stop.maj;
				bus_stop.position(2 , 20 , 6);
				b := bus_stop.calc_time;
				put_line("");
				
				delay(1.0);
				bus_stop.maj;
				bus_stop.position(5 , 33 , 4);
				b := bus_stop.calc_time;
				put_line("");
				
				delay(1.0);
				bus_stop.maj;
				bus_stop.position(7 , 22 , 30);
				b := bus_stop.calc_time;
				put_line("");
				
				delay(1.0);
				bus_stop.maj;
				bus_stop.position(id , 4000 , 300);
				b := bus_stop.calc_time;
				put_line("");
				
				delay(1.0);
				bus_stop.maj;
				bus_stop.position(90 , d , s);
				b := bus_stop.calc_time;
				put_line("");
				
				delay(1.0);
				bus_stop.maj;
				bus_stop.position(id , 3000 , 400);
				b := bus_stop.calc_time;
				put_line("");
				
				delay(1.0);
				bus_stop.maj;
				bus_stop.position(32 , d , s);
				b := bus_stop.calc_time;
				put_line("");
			else
				delay(1.0);
			end if;

		end loop;
	
	end start;

	procedure init (Station_id : in integer ;
		road : in integer;
		bus : in listeBus;
		len : in integer) is
	begin
		bus_stop.init(Station_id,road,bus,len);
	end;

end arret;