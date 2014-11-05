#include <stdio.h>
#include <string.h>

void encrypt(char *plaintext, char *ciphertext)
{
	int i = 0;
	while(plaintext[i] !='\0')
	{
		ciphertext[i] = plaintext[i] + 7;
		i++;
	}
	ciphertext[i] = '\0';
}

void decrypt(char *ciphertext, char *plaintext)
{
	int i = 0;
	while(ciphertext[i] !='\0')
	{
		plaintext[i] = ciphertext[i] - 7;
		i++;
	}
	plaintext[i] = '\0';
}


int main()
{
	char input[128];
	char encrypted[128];
	char decrypted[128];
	strcpy(input, "ameya");
	encrypt(input, encrypted);
	printf("encrypted: %s\n", encrypted);
	decrypt(encrypted, decrypted);
	printf("decrypted: %s\n", decrypted);
	return 0;
}

