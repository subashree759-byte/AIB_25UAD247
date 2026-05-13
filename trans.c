#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// Function prototypes
void initializeFile();
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // Create file if it doesn't exist
    initializeFile();

    // Open binary file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        puts("File could not be opened.");
        return 1;
    }

    while ((choice = enterChoice()) != 5)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        default:
            puts("Invalid choice.");
        }
    }

    fclose(cfPtr);

    puts("Program ended.");
    return 0;
}

// Create empty binary file
void initializeFile()
{
    FILE *filePtr;
    struct clientData blankClient = {0, "", "", 0.0};

    filePtr = fopen("credit.dat", "rb");

    // If file already exists
    if (filePtr != NULL)
    {
        fclose(filePtr);
        return;
    }

    // Create new file
    filePtr = fopen("credit.dat", "wb");

    if (filePtr == NULL)
    {
        puts("Cannot create file.");
        exit(1);
    }

    for (int i = 0; i < SIZE; i++)
    {
        fwrite(&blankClient, sizeof(struct clientData), 1, filePtr);
    }

    fclose(filePtr);
}

// Menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n===== BANK MENU =====\n");
    printf("1 - Create text file\n");
    printf("2 - Update account\n");
    printf("3 - Add account\n");
    printf("4 - Delete account\n");
    printf("5 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);

    return choice;
}

// Create accounts.txt
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL)
    {
        puts("Cannot create text file.");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr,
            "%-6s %-15s %-10s %10s\n",
            "Acct",
            "Last Name",
            "First Name",
            "Balance");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-6u %-15s %-10s %10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    puts("accounts.txt created.");
}

// Add new account
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        puts("Invalid account number.");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        puts("Account already exists.");
    }
    else
    {
        printf("Enter lastname firstname balance:\n");
        scanf("%14s %9s %lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        puts("Account added.");
    }
}

// Update account
void updateRecord(FILE *fPtr)
{
    struct clientData client;

    unsigned int account;
    double transaction;

    printf("Enter account number: ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        puts("Invalid account number.");
        return;
    }

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        puts("Account does not exist.");
    }
    else
    {
        printf("Current balance: %.2f\n",
               client.balance);

        printf("Enter amount (+ deposit / - withdraw): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("New balance: %.2f\n",
               client.balance);
    }
}

// Delete account
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        puts("Invalid account number.");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        puts("Account does not exist.");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        puts("Account deleted.");
    }
}