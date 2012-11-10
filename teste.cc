extern "C" {
#define LDAP_DEPRECATED 1
#include <ldap.h>
//#include <lber.h>
}
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*
 *  authusername could be the name of the user that we are investigating, set it to NULL if we want to list all the users in the LDAP
 *  ldaploginattr could be the search criteria that uniquely identifies a user like samAccountname or UID or UniquePrincipalName
 *  ldapbasedn should be the DN scope like DC=example,DC=local
 *  ldapgroupid should be the identifier for user's group enlistments like MemberOf in Microsoft AD
 **/

int ldap_to_cache (LDAP *ld, const char *authusername, const char *ldaploginattr , const char *ldapbasedn , const char *ldapgroupid){
	int ldap_search_result = -1,ldap_bind_result = 0;
	int num_entries_returned = 0;
	char bind_filter[512] = "";
	int cnt = 1, ret = -11; // return -1 on failure
	LDAPMessage *pMsg = NULL, *entry = NULL;

//	snprintf(bind_filter, sizeof(bind_filter) -1, "(&(objectclass=inetOrgPerson)(%s=%s))", ldaploginattr, (authusername == NULL) ? "*" : authusername);
	snprintf(bind_filter, sizeof(bind_filter) -1, "(&(objectClass=posixGroup)(%s=%s))", ldaploginattr, (authusername == NULL) ? "*" : authusername);

	/*  search for all the users that have the desired ldaploginattr like samAccountname or UID or UPN*/
	static const char *const attrs[] = {"cn", NULL};
	ldap_search_result = ldap_search_ext_s(ld, ldapbasedn, LDAP_SCOPE_SUBTREE, bind_filter, NULL, false, NULL, NULL, NULL, 0, &pMsg);
	//ldap_search_result = ldap_search_ext_s(ld, ldapbasedn, LDAP_SCOPE_SUBTREE, bind_filter, (char**)attrs, false, NULL, NULL, NULL, 0, &pMsg);

	if (ldap_search_result != LDAP_SUCCESS ) {
		printf ("error: %d:%s\n", ldap_search_result, ldap_err2string(ldap_search_result) );
		if(pMsg == NULL)    
			return ret;
	}


	num_entries_returned = ldap_count_entries(ld, pMsg);  // if we were called with authusername = NULL, then we create a list of all users in the LDAP Directory

	for (entry = ldap_first_entry(ld, pMsg); entry != NULL; entry = ldap_next_entry(ld, entry)){
		int i = 0, j = 0;
		char LDAPuser[512] = "", LDAP_user_DN[512] = "", LDAP_user_group_memberships[512] = ""; 
		char *gdn = NULL, **dn_res, **attrib_vals; 

		memset(LDAP_user_group_memberships, '\0', 512);
		char *ptr = LDAP_user_group_memberships;

		/*  retrieve the names of all the users in LDAP that match the desired attribute*/
		attrib_vals = ldap_get_values(ld, entry, ldaploginattr);


		for(i = 0; attrib_vals[i] != NULL; i++)
			snprintf (LDAPuser , 512 -1 , "%s" , attrib_vals[i]);

		gdn = ldap_get_dn(ld, entry);
		snprintf(LDAP_user_DN, 512 -1, "%s",gdn);  

		printf ("LDAP user #%d : %s\n" , cnt, LDAPuser);
		printf ("userDN: %s\n" , LDAP_user_DN);
		printf ("group memberships: (%s):\n" , ldapgroupid);

		dn_res = ldap_get_values(ld, entry, ldapgroupid);

		if(dn_res != NULL) {

			for(j = 0; dn_res[j] != NULL; j++) {
				printf ("%s: %s\n" , ldapgroupid, dn_res[j]);
			}
		}

		cnt++;

		if(i > 0)
			ldap_value_free(attrib_vals);                   
		if(j > 0)       
			ldap_value_free(dn_res);        
		if (gdn != NULL )
			ldap_memfree (gdn) ;
	}

	if (pMsg != NULL)
		ldap_msgfree(pMsg);

	return cnt;
}

int main( int argc, char *argv[] ){
	LDAP *ld;

	char *ldap_host = "ldap://192.168.1.252";
	int ldap_port   = 389;
	int auth_method = LDAP_AUTH_SIMPLE;
	int desired_version = LDAP_VERSION3;
	char *root_dn   = "uid=psycho,ou=People,ou=lccv,o=ufal,dc=br";
	char *root_ps   = argv[1];

	int result;

	result = ldap_initialize(&ld, ldap_host);

	cout << "result: " << result << endl;

	result = ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &desired_version);

	cout << "result: " << result << endl;

	result = ldap_bind_s(ld, root_dn, root_ps, auth_method);
	ldap_to_cache(ld, "psycho", "uid" , "o=ufal,dc=br" , "memberUid" );

	cout << "result: " << result << endl;
	return 0;

}

