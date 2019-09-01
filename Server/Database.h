typedef const std::string & str;

class Database {
private:
    static MYSQL *myconnect;
    static char database_username[64];
    static char database_userpwd[64];

    static bool is_create();

    static bool setup_database();

    static bool connect();

    static bool disconnect();

public:

    static bool init();



    // functional functions

    /*
     *
     * {
     *     "status": [true | false]
     *     "msg": "..."
     * }
     */
    static json check_password(str username, str password);


    static json signup(str username, str password);


    static
}