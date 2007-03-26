with Ada.Text_Io;
use Ada.Text_Io;

with bus_de_ligne;
use bus_de_ligne;



package body Bus_Cb is

procedure position(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T) is

	nbCaseParcouru : integer := 0;
	Tab_arg :  C_Char_Etoile_Etoile_T :=  argv;	

	begin

	nbCaseParcouru := integer'value(Value(Tab_arg.all));
	
	bus_de_ligne.position(nbCaseParcouru);

end position;

procedure init(		app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T) is

	bus_id : integer;
	nb_passengers : integer;
	bus_line_id : integer;
	bus_line : Unbounded_String;
	
	ligne : circuit;
 	tmp : road;
	buffer : Unbounded_String;
	cpt_road : integer := 1;
	cpt_char : Positive := 1;
	cur_char : Character;

	Tab_arg :  C_Char_Etoile_Etoile_T :=  argv;

	begin	

	--id=1 passengers=50 line=1:1,200;2,300;3,400;5,200;

	--Recupération du bus id
	bus_id := integer'value(Value(Tab_arg.all));
        Chars_ptr_pointers.Increment(Tab_arg);

	--Recuperation de la capacité du bus
	nb_passengers := integer'value(Value(Tab_arg.all));
        Chars_ptr_pointers.Increment(Tab_arg);

	--Recuperation du num de la ligne de bus
	bus_line_id := integer'value(Value(Tab_arg.all));
        Chars_ptr_pointers.Increment(Tab_arg);

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
				ligne(cpt_road) := tmp;
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



-- 	put_line("dsqdsqdqskjdhqsjdhqsjkdhqsjkfhsdjkghjskfghsfjdkghsjdkfghsjkd");

	bus_de_ligne.init(bus_id,nb_passengers,bus_line_id,ligne);

end init;

procedure nextStop(	app : AppClientPtr_T;
		user_data : UserData_T;
		argc : C_Int_T;
		argv : C_Char_Etoile_Etoile_T) is

	nextStationId : integer;
	pos : integer;
	Tab_arg :  C_Char_Etoile_Etoile_T :=  argv;

begin
	nextStationId := integer'value(Value(Tab_arg.all));
        Chars_ptr_pointers.Increment(Tab_arg);

	pos := integer'value(Value(Tab_arg.all));
        Chars_ptr_pointers.Increment(Tab_arg);

	bus_de_ligne.nextStop(nextStationId,pos);

end nextStop;

procedure speed(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T) is
	Tab_arg :  C_Char_Etoile_Etoile_T :=  argv;
begin
	bus_de_ligne.Speed(integer'value(Value(Tab_arg.all)));
end speed;

end Bus_Cb;