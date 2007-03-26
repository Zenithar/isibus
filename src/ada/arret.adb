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
		ENTRY setLignes ( liste : listeBus);
	
		function calc_time return boolean;
		function isInit return boolean;
		function hasThis(line : in integer) return boolean;
		function getRoad return integer;
		function getLen return integer;
		function getStationId return integer;
		function getLignes return listeBus;
		function getListe_attente return bus_attendu;
	
		procedure maj;
		procedure setListe_attente (liste : in bus_attendu);

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
		arret_signale : integer := 0;
	
	end bus_stop;
	
	-- CORPS OBJET PROTEGE: arret de bus
	protected body bus_stop is

		procedure setListe_attente (liste : in bus_attendu) is
		begin
			liste_attente := liste;
		end setListe_attente;

		function getListe_attente return bus_attendu is
		begin
			return liste_attente;
		end getListe_attente;

		function isInit return boolean is 
		begin
			return isInitialized;
		end isInit;

		function hasThis(line : in  integer) return boolean is
		result : boolean := FALSE;
		begin
			for I in 1..bus'LENGTH loop
				if (bus(I).num = line)
				then
					result := TRUE;
				end if;
			end loop;
			return result;
		end hasThis;
		
		function getRoad return integer is
		begin
			return road;
		end getRoad;

		function getLen return integer is
		begin
			return len;
		end getLen;

		function getStationId return integer is
		begin
			return Station_id;
		end getStationId;

		function getLignes return listeBus is
		begin
			return bus;
		end getLignes;

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
			if (liste_attente(i).speed /= 0)
			then
				put(" arrive dans: ");
				--Calcul du temps
				timeleft := liste_attente(i).dist / liste_attente(i).speed;
				put(integer'image(timeleft));
				put_line(" secondes");
			else
				put_line("est arrete");
			end if;
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

			put("Position");
			put(integer'image(temp.bus_id));
			put(integer'image(temp.dist));
			put_line(integer'image(temp.speed));

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

		ENTRY setLignes ( liste : listeBus) when TRUE is
		begin
			bus := liste;
			put_line("Mise a jour setLignes ( liste : listeBus)");
		end setLignes;

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
	
-- 		Station_([0-9]+) id=5 road=3 lines=1,2 len=100
		delay(0.1);
		Ivy.BindMsg( MsgCallback => Arret_Cb.init'access,
		User_Data        => 0,
		Regexp      => To_String(To_Unbounded_String("^" &  Id_station &	 natural'image(num_station)(2..natural'image(num_station)'LENGTH) & " id=([0-9]+) road=([0-9]+) lines=([0-9]+(,[0-9]+)*) len=([0-9]+)"))
		);

-- 		Bus id=([0-9]+) line=([0-9]+) pos=([0-9]+),([-]?[0-9]+) capacity=([0-9]+) speed=([0-9]+)
		delay(0.1);
		Ivy.BindMsg( MsgCallback => Arret_Cb.position'access,
		User_Data        => 0,
		Regexp      => To_String(To_Unbounded_String("^Bus id= ([0-9]+) line= ([0-9]+) pos= ([0-9]+), ([-]?[0-9]+) capacity= ([0-9]+) speed= ([0-9]+)"))
		);

		delay(0.1);
 		Ivy.BindMsg( 	MsgCallback => Arret_Cb.attente'access,
			User_Data        => 0,
			Regexp      => To_String(To_Unbounded_String("^Bus_([0-9]+) id=([0-9]+) passengers=([0-9]+) line=([0-9]):(([0-9]+,[0-9]+;)*)"))
		   );

		while(true)
		loop

			if (bus_stop.isInit)
			then
				--Tests
				while(TRUE)
				loop
				delay(1.0);
				bus_stop.maj;
				b := bus_stop.calc_time;
				end loop;
				

			else
				delay(1.0);
				put_line("Attente d'initialisation...");
			end if;

		end loop;
	
	end start;

	procedure init (Station_id : in integer ;
		road : in integer;
		bus : in listeBus;
		len : in integer) is
	begin
		bus_stop.init(Station_id,road,bus,len);
	end init;

	function hasThis (line : integer) return boolean is
	begin
		return bus_stop.hasThis(line);
	end hasThis;

	function getRoad return integer is
	begin
		return bus_stop.getRoad;
	end getRoad;

	function getLignes return listeBus is
	begin
		return bus_stop.getLignes;
	end getLignes;

	procedure setLignesCircuit (liste : in listeBus) is
	begin
		bus_stop.setLignes(liste);
	end setLignesCircuit;

	procedure storeInformations(	id : in integer ;
				line : in integer ;
				cur_road : in integer;
				cur_pos : in integer;
				cur_capacity : in integer;
				cur_speed : in integer ) is
	cur_line : ligne;
	lignes_passant: listeBus;
	bus_stop_road : integer;	

	dist : integer := 0;
	liste_bus : bus_attendu;
	cpt : integer := 1;
	ptr : integer;


	begin
		put_line("DEBUT");
-- 		liste_bus := bus_stop.getListe_attente;
-- 		for I in 0..liste_bus'LENGTH-1
-- 		loop
-- 			if (liste_bus(I).bus_id = id)
-- 			then
-- 				ptr := I;
-- 			end if;
-- 		end loop;
-- 		put_line("DEBUT");
-- 		if (not liste_bus(ptr).stop_signaled)
-- 		then
-- 			put_line("FALSE");
-- 		end if;
-- 		put_line("DEBUT");
		
			if ((cur_road = arret.getRoad) and then (cur_pos = 0))
			then
-- 				if (liste_bus(ptr).stop_signaled)
-- 				then
-- 					null;
-- 				else
-- 					liste_bus(ptr).stop_signaled := TRUE;
-- 					if (liste_bus(ptr).stop_signaled)
-- 					then
-- 						put_line("TRUE");
-- 					end if;
-- 					put_line(boolean'image(liste_bus(ptr).stop_signaled));
-- 					bus_stop.setListe_attente(liste_bus);
-- 					put_line("SET");
		-- 			Station id=([0-9]+) bus_id=([0-9]+) len=([0-9]+)
					Ivy.SendMsg(	"Station id="&natural'image(bus_stop.getStationId)
							&" bus_id="&integer'image(id)
							&" len="&integer'image(bus_stop.getLen - cur_pos));
				--end if;
			end if;
-- 			PB HERE

			lignes_passant := getLignes;



			for I in 1..lignes_passant'LENGTH
			loop
				if (lignes_passant(I).num = line)
				then
					cur_line := lignes_passant(I);
				end if;
			end loop;

			while((cpt <= cur_line.prog'LENGTH) and then (cur_line.prog(cpt).num /= cur_road))
			loop
				cpt := cpt + 1;
			end loop;

			bus_stop_road := getRoad;

			dist := dist - cur_pos;
			while((cpt <= cur_line.prog'LENGTH) and then (cur_line.prog(cpt).num /= bus_stop_road))
			loop
				dist := dist + cur_line.prog(cpt).length;
				cpt := cpt + 1;
			end loop;

			dist := dist + bus_stop.getLen;
			
-- 			put("HEEEY ");
-- 			put(integer'image(dist));
-- 			put_line(integer'image(cur_speed));

 			bus_stop.position(id , dist , cur_speed);
	end storeInformations;

	procedure storeBus (id : in natural ;
			distance : in integer ;
			cs : in integer) is
	begin
		bus_stop.position(id , distance , cs);
	end storeBus;


end arret;