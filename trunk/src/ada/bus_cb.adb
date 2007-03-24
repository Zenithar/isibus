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
	temp : string := Value(Tab_arg.all);	

	begin

	for i in 1..temp'last loop
		nbCaseParcouru := nbCaseParcouru*10 + character'pos(temp(i)) - character'pos('0');
	end loop;
	
	bus_de_ligne.position(nbCaseParcouru);

end position;

end Bus_Cb;