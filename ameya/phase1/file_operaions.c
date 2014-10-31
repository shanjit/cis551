#include <stdio.h>
#include <string.h>
#define MAX_USERS 8
#define MAX_UNAME 32
#define MAX_PASSWD 32

#define SUCCESS 0
#define USER_ALREADY_EXISTS 1
#define FILE_DOESNT_EXIST 2
#define USER_DOESNT_EXIST 3

int is_user_present(char *user_name, FILE *database)
{
	char user_in_file[MAX_UNAME];
	char passwd_in_file[MAX_PASSWD];
	while(fscanf(database, "%s\t%s\n", user_in_file, passwd_in_file) != EOF)
	{
		if(strcmp(user_in_file, user_name) == 0)
		{
			printf("user is present\n");
			return 1;
		}
	}
	return 0;
}
		
int add_user(char *user_name, char *passwd)
{
	FILE *database;
	database = fopen("database.txt", "r");
	if(is_user_present(user_name, database))
	{
		return USER_ALREADY_EXISTS;
	}
	fclose(database);
	database = fopen("database.txt", "a");
	fprintf(database, "%s\t%s\n", user_name, passwd);
	fclose(database);
	return SUCCESS;
}

int update_passwd(char *user_name, char *passwd)
{
	FILE *database;
	int i = 0;
	char users_in_file[MAX_USERS][MAX_UNAME];
	char passwds_in_file[MAX_USERS][MAX_PASSWD];
	database = fopen("database.txt", "r");
	if(!database)
	{
		return FILE_DOESNT_EXIST;
	}

	if(!is_user_present(user_name, database))
	{
		printf("user not found\n");
		return USER_DOESNT_EXIST;
	}
	rewind(database);
	while(fscanf(database, "%s\t%s\n", users_in_file[i], passwds_in_file[i]) != EOF)
	{
		printf("%s\n",users_in_file[i]);
		i++;
	}
	fclose(database);
	database = fopen("database.txt", "w");
	i--;

	while(i>=0)
	{
		if(strcmp(users_in_file[i], user_name) == 0)
			fprintf(database, "%s\t%s\n", users_in_file[i], passwd);
		else
			fprintf(database, "%s\t%s\n", users_in_file[i], passwds_in_file[i]);
		i--;
	}
	fclose(database);
	return SUCCESS;
}

int match(char *user_name, char *passwd)
{
	FILE *database;
	char user_in_file[MAX_UNAME];
	char passwd_in_file[MAX_PASSWD];
	database = fopen("database.txt", "r");
	while(fscanf(database, "%s\t%s\n", user_in_file, passwd_in_file) != EOF)
	{
		if(strcmp(user_in_file, user_name) == 0)
		{
			if(strcmp(passwd_in_file, passwd) == 0)
				return 1;
			else
				return 0;
		}
	}
	fclose(database);
	return 0;
}

int create_database()
{
	FILE *database;
	database = fopen("database.txt", "w");
	if(!database)
		return -1;
	fclose(database);
	return SUCCESS;
}


int main()
{
	FILE *temp;
	char temp1[8], temp2[8];
/*
	temp = fopen("temp.txt", "r");
	fscanf(temp, "%s\t%s", temp1, temp2);
	printf("temp1:%s\n", temp1);
	printf("temp2:%s\n", temp2);
	fscanf(temp, "%s\t%s", temp1, temp2);
	printf("temp1:%s\n", temp1);
	printf("temp2:%s\n", temp2);
*/

	create_database();
	add_user("ameyam", "passwd");
	add_user("more", "pwd");
	update_passwd("ameyam", "pppp");
	update_passwd("upenn", "sd");
	add_user("upenn","td");
	printf("%d\n", match("more", "pwd"));
	printf("%d\n", match("mia", "pwd"));
	printf("%d\n", match("upenn", "uu"));
}
