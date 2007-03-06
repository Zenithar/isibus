// Ivy.h: interface for the Ivy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__IVY_H)
#define __IVY_H

#ifdef USE_GLFW
#include <GL/glfw.h>
#endif

namespace IvyC
{
 #include <Ivy/ivyloop.h>
 #include <Ivy/ivysocket.h>
 #include <Ivy/ivy.h>
}
#include "IvyCallback.h"

class IvyApplication;

class  Ivy  
{

public:

	Ivy();
#ifdef USE_GLFW
	Ivy( const char *name, const char* ready, IvyApplicationCallback *callback, 
	     bool argaFrc, GLFWmutex  _ivyCbmutex = NULL);
#else
	Ivy( const char *name, const char* ready, IvyApplicationCallback *callback, 
	     bool argaFrc);
#endif
	virtual ~Ivy();

	static long  BindMsg (const char *regexp, IvyMessageCallback *cb );
        static long  BindMsg (IvyMessageCallback *cb, const char *regexp, ... );
	static void UnbindMsg( int id );

        static int  SendMsg(const char * message, ... );
	static void SendDirectMsg( IvyApplication *app, int id, 
				   const char *message);
	static void BindDirectMsg( IvyDirectMessageCallback *callback );


	static void SetBindCallback(IvyBindingCallback* bind_callback );
	static void SetFilter( int argc, const char **argv );
	static void start(const char *domain);
	static void stop();
#ifndef USE_GLFW        
static void ivyMainLoop ();
#endif

protected:

	static void ApplicationCb( IvyC::IvyClientPtr app, void *user_data, 
				   IvyC::IvyApplicationEvent event ) ;
	static void DieCb( IvyC::IvyClientPtr app, void *user_data, int id ) ;
	static void MsgCb( IvyC::IvyClientPtr app, void *user_data, int argc, char **argv ) ;
	static void MsgDirectCb( IvyC::IvyClientPtr app, void *user_data, int id, char *msg ) ;
        static void BindCallbackCb( IvyC::IvyClientPtr app, void *user_data, int id, char *msg,
			            IvyC::IvyBindEvent event) ;
#ifdef USE_GLFW
  static void GLFWCALL ivyMainLoopInSeparateThread (void *arg);	

private:
  static GLFWmutex ivyCbmutex;

#endif
};

#endif // !defined(__IVY_H)

