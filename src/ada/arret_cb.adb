with Ada.Text_Io;
use Ada.Text_Io;

with arret;
use arret;

package body Arret_Cb is

procedure init(		app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T) is

	Station_id : integer;
	road : integer;
	lines : Unbounded_String;
	len : integer;
	
	buffer : Unbounded_String;
	cpt_bus : integer := 1;
	cpt_char : Positive := 1;
	cur_char : Character;

	Tab_arg :  C_Char_Etoile_Etoile_T :=  argv;

	tmp : ligne;
	bus : listeBus;

	begin	


-- 	 id=3 road=113 lines=4,34,342 len=100
-- 	 id=3 road=113 lines=4,34,342 len=100

	Station_id := integer'value(Value(Tab_arg.all));
        Chars_ptr_pointers.Increment(Tab_arg);

	road := integer'value(Value(Tab_arg.all));
        Chars_ptr_pointers.Increment(Tab_arg);

	--Recuperation des lignes
	lines := To_Unbounded_String(Value(Tab_arg.all));	
	Chars_ptr_pointers.Increment(Tab_arg);

	put_line(To_String(lines));

	while ( (cpt_char <= Length(lines)) and then (cpt_bus <= maxBus) )
	loop
		cur_char := Element(lines,cpt_char);
		if ( cur_char = ',')
		then
			--put_line("Dans le ,");
			tmp.num := integer'value(To_String(buffer));
			put("Line : ");
			put_line(integer'image(tmp.num));
			bus(cpt_bus) := tmp;
			cpt_bus := cpt_bus + 1;

			Delete (buffer,1,Length(buffer));
		else
			buffer := buffer & cur_char;
		end if;
		cpt_char := cpt_char + 1;
	end loop;

	tmp.num := integer'value(To_String(buffer));
	put("Line : ");
	put_line(integer'image(tmp.num));
	bus(cpt_bus) := tmp;

	--PB here!!!
	Chars_ptr_pointers.Increment(Tab_arg);
        --put_line(Value(Tab_arg.all));
 	len := integer'value(Value(Tab_arg.all));

	--put_line(integer'image(len));

--  	put_line("dsqdsqdqskjdhqsjdhqsjkdhqsjkfhsdjkghjskfghsfjdkghsjdkfghsjkd");

 	arret.init(Station_id,road,bus,len);

end init;

procedure position(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T)is

Tab_arg :  C_Char_Etoile_Etoile_T :=  argv;

id : integer;
line : integer;
cur_road : integer;
cur_pos : integer;
cur_capacity : integer;
cur_speed : integer;

begin
-- 	Bus id=([0-9]+) line=([0-9]+) pos=([0-9]+),([-]?[0-9]+) capacity=([0-9]+) speed=([0-9]+)
	
	id := integer'value(Value(Tab_arg.all));
	Chars_ptr_pointers.Increment(Tab_arg);

	line := integer'value(Value(Tab_arg.all));
	Chars_ptr_pointers.Increment(Tab_arg);

	if (arret.hasThis(line))
	then
		cur_road := integer'value(Value(Tab_arg.all));
		Chars_ptr_pointers.Increment(Tab_arg);

		cur_pos := integer'value(Value(Tab_arg.all));
		Chars_ptr_pointers.Increment(Tab_arg);

		cur_capacity := integer'value(Value(Tab_arg.all));
		Chars_ptr_pointers.Increment(Tab_arg);
	
		cur_speed := integer'value(Value(Tab_arg.all));
		Chars_ptr_pointers.Increment(Tab_arg);

-- 		put_line("Avant storeInformations");
		arret.storeInformations(id,line,cur_road,cur_pos,cur_capacity,cur_speed);
	end if;

end position;

procedure attente(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T) is

	bus_line_id : integer;
	bus_line : Unbounded_String;

	lignepassantes : listeBus;
	
	trajet : circuit;
 	tmp : road;
	buffer : Unbounded_String;
	cpt_road : integer := 1;
	cpt_char : Positive := 1;
	cur_char : Character;

	continue : boolean := FALSE;
	ptr: integer;

	Tab_arg :  C_Char_Etoile_Etoile_T :=  argv;

	begin	

	--id=1 passengers=50 line=1:1,200;2,300;3,400;5,200;

	Chars_ptr_pointers.Increment(Tab_arg);

	--Recupération du bus id
        Chars_ptr_pointers.Increment(Tab_arg);

	--Recuperation de la capacité du bus
        Chars_ptr_pointers.Increment(Tab_arg);

	--Recuperation du num de la ligne de bus
	bus_line_id := integer'value(Value(Tab_arg.all));
        Chars_ptr_pointers.Increment(Tab_arg);
	
	lignepassantes:= getLignes;

	for I in 1..lignepassantes'LENGTH
	loop
		if (lignepassantes(I).num = bus_line_id)
		then 
			continue := TRUE; 
			ptr:= I;
		end if;
	end loop;


	if (continue)
	then
		--Recuperation de la ligne
	
		bus_line := To_Unbounded_String(Value(Tab_arg.all));
		put_line(To_String(bus_line));	
	
		while ( (cpt_char <= Length(bus_line)) and then (cpt_road <= maxStation) )
		loop
			cur_char := Element(bus_line,cpt_char);
			if ( cur_char = ',')
			then
				--put_line("Dans le ,");
				tmp.num := integer'value(To_String(buffer));
				put("Road : ");
				put(integer'image(tmp.num));
				put(" - ");
				Delete (buffer,1,Length(buffer));
			elsif ( cur_char = ';')
				then
					--put_line("Dans le ;");
					tmp.length := integer'value(To_String(buffer));
					trajet(cpt_road) := tmp;
					cpt_road := cpt_road + 1;
					put("Length : ");
					put_line(integer'image(tmp.length));
					Delete (buffer,1,Length(buffer));
				else
					buffer := buffer & cur_char;
					--put_line(To_String(buffer));
			end if;
			cpt_char := cpt_char + 1;
		end loop;

		lignepassantes(ptr).prog := trajet;

		arret.setLignesCircuit(lignepassantes);

		
		
	end if;
end attente;

procedure getAttente(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T) is
begin
	arret.getAttente;
end getAttente;


end Arret_Cb;