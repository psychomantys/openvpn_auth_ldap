#include	<openvpn_plugin.h>
#include	<main_obj.h>
#include	<cstring>

/*
 * Given an environmental variable name, search
 * the envp array for its value, returning it
 * if found or NULL otherwise.
 */
static const char *
get_env (const char *name, const char *envp[])
{
  if (envp)
    {
      int i;
      const int namelen = strlen (name);
      for (i = 0; envp[i]; ++i)
	{
	  if (!strncmp (envp[i], name, namelen))
	    {
	      const char *cp = envp[i] + namelen;
	      if (*cp == '=')
		return cp + 1;
	    }
	}
    }
  return NULL;
}


OPENVPN_EXPORT openvpn_plugin_handle_t
openvpn_plugin_open_v1( unsigned int *type_mask, const char *argv[], const char *envp[]){
	Main_obj *handle;

	/*
	 * Allocate our context
	 */
	handle=new Main_obj( argv[1] );

	/*
	 * We are only interested in intercepting the
	 * --auth-user-pass-verify callback.
	 */
	*type_mask = OPENVPN_PLUGIN_MASK(OPENVPN_PLUGIN_AUTH_USER_PASS_VERIFY);

	return (openvpn_plugin_handle_t)handle;
}

OPENVPN_EXPORT int
openvpn_plugin_func_v1 (openvpn_plugin_handle_t handle, const int type, const char *argv[], const char *envp[]){
	/* get username/password from envp string array */
	const char *username = get_env ("username", envp);
	const char *password = get_env ("password", envp);

	return ((Main_obj*)(handle))->auth(username, password);
}

OPENVPN_EXPORT void
openvpn_plugin_close_v1 (openvpn_plugin_handle_t handle){
	Main_obj *context=(Main_obj*)handle;
	if(context)
		delete context;
}

