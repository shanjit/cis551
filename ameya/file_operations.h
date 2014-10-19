#define MAX_USERS 8
#define MAX_UNAME 32
#define MAX_PASSWD 32

#define SUCCESS 0
#define USER_ALREADY_EXISTS 1
#define FILE_DOESNT_EXIST 2
#define USER_DOESNT_EXIST 3

int is_user_present(char *user_name, FILE *database);
int add_user(char *user_name, char *passwd);
int update_passwd(char *user_name, char *passwd);
int match(char *user_name, char *passwd)
int create_database();
