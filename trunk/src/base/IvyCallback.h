// IvyCallback.h : Interface for the IvyMessageCallback Class
//               : Interface for the IvyDirectMessageCallback Class
//               : Interface for the IvyApplicationCallback Class
//


#ifndef __IVYXTLOOP_H__
#define __IVYXTLOOP_H__

class IvyApplication;

/* Callback for the normal bus Message */

class IvyMessageCallback  {
public:
	virtual void OnMessage (IvyApplication *app, int argc, const char **argv )=0;
	virtual ~IvyMessageCallback()
	{
	}
};


class IvyMessageCallbackFunction: public IvyMessageCallback  {
public:
	typedef	void ( *IvyMessageCallback_fun )( IvyApplication *app, void *user_data, int argc, const char **argv );
	IvyMessageCallback_fun MessageCb;
	void *data;

public:
	IvyMessageCallbackFunction (  IvyMessageCallback_fun m_cb, void *udata = NULL ) : MessageCb( m_cb ) 
	{
		data = udata;
	}
	~IvyMessageCallbackFunction ()
	{
	}
	void OnMessage (IvyApplication *app, int argc, const char **argv)
	{
	(*MessageCb) (app, data, argc, argv);
	}

/* raccourci d'ecriture */
#define BUS_CALLBACK(  m , d ) new IvyMessageCallbackFunction( m, d )
};
/* template Class Callback for the normal bus Message */
template <class T> class IvyMessageCallbackOf : public IvyMessageCallback {

protected:
	T*      Object;
	typedef	void ( T::*IvyMessageCallback_fun )( IvyApplication *app, int argc, const char **argv );
	IvyMessageCallback_fun MessageCb;

public:
	IvyMessageCallbackOf ( T* o, IvyMessageCallback_fun m_cb ) : Object (o), MessageCb( m_cb )
	{
	}
	~IvyMessageCallbackOf ()
	{
	}
	void OnMessage (IvyApplication *app, int argc, const char **argv)
	{
	(Object->*MessageCb) (app, argc, argv);
	}

/* raccourci d'ecriture */
#define BUS_CALLBACK_OF( cl, m ) new IvyMessageCallbackOf<cl>( this, m )
};
/* Callback for the direct Message */
class IvyDirectMessageCallback {
public:
  virtual void OnDirectMessage (IvyApplication *app, int id, const char *arg ) = 0;
  virtual ~IvyDirectMessageCallback () {};
};

/* Application Callback */

class IvyApplicationCallback {
public:
	virtual void OnApplicationConnected (IvyApplication *app) = 0;
	virtual void OnApplicationDisconnected (IvyApplication *app) = 0;
	virtual ~IvyApplicationCallback()
	{
	}
};
class IvyApplicationNullCallback : public IvyApplicationCallback {
public:
	virtual void OnApplicationConnected (IvyApplication *app)
	{};
	virtual void OnApplicationDisconnected (IvyApplication *app)
	{};
	virtual ~IvyApplicationNullCallback()
	{
	}
};

// Static function CB
class IvyApplicationCallbackFunction: public IvyApplicationCallback  {
public:
	typedef	void ( *IvyApplicationCallback_fun )( IvyApplication *app );
	IvyApplicationCallback_fun ApplicationConnectedCb;
	IvyApplicationCallback_fun ApplicationDisconnectedCb;

public:
	IvyApplicationCallbackFunction ( IvyApplicationCallback_fun con_cb,  IvyApplicationCallback_fun disc_cb) 
		: ApplicationConnectedCb( con_cb ), ApplicationDisconnectedCb( disc_cb )
	{
	}
	~IvyApplicationCallbackFunction ()
	{
	}
	virtual void OnApplicationConnected (IvyApplication *app)
	{
	(*ApplicationConnectedCb) (app);
	};
	virtual void OnApplicationDisconnected (IvyApplication *app)
	{
	(*ApplicationDisconnectedCb) (app);
	};
	
/* raccourci d'ecriture */
#define BUS_APPLICATION_CALLBACK(  conn, disconn ) new IvyApplicationCallbackFunction(  conn, disconn )
};

/* Binding Callback */

class IvyBindingCallback {
public:
	virtual void OnAddBind (IvyApplication *app, int id, const char * regexp) = 0;
	virtual void OnRemoveBind (IvyApplication *app, int id, const char * regexp) = 0;
	virtual void OnFilterBind (IvyApplication *app, int id, const char * regexp) = 0;
	virtual void OnChangeBind (IvyApplication *app, int id, const char * regexp) = 0;
	virtual ~IvyBindingCallback()
	{
	}
};

class IvyBindingNullCallback : public IvyBindingCallback {
public:
	virtual void OnAddBind (IvyApplication *app, int id, const char * regexp)
	{};
	virtual void OnRemoveBind (IvyApplication *app, int id, const char * regexp)
	{};
	virtual void OnFilterBind (IvyApplication *app, int id, const char * regexp)
	{};
	virtual void OnChangeBind (IvyApplication *app, int id, const char * regexp)
	{};
	virtual ~IvyBindingNullCallback()
	{
	}
};
// Static function CB
class IvyBindingCallbackFunction: public IvyBindingCallback  {
public:
	typedef	void ( *IvyBindingCallback_fun )( IvyApplication *app, int id, const char * regexp );
	IvyBindingCallback_fun BindingAddCb;
	IvyBindingCallback_fun BindingRemoveCb;
	IvyBindingCallback_fun BindingFilterCb;
	IvyBindingCallback_fun BindingChangeCb;

public:
	IvyBindingCallbackFunction ( IvyBindingCallback_fun add_cb,  IvyBindingCallback_fun remove_cb, IvyBindingCallback_fun filter_cb, IvyBindingCallback_fun change_cb ) 
		: BindingAddCb( add_cb ), BindingRemoveCb( remove_cb ), BindingFilterCb( filter_cb ), BindingChangeCb( change_cb )
	{
	}
	~IvyBindingCallbackFunction ()
	{
	}
	virtual void OnAddBind (IvyApplication *app, int id, const char * regexp)
	{
	if(BindingAddCb) (*BindingAddCb) (app, id, regexp);
	};
	virtual void OnRemoveBind (IvyApplication *app, int id, const char * regexp)
	{
	if (BindingRemoveCb) (*BindingRemoveCb) (app, id, regexp);
	};
	
	virtual void OnFilterBind (IvyApplication *app, int id, const char * regexp)
	{
	if(BindingFilterCb ) (*BindingFilterCb) (app, id, regexp);
	};
	
	virtual void OnChangeBind (IvyApplication *app, int id, const char * regexp)
	{
		if(BindingChangeCb ) (*BindingChangeCb) (app, id, regexp);
	};
	
/* raccourci d'ecriture */
#define BUS_BINDING_CALLBACK(  add, remove, filter, change ) new IvyBindingCallbackFunction(  add, remove, filter, change )
};


/* Callback for the die Message */
class IvyDieCallback {
public:
	virtual bool OnDie (IvyApplication *app, int id, const char *arg ) = 0;
	virtual ~IvyDieCallback()
	{
	}
};
#endif // __IVYXTLOOP_H__


