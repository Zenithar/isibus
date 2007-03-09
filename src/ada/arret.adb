with Text_io;
use Text_io;
with Ada.Calendar;
use Ada.Calendar;

procedure arret is			

--DEFINITION DES VARIABLES
MAX_BUS : integer := 4;

--DECLARATION DES TYPES

--type ligne permettant de stocker les bus afin de calculer le temps restant d'attente
type ligne is record
	-- Identificateur du bus
	bus_id : natural;
	-- Distance a laquelle il se trouve par rapport a l arret
	dist : integer;
	-- Vitesse a laquelle il roule
        speed : integer;
end record;

--type bus_aendu permettant de stocker l'ensemble des bus passants par l'arret
type bus_attendu is array(0..MAX_BUS) of ligne;

--FIN DECLARATION DES TYPES


-- DECLARATION OBJET PROTEGE: arret de bus
protected bus_stop is

	ENTRY position	(id : natural ; distance : integer ;cs : integer);
	--ENTRY remove 	(id : natural);

	function calc_time return boolean;	

	procedure maj;

        private
		dist : integer;
		speed : integer;

		liste_attente : bus_attendu;
		nb_bus_passant_par_l_arret : integer := 0;

end bus_stop;

-- CORPS OBJET PROTEGE: arret de bus
protected body bus_stop is
	
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
	
	temp : ligne;
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

end bus_stop;


-- DECLARATION POUR LE CORPS DE LA PROCEDURE
i : integer;
b : boolean;

id : natural := 24;
d : integer := 50;
s : integer := 30;

begin

i :=0;


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

end arret;