with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;

package arret is

--Nb max de bus passant par un arret
maxBus : constant integer := 7;
maxStation : constant integer := 7;

--DEFINITION DES VARIABLES
MAX_BUS : integer := 4;

--Id de la station
Id_station: String := "Station_";
num_station : natural;

--DECLARATION DES TYPES

--Declaration du type circuit
type road is record
	num : integer;
	length : integer;
end record;

type circuit is array(1..maxStation) of road;

--type ligne permettant de stocker les bus afin de calculer le temps restant d'attente
type a_bus is record
	-- Identificateur du bus
	bus_id : natural;
	-- Distance a laquelle il se trouve par rapport a l arret
	dist : integer;
	-- Vitesse a laquelle il roule
	speed : integer;
	--
	stop_signaled : boolean := FALSE;
	--
	stat : integer;
end record;
	
--type bus_aendu permettant de stocker l'ensemble des bus passants par l'arret
type bus_attendu is array(0..MAX_BUS) of a_bus;
	
--FIN DECLARATION DES TYPES

--Declaration du type circuit
type ligne is record
	num : integer;
	prog : circuit;
end record;

type listeBus is array(1..maxBus) of ligne;

procedure start;

procedure init (Station_id : in integer ;
		road : in integer;
		bus : in listeBus;
		len : in integer);

function hasThis (line : integer) return boolean;
function getRoad return integer;
function getLignes return listeBus;

procedure storeInformations(	id : in integer ;
				line : in integer ;
				cur_road : in integer;
				cur_pos : in integer;
				cur_capacity : in integer;
				cur_speed : in integer ;
				cur_status : in integer);

procedure storeBus	(id : in natural ;
			distance : in integer ;
			cs : in integer;
			status : in integer);

procedure setLignesCircuit (liste : in listeBus);

procedure getAttente;

end arret;