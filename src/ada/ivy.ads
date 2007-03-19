
-- Ajouter commentaires sur le binding
-- pas d'exeptions a cause du C

--
-- Portage rapide pour utiliser Ivy en mode minimal en Ada
-- (Interfaces avec les routines C)
-- 

with Interfaces.C;
with Interfaces.C.Strings;
with Interfaces.C.Pointers;


package Ivy is


Default_Bus : constant String := ":2010";

-------------------------------------------------------------------
-- Interface avec C
-------------------------------------------------------------------
package C renames Interfaces.C;

type chars_ptr_array is array(C.size_t range <>)
                           of aliased C.Strings.chars_ptr;

-- Package pour les char ** de C
Package chars_ptr_pointers is new Interfaces.C.Pointers(
                  Index              => C.size_t,
                  Element            => C.Strings.chars_ptr,
                  Element_array      => chars_ptr_array,
                  Default_Terminator => C.Strings.Null_Ptr);

-- les types char * et char ** de C
Type C_Char_Etoile_T is  new C.Strings.chars_ptr;

--Type C_Char_Etoile_Etoile_T is new chars_ptr_pointers.Pointer;
subType C_Char_Etoile_Etoile_T is  chars_ptr_pointers.Pointer;

-- Transformation Chaine de caracteres  C -> String Ada
Function Value ( V :  C.Strings.chars_ptr) return String 
               renames C.Strings.Value;

-------------------------------------------------------------------
--
-- Les Types necessaires pour interfacer avec les routines Ivy C
--
-------------------------------------------------------------------

subtype C_Int_T is C.int;
Subtype Nombre_T is C.int;
Type UserData_T is new Integer;
Null_Data : constant UserData_T := 0;
Type Id_T is new C.int;

Type AppEvent_T is (AppConnected, AppDisconnected);
for  AppEvent_T use (AppConnected    => C.int(0),
                     AppDisconnected => C.int(1));

Type AppClientPtr_T is  private;
Type Subscription_Id_T is private; -- identificateur quand on s'abonne

-- Type de callback appele sur connexion deconnexion d'une appli 
Type AppCallback_T is access procedure (
               app       : AppClientPtr_T;
               user_data : UserData_T;
               event     : AppEvent_T);

-- Type de callback appele sur reception de die 
Type DieCallback_T is access procedure(
               app       : AppClientPtr_T;
               user_data : UserData_T;
               id        : Id_T);

-- Type de callback appele sur reception de messages normaux 
Type MsgCallback_T is access procedure(
               app       : AppClientPtr_T;
               user_data : UserData_T;
               argc      : C_Int_T;
               argv      : C_Char_Etoile_Etoile_T);

-- Type de callback appele sur reception de messages directs 
Type MsgDirectCallback_T is access procedure(
               app       : AppClientPtr_T;
               user_data : UserData_T;
               id        : Id_T;
               msg       : C_Char_Etoile_T);

-- filtrage des regexps 
-- procedure Classes( )  a implementer avec operateur +
--     pour pouvoir ecrire: Classes("avion"+"....."+"....");

--------------------------------------------------------------------------
--
-- Les Procedure et fonctions principales d'Ivy
--
--------------------------------------------------------------------------

-- Procedure de configuration qui appelle IvyInit, IvyStart, 
-- (IvyBindMsg si neccessaire) et IvyMainloop

procedure Configure ( 
                 AppName : String; -- nom de l'application
                 Ready   : String; -- ready Message peut etre NULL
                 Bus     : String := Default_Bus; 
                                     -- valeur par defaut de la lib Ivy C
                 AppCallback : AppCallback_T := null;
                 AppData     : UserData_T := Null_Data;
                 DieCallback : DieCallback_T := null;
                 DieData     : UserData_T:= Null_Data);

function SendMsg (Mess : String) return Nombre_T ; 
-- emission d'un message
-- rend le nombre de messages effectivement envoyes

procedure SendMsg (Mess : String);
-- emission d'un message sans compte-rendu

procedure SendDieMsg ( app : AppClientPtr_T);
-- emission d'un message die pour terminer l'application

-- Abonnement avec une regexp
-- La fonction rend un identificateur utile pour se desabonner
function BindMsg ( MsgCallback : MsgCallback_T;
                   user_data : UserData_T;
                   regexp : String) return Subscription_Id_T;

procedure BindMsg ( MsgCallback : MsgCallback_T;
                   user_data : UserData_T;
                   regexp : String); 

-- Annulation abonnement
procedure UnbindMsg ( Id : Subscription_Id_T);

-- Tue la main loop Ivy (definitif avant un exit ....)
Procedure Kill;

---------------------------------------------------------------------------
private

Type Dummy;
Type AppClientPtr_T is  access Dummy;
Type Subscription_Id_T is access Dummy; -- identificateur quand on s'abonne

-- Les procedures ou fonctions de la lib ivy en C utilisent ces types
-- pointeurs sur des structures ad hoc.
-- En Ada, pour ce binding, on a juste besoin de recuperer des variables
-- de ces types pour les fournir a d'autres procedures sans intervenir 
-- directement sur les donnees sur lesquelles elles pointent, d'ou le 
-- typage utilise.


-- Autres infos pour poursuivre le portage

--typedef struct _clnt_lst *IvyClientPtr;
--/* identifiant d'une expression reguliere ( Bind/Unbind ) */
--typedef struct _msg_rcv *MsgRcvPtr;


--/* query sur les applications connectees */
--char *IvyGetApplicationName( IvyClientPtr app );
--char *IvyGetApplicationHost( IvyClientPtr app );
--IvyClientPtr IvyGetApplication( char *name );
--char *IvyGetApplicationList();
--char **IvyGetApplicationMessages( IvyClientPtr app); /* demande de reception d'un message */


--/* emission d'un message d'erreur */
--void IvySendError( IvyClientPtr app, int id, const char *fmt, ... );


--/* Message Direct Inter-application */

--void IvyBindDirectMsg( MsgDirectCallback callback, void *user_data);
--void IvySendDirectMsg( IvyClientPtr app, int id, char *msg );

end Ivy;
