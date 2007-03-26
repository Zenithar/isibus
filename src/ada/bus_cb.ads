with Ivy; use Ivy;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;

package Bus_Cb is

procedure position(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T);

procedure init(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T);

procedure nextStop(	app : AppClientPtr_T;
                	user_data : UserData_T;
                	argc : C_Int_T;
                	argv : C_Char_Etoile_Etoile_T);

end Bus_Cb;
