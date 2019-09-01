#include "Database.h"

MYSQL Database::myconnect = nullptr;
char Database::username[64] = "IATOW";
char Database::usepwd[64] = "yaoxilai123";

bool Database::is_create() {
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    int ans = 0;
    sprintf(query_buf,"SELECT * FROM information_schema.SCHEMATA where SCHEMA_NAME = 'kunkun';");
    if(mysql_query(myconnect,query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        ans++;
    }
    if(ans == 0)
    {
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    return 1;
}

bool Database::setup_database() {
    char query_buf[4096] = "\0";
    sprintf(query_buf, "CREATE SCHEMA kunkun default character set utf8 collate utf8_general_ci;");
    if(!iscreate())
    {
        if(mysql_query(myconnect, query_buf))
        {
            fprintf(stderr, "%s\n", mysql_error(myconnect));
            exit(1);
        }
    }
    return 1;
}

bool Database::connect() {
    myconnect = mysql_init(myconnect);
    if(mysql_real_connect(myconnect, "localhost", database_username, database_userpwd, NULL, 0, NULL, 0))
    {
        return TRUE;
    }
    else
    {
        myconnect = NULL;
        return FALSE;
    }
}

bool Database::disconnect() {
    mysql_close(myconnect);
    myconnect = nullptr;
}

bool Database::init() {
    if (!connect()) {
        return false;
    }

    if (!is_create()) {
        if (!setup_database()) {
            return false;
        }
    }

    return true;
}