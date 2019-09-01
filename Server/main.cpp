#include <iostream>
#include <string>
#include <gtk/gtk.h>
#include <mysql/mysql.h>

using std::string;


MYSQL *myconnect = NULL;
char database_username[64] = "test";
char database_userpwd[64] = "test";
bool iscreate()
{
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
bool Setup_Database()
{
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
bool my_connect()
{
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
void my_disconnect()
{
    mysql_close(myconnect);
    myconnect = NULL;
}
//gboolean Check_password(gstring)
bool Check_Password(string user_name, string user_password)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT * FROM alluser where username = '%s' and password = '%s';", user_name.c_str(), user_password.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    int ans=0;
    while((row = mysql_fetch_row(res)) != NULL)
    {
        ans++;
    }
    if(ans==0)
    {
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    return 1;
}
int main() {
    my_connect();
    Setup_Database();
    return 0;
}