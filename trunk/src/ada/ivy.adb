package body Ivy is


Type Dummy is null record;
Type Proc_Ptr is access procedure;

--------------------------- routines de Ivy-c ------------------------------
procedure C_IvyInit (Appname     : C.Strings.chars_ptr;
                     Ready       : C.Strings.chars_ptr;
                     AppCallback : AppCallback_T;
                     AppData     : UserData_T;
                     DieCallback : DieCallback_T;
                     DieData     : UserData_T);
pragma Import (C, C_IvyInit, "IvyInit");

procedure C_IvyStart ( Bus : C.Strings.chars_ptr );
pragma Import (C, C_IvyStart, "IvyStart");

function C_IvyBindMsg (MsgCallback : MsgCallback_T; 
                              Data : UserData_T; 
                              Mess : C.char_array) 
                              return Subscription_Id_T;
pragma Import (C, C_IvyBindMsg, "IvyBindMsg");

procedure C_IvyBindMsg (MsgCallback : MsgCallback_T; 
                              Data : UserData_T; 
                              Mess : C.char_array) is

Dummy : Subscription_Id_T;
begin
     Dummy:=  C_IvyBindMsg (MsgCallback, Data, Mess);
end C_IvyBindMsg;
-- pragma Import (C, C_IvyBindMsg, "IvyBindMsg"); Non autorise en version 3.12
-- de gnat (import multiple avec profils differents !?)

-- Il faudrait traiter va_list ....
-- ou utiliser _BindMsg

function  C_IvySendMsg (Format : C.char_array := C.To_C("%s");
                        Mess : C.char_array) return C.int;
pragma Import (C, C_IvySendMsg, "IvySendMsg");

procedure  C_IvySendMsg (Format : C.char_array := C.To_C("%s");
                         Mess : C.char_array) is
Dummy :  C.int;
begin
     Dummy:=C_IvySendMsg (Format, Mess);
end  C_IvySendMsg;
-- pragma Import (C, C_IvySendMsg, "IvySendMsg"); Non autorise en version 3.12
-- de gnat (import multiple avec profils differents !?)

procedure C_IvySendDieMsg ( app : AppClientPtr_T);
pragma Import (C, C_IvySendDieMsg,"IvySendDieMsg");

procedure C_IvyUnbindMsg( Id : Subscription_Id_T);
pragma import (C, C_IvyUnbindMsg,"IvyUnbindMsg");

procedure C_IvyMainLoop ( Hook : Proc_Ptr := null); -- Hook inutile en Ada
pragma Import (C, C_IvyMainLoop, "IvyMainLoop");

-------------------------------------------------------------------------

-- Objet protege pour garantir l'exclusion sur l'utilisation des
-- routines C (non re-entrantes) fournies par la lib Ivy.
-- Autorise aussi les appels en fonction du contexte:
-- Configure une seule fois, et les autres quand le bus est en 
-- regime de croisiere....

Protected API is
     entry Configure (
                 AppName : String; -- nom de l'application
                 Ready   : String; -- ready Message peut etre NULL
                 Bus     : String := Default_Bus;
                           -- valeur par defaut de la lib Ivy C
                 AppCallback : AppCallback_T := null;
                 AppData     : UserData_T := Null_Data;
                 DieCallback : DieCallback_T := null;
                 DieData     : UserData_T:= Null_Data);

     entry SendMsg (Mess : String; Nb : out Nombre_T);
     entry SendMsg (Mess : String);
     entry SendDieMsg ( app : AppClientPtr_T);
     entry Wait_BusReady;
     entry BindMsg ( MsgCallback : MsgCallback_T;
                     user_data   : UserData_T;
                     regexp      : String;
                     Id          : out  Subscription_Id_T);
     entry BindMsg ( MsgCallback : MsgCallback_T;
                     user_data   : UserData_T;
                     regexp      : String);
     entry UnbindMsg ( Id : Subscription_Id_T);
private
     BusReady : Boolean :=False;
end API;     

protected body API is

entry  Configure(
                 AppName : String; -- nom de l'application
                 Ready   : String; -- ready Message peut etre NULL
                 Bus     : String := Default_Bus;
                                     -- valeur par defaut de la lib Ivy C
                 AppCallback : AppCallback_T :=null;
                 AppData    : UserData_T := Null_Data;
                 DieCallback: DieCallback_T := null;
                 DieData    : UserData_T:= Null_Data) 
              when not(BusReady) is
