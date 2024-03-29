with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;

package bus_de_ligne is

--Nb max d'arret sur une ligne
maxStation : constant integer := 7;

--Id du bus
Id: String := "Bus_";
num_bus : natural;

id_bus : integer;

estInitialise : boolean := FALSE;
--Incidents
situation : integer;

--Declaration du type circuit
type road is record
	num : integer;
	length : integer;
end record;

type circuit is array(1..maxStation) of road;

procedure start;

procedure position(nb : in integer);

procedure init(	bus_id : in integer;
		nb_passengers : in integer;
		bus_line_id : in integer;
		bus_line : in circuit;
		nb_road : in integer);

procedure nextStop ( 	nextStationId : in integer;
			pos : in integer);

procedure Speed (	change : in integer);

procedure emeute;
procedure panne;
procedure bouchon;
procedure ok;

end bus_de_ligne;