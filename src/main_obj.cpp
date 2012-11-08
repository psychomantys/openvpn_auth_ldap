/*
 * =====================================================================================
 *
 *       Filename:  main_obj.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08-11-2012 10:58:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include	<ldap_cdefs.h>
#include	<main_obj.hpp>

Main_obj::Main_obj(const std::string &filename){}

int Main_obj::auth(const std::string &username, const std::string &password ){
	LDAP *ld;

	char *ldap_host = "ldap://192.168.1.252";
	int ldap_port   = 389;
	int auth_method = LDAP_AUTH_SIMPLE;
	int desired_version = LDAP_VERSION3;
	char *root_dn   = "ou=people,dc=localhost,dc=local";
	char *root_ps   = "password";

	int result;

	result = ldap_initialize(&ld, ldap_host);

	cout << "result: " << result << endl;

	result = ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &desired_version);

	cout << "result: " << result << endl;

	result = ldap_bind_s(ld, root_dn, root_ps, auth_method);

	cout << "result: " << result << endl;

	return 0;
}