begin
      C_IvyInit (Appname     => C.Strings.New_String(AppName),
                 Ready       => C.Strings.New_String(Ready),
                 AppCallback => AppCallback,
                 AppData     => AppData, 
                 DieCallback => DieCallback,
                 DieData     => DieData);
      C_IvyStart(C.Strings.New_String(Bus));
      BusReady:= True;
end configure;

entry SendMsg (Mess : String; Nb : out Nombre_T) when BusReady is
begin
     Nb:=Nombre_T(C_IvySendMsg( Mess => C.To_C(Mess)));
end SendMsg;

entry SendMsg (Mess : String) when BusReady is
begin
     C_IvySendMsg( Mess => C.To_C(Mess));
end SendMsg;

entry SendDieMsg ( app : AppClientPtr_T) when BusReady is
begin
     C_IvySendDieMsg(app);
end SendDieMsg;

entry BindMsg ( MsgCallback : MsgCallback_T;
                User_data : UserData_T;
                Regexp : String;
                Id : out  Subscription_Id_T) when  BusReady is
begin
     Id:=C_IvyBindMsg( MsgCallback,  user_data, C.To_C(regexp));
end BindMsg;

entry BindMsg ( MsgCallback : MsgCallback_T;
                user_data : UserData_T;
                Regexp : String) when  BusReady is
begin
     C_IvyBindMsg( MsgCallback,  user_data, C.To_C(regexp));
end BindMsg;

entry UnbindMsg ( Id : Subscription_Id_T) when BusReady is
begin
     C_IvyUnbindMsg(Id);
end UnbindMsg;

entry Wait_BusReady when BusReady is
begin
     null;
end Wait_BusReady;

end API;

-----------------------------------------------------------
-- Tache qui encapsule la mainloop C (qui ne retourne jamais)

Task Ivy_Run;
Task body Ivy_Run is
begin
     API.Wait_BusReady;
     C_IvyMainloop;
end Ivy_Run;

-------------------  Configure  ------------------------

procedure Configure (
                      AppName     : String; -- nom de l'application
                      Ready       : String; -- ready Message peut etre NULL
                      Bus         : String := Default_Bus;
                      AppCallback : AppCallback_T := null;
                      AppData     : UserData_T := Null_Data;
                      DieCallback : DieCallback_T := null;
                      DieData     : UserData_T:= Null_Data) is
begin
     API.Configure(AppName, Ready, Bus, AppCallback, AppData,
                   DieCallback, DieData);
end Configure;

-------------------  SendMsg  --------------------------

function SendMsg (Mess : String) return Nombre_T is

Nb : Nombre_T;
begin
     API.SendMsg(Mess,Nb);
     return Nb;
end SendMsg;

procedure SendMsg (Mess : String) is
Dummy : Nombre_T;
begin
     API.SendMsg(Mess,Dummy);
end SendMsg;


------------------  SendDieMsg  -------------------------

procedure SendDieMsg ( app : AppClientPtr_T) is
begin
     API.SendDieMsg( App);
end SendDieMsg; 

------------------  BindMsg  ----------------------------

function BindMsg ( MsgCallback : MsgCallback_T;
                   user_data : UserData_T;
                   regexp : String) return Subscription_Id_T is
Id : Subscription_Id_T;
begin
     API.BindMsg(MsgCallback, user_data, regexp, Id);
     return Id;
end BindMsg;  

procedure BindMsg ( MsgCallback : MsgCallback_T;
                    user_data : UserData_T;
                    regexp : String) is
Dummy : Subscription_Id_T;
begin
     API.BindMsg(MsgCallback, user_data, regexp, Dummy);
end BindMsg;

-------------------  UnbindMsg  ----------------------------

procedure UnbindMsg ( Id : Subscription_Id_T) is
begin
     API. UnbindMsg ( Id);
end UnbindMsg;

-------------------- Kill -----------------------------------

procedure Kill is
begin
   -- Massacre sauvage de la main loop!
   abort Ivy_Run;
end Kill;
end ivy;
