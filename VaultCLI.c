#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void xorEncrypt(char *data, char *key, int dataLen) {
    int keyLen = strlen(key);
    if (keyLen == 0) return;

    for (int i = 0; i < dataLen; i++) {
        data[i] ^= key[i % keyLen];
    }
}

int main() {
    srand(time(NULL));
    _Bool MasterPassCheck = false;
    int condition = 1;
    struct entry {
        char service[100];
        char username[100];
        char password[100];
    };
    FILE *masterPassword;
    char savedMaster[100];
    char testMaster[100];
    system("cls");

    masterPassword = fopen("masterPassword.bin", "rb");

    if (masterPassword) {
        fread(savedMaster, sizeof(savedMaster), 1, masterPassword);
        fclose(masterPassword);

        printf("==== EXISTING USER ====\n");
        while (!MasterPassCheck) {
            printf("Enter Master Password: ");

            fgets(testMaster, sizeof(testMaster), stdin);
            testMaster[strcspn(testMaster, "\n")] = '\0';

            if (strcmp(testMaster, savedMaster) == 0) {
                MasterPassCheck = true;
            } else {
                system("cls");
                printf("==== INCORRECT PASSWORD ====\n");
            }
        }
    } else {
        masterPassword = fopen("masterPassword.bin", "wb");
        printf("==== NEW SETUP ====\n");
        printf("Create Master Password: ");

        fgets(savedMaster, sizeof(savedMaster), stdin);
        savedMaster[strcspn(savedMaster, "\n")] = '\0';

        fwrite(&savedMaster, sizeof(savedMaster), 1, masterPassword);
        fclose(masterPassword);
    }

    system("cls");

    while (condition != 6) {
        printf("==== PASSWORD VAULT ====\n");
        printf("1. Add new password\n");
        printf("2. View all saved passwords\n");
        printf("3. Search for a password\n");
        printf("4. Check strength of password\n");
        printf("5. Generate strong passwords\n");
        printf("6. Exit\n\n");
        printf("Choose an option: ");

        if (scanf("%d", &condition) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            printf("Invalid input. Please enter a number.\n");

            printf("\nPress Enter to return to menu...");
            getchar();
            system("cls");

            continue;
        }

        system("cls");

        FILE *passwordVault;

        if (condition == 1) {
            _Bool overallMatch = false;
            int numStructs;
            struct entry testEntry;
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            printf("==== ADD PASSWORD ====\n");
            printf("Please enter the website or service name: ");
            fgets(testEntry.service, sizeof(testEntry.service), stdin);
            testEntry.service[strcspn(testEntry.service, "\n")] = '\0';

            passwordVault = fopen("passwordVault.bin", "rb");

            if (passwordVault) {
                int bytes;
                for(bytes = 0; getc(passwordVault) != EOF; ++bytes);

                rewind(passwordVault);
                numStructs = bytes/sizeof(testEntry);
                struct entry passwordEntry[numStructs];

                for (int i = 0; i < numStructs; i++) {
                    fread(&passwordEntry[i], sizeof(testEntry), 1, passwordVault);
                }

                for (int i = 0; i < numStructs; i++) {
                    _Bool match = true;
                    xorEncrypt(passwordEntry[i].service, savedMaster, 100);
                    xorEncrypt(passwordEntry[i].username, savedMaster, 100);
                    xorEncrypt(passwordEntry[i].password, savedMaster, 100);

                    int j = 0;

                    while (passwordEntry[i].service[j] != '\0' && testEntry.service[j] != '\0') {
                        if (tolower(passwordEntry[i].service[j]) != tolower(testEntry.service[j])) {
                            match = false;
                            break;
                        }
                        j++;
                    }

                    if (passwordEntry[i].service[j] != '\0' || testEntry.service[j] != '\0') {
                        match = false;
                    }

                    if (match) {
                        printf("Service already saved.\n");
                        overallMatch = true;
                        printf("\nPress Enter to return to menu...");
                        getchar();
                        system("cls");
                        break;
                    }
                }
                fclose(passwordVault);
            }

            if (!overallMatch) {
                xorEncrypt(testEntry.service, savedMaster, 100);
                printf("Please enter the username: ");
                fgets(testEntry.username, sizeof(testEntry.username), stdin);
                testEntry.username[strcspn(testEntry.username, "\n")] = '\0';
                xorEncrypt(testEntry.username, savedMaster, 100);
                printf("Please enter the password: ");
                fgets(testEntry.password, sizeof(testEntry.password), stdin);
                testEntry.password[strcspn(testEntry.password, "\n")] = '\0';
                xorEncrypt(testEntry.password, savedMaster, 100);

                passwordVault = fopen("passwordVault.bin", "ab");
                if (!passwordVault) {
                    printf("Error opening file.\n");
                    continue;
                }

                printf("\nPassword Saved.");
                printf("\nPress Enter to return to menu...");
                getchar();
                system("cls");

                fwrite(&testEntry, sizeof(testEntry), 1, passwordVault);
                fclose(passwordVault);
            }
        } else if (condition == 2) {
            int numStructs;
            struct entry testEntry;
            passwordVault = fopen("passwordVault.bin", "rb");

            if (passwordVault) {
                int bytes;
                for(bytes = 0; getc(passwordVault) != EOF; ++bytes);

                rewind(passwordVault);
                numStructs = bytes/sizeof(testEntry);
                struct entry passwordEntry[numStructs];

                for (int i = 0; i < numStructs; i++) {
                    fread(&passwordEntry[i], sizeof(testEntry), 1, passwordVault);
                }

                printf("==== PASSWORD LIST ====\n");
                for (int i = 0; i < numStructs; i++) {
                    xorEncrypt(passwordEntry[i].service, savedMaster, 100);
                    xorEncrypt(passwordEntry[i].username, savedMaster, 100);
                    xorEncrypt(passwordEntry[i].password, savedMaster, 100);

                    printf("Service: %s\n", passwordEntry[i].service);
                    printf("Username: %s\n", passwordEntry[i].username);
                    printf("Password: %s\n\n", passwordEntry[i].password);
                }

                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                printf("\nPress Enter to return to menu...");
                getchar();
                system("cls");

                fclose(passwordVault);
            } else {
                printf("No passwords saved.\n");

                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                printf("\nPress Enter to return to menu...");
                getchar();
                system("cls");
            }
        } else if (condition == 3) {
            _Bool overallMatch = false;
            char serviceSearch[100];
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("==== PASSWORD SEARCH ====\n");
            printf("What service would you like to search for?: ");
            fgets(serviceSearch, sizeof(serviceSearch), stdin);
            serviceSearch[strcspn(serviceSearch, "\n")] = '\0';

            int numStructs;
            struct entry testEntry;
            passwordVault = fopen("passwordVault.bin", "rb");

            if (passwordVault) {
                int bytes;
                for(bytes = 0; getc(passwordVault) != EOF; ++bytes);

                rewind(passwordVault);
                numStructs = bytes/sizeof(testEntry);
                struct entry passwordEntry[numStructs];

                for (int i = 0; i < numStructs; i++) {
                    fread(&passwordEntry[i], sizeof(testEntry), 1, passwordVault);
                }

                for (int i = 0; i < numStructs; i++) {
                    _Bool match = true;
                    xorEncrypt(passwordEntry[i].service, savedMaster, 100);
                    xorEncrypt(passwordEntry[i].username, savedMaster, 100);
                    xorEncrypt(passwordEntry[i].password, savedMaster, 100);

                    int j = 0;

                    while (passwordEntry[i].service[j] != '\0' && serviceSearch[j] != '\0') {
                        if (tolower(passwordEntry[i].service[j]) != tolower(serviceSearch[j])) {
                            match = false;
                            break;
                        }
                        j++;
                    }

                    if (passwordEntry[i].service[j] != '\0' || serviceSearch[j] != '\0') {
                        match = false;
                    }

                    if (match) {
                        system("cls");
                        printf("==== MATCH FOUND ====\n");
                        printf("Service: %s\n", passwordEntry[i].service);
                        printf("Username: %s\n", passwordEntry[i].username);
                        printf("Password: %s\n\n", passwordEntry[i].password);
                        overallMatch = true;

                        printf("\nPress Enter to return to menu...");
                        getchar();
                        system("cls");
                    }
                }

                if (!overallMatch) {
                    printf("No match found.\n");

                    printf("\nPress Enter to return to menu...");
                    getchar();
                    system("cls");
                }

                fclose(passwordVault);
            } else {
                printf("No passwords saved.\n");

                printf("\nPress Enter to return to menu...");
                getchar();
                system("cls");
            }
        } else if (condition == 4) {
            char password[100];
            int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            printf("==== PASSWORD STRENGTH CHECKER ====\n");
            printf("Enter your password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';

            for (int i = 0; password[i] != '\0'; i++) {
                if (isupper(password[i])) hasUpper = 1;
                else if (islower(password[i])) hasLower = 1;
                else if (isdigit(password[i])) hasDigit = 1;
                else if (ispunct(password[i])) hasSpecial = 1;
            }

            int checks = hasUpper + hasLower + hasDigit + hasSpecial;

            if (checks == 0 || strlen(password) < 5) {
                printf("Very Weak Password\n");
            }
            else if (checks == 1 || strlen(password) < 8) {
                printf("Weak Password\n");
            }
            else if (checks >= 3 && strlen(password) > 15) {
                printf("Very Strong Password\n");
            }
            else if (checks >= 3 && strlen(password) > 11) {
                printf("Strong Password\n");
            }
            else if (checks > 1) {
                printf("Moderate Password\n");
            }
            else {
                printf("Weak Password\n");
            }

            printf("\nPress Enter to return to menu...");
            getchar();
            system("cls");
        } else if (condition == 5) {
            int numPasswords;
            char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
            char digits[]  = "0123456789";
            char punct[]   = "!@#$%&";

            printf("==== PASSWORD GENERATOR ====\n");
            printf("Enter number of passwords: ");

            if (scanf("%d", &numPasswords) != 1) {
                printf("Invalid input. Please enter a number.\n");
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);
                continue;
            }

            system("cls");
            printf("==== PASSWORDS ====\n");
            for (int j = 0; j < numPasswords; j++) {
                char password[21];
                password[0] = '\0';

                for (int i = 0; i < 20; i++) {
                    if (i < 13) {
                        password[i] = letters[rand() % strlen(letters)];
                    }
                    else if (i == 13) {
                        password[i] = punct[rand() % strlen(punct)];
                    }
                    else if (i < 16) {
                        password[i] = digits[rand() % strlen(digits)];
                    }
                    else {
                        password[i] = letters[rand() % strlen(letters)];
                    }
                }

                password[20] = '\0';

                printf("%s\n", password);
            }

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            printf("\nPress Enter to return to menu...");
            getchar();
            system("cls");

        } else if (condition == 6) {
            break;
        } else {
            printf("Unknown Input Entered.\n");

            printf("\nPress Enter to return to menu...");
            getchar();
            system("cls");

        }
    }

    return 0;
}
