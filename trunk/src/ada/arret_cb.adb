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

		arret.storeInformations(id,line,cur_road,cur_pos,cur_capacity,cur_speed);
	end if;

	

end position;

end Arret_Cb;