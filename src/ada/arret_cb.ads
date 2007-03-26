with Ivy; use Ivy;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;

package Arret_Cb is

procedure init(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T);

procedure position(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T);

end Arret_Cb;