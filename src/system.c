#include "header.h"

#define MAX_RECORDS 100
const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r) {
  return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s", &r->id, &r->userId,
                name, &r->accountNbr, &r->deposit.month, &r->deposit.day,
                &r->deposit.year, r->country, &r->phone, &r->amount,
                r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r) {
  fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", r.id, u.id, u.name,
          r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year,
          r.country, r.phone, r.amount, r.accountType);
}

void updateUserAccountInFile(FILE *ptr, struct Record r) {
  fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n", r.id, r.userId,
          r.name, r.accountNbr, r.deposit.month, r.deposit.day, r.deposit.year,
          r.country, r.phone, r.amount, r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u) {
  int option;
  if (notGood == 0) {
  invalid:
    printf("\n\t\tEnter 0 to try again, 1 to return to main menu and 2 to "
           "exit! \n\n");
    scanf("%d", &option);
    if (option == 0)
      f(u);
    else if (option == 1)
      mainMenu(u);
    else if (option == 2)
      exit(0);
    else {
      printf("Insert a valid operation!\n");
      goto invalid;
    }
  } else {
    printf("\n\t\tEnter 1 to go to the main menu and 0 to exit! \n\n");
    scanf("%d", &option);
  }
  if (option == 1) {
    system("clear");
    mainMenu(u);
  } else {
    system("clear");
    exit(1);
  }
}

void success(struct User u) {
  int option;

invalid:
  printf("\n\t\tEnter 1 to go to the main menu and 0 to exit! \n\n");
  scanf("%d", &option);
  system("clear");
  if (option == 1) {
    mainMenu(u);
  } else if (option == 0) {
    exit(1);
  } else {
    printf("\n\t\tInsert a valid operation!\n");
    goto invalid;
  }
}

int getUserId(const char *username) {
  FILE *fp = fopen("./data/users.txt", "r");
  if (!fp) {
    perror("\n\t\tError opening file");
    exit(1);
  }

  char id[5], name[50], pass[50];
  while (fscanf(fp, "%s %s %s", id, name, pass) != EOF) {
    if (strcmp(name, username) == 0) {
      fclose(fp);
      return atoi(id);
    }
  }

  fclose(fp);
  printf("\n\t\tUser not found!\n");
  exit(1);
}

void createNewAcc(struct User u) {
  struct Record r;
  struct Record cr;
  char userName[50];
  FILE *pf = fopen(RECORDS, "a+");
  FILE *rf = fopen("./data/records.txt", "r");

  if (!rf) {
    perror("\n\t\tError opening file");
    exit(1);
  }

  int lastRecordId = 0;
  while (fscanf(rf, "%d %*d %*s %*d %*s %*s %*d %*lf %*s", &r.id) != EOF) {
    lastRecordId = r.id;
  }

  fclose(rf);

noAccount:
  system("clear");
  printf("\t\t\t===== New record =====\n");

  printf("\n\t\tEnter today's date(mm/dd/yyyy): ");
  scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

  printf("\n\t\tEnter the account number: ");
  scanf("%d", &r.accountNbr);

  while (getAccountFromFile(pf, userName, &cr)) {
    if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
      printf("\n\t\t✖ This Account already exists for this user\n\n");
      goto noAccount;
    }
  }

  r.id = lastRecordId + 1;
  u.id = getUserId(u.name);
  printf("\n\t\tEnter the country: ");
  scanf("%s", r.country);

  printf("\n\t\tEnter the phone number: ");
  scanf("%d", &r.phone);

  printf("\n\t\tEnter amount to deposit: $");
  scanf("%lf", &r.amount);

  printf("\n\t\tChoose the type of account:\n\n\t\t -> savings\n\t\t -> "
         "current\n\t\t -> "
         "fixed01(for 1 year)\n\t\t -> fixed02(for 2 years)\n\t\t -> "
         "fixed03(for 3 "
         "years)\n\n\t\tEnter your choice: ");
  scanf("%s", r.accountType);

  saveAccountToFile(pf, u, r);

  fclose(pf);
  printf("\n\t\t✔ Success!\n");
  success(u);
}

void checkAllAccounts(struct User u) {
  char userName[100];
  struct Record r;
  int accountsFound = 0;

  FILE *pf = fopen(RECORDS, "r");

  system("clear");
  printf("\t\t====== All accounts for %s =====\n\n", u.name);
  while (getAccountFromFile(pf, userName, &r)) {
    if (strcmp(userName, u.name) == 0) {
      accountsFound = 1;
      printf("\t\t_____________________\n");
      printf(
          "\n\t\tAccount number: %d\n\t\tDeposit date: %d/%d/%d\n\t\tCountry: "
          "%s \n\t\tPhone number: %d \n\t\tAmount deposited: $%.2f \n\t\tType "
          "of account: %s\n",
          r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
          r.country, r.phone, r.amount, r.accountType);
    }
  }

  if (accountsFound == 0) {
    printf("\t\tNo accounts found for %s.\n", u.name);
  }

  fclose(pf);
  success(u);
}

void updateAccountInformation(struct User u) {
  int accountNbr, choice;
  char newCountry[50];
  int newPhoneNumber;

  system("clear");

  printf("\t\t====== Update accounts for %s =====\n\n", u.name);
  printf("\n\t\tEnter the account number you wish to update: ");
  scanf("%d", &accountNbr);

  printf("\n\t\tWhich field do you want to update?\n\n\t\t 1. Phone "
         "number\n\n\t\t 2. Country\n\n\t\tEnter choice (1/2): ");
  scanf("%d", &choice);

  struct Record records[MAX_RECORDS];
  int recordCount = 0;

  FILE *pf = fopen(RECORDS, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    return;
  }

  while (getAccountFromFile(pf, records[recordCount].name,
                            &records[recordCount])) {
    recordCount++;
  }
  fclose(pf);

  int found = 0;
  for (int i = 0; i < recordCount; i++) {
    if (records[i].accountNbr == accountNbr &&
        strcmp(records[i].name, u.name) == 0) {
      found = 1;
      switch (choice) {
      case 1:
        printf("\n\t\tEnter new phone number: ");
        scanf("%d", &newPhoneNumber);
        records[i].phone = newPhoneNumber;
        break;
      case 2:
        printf("\n\t\tEnter new country: ");
        scanf("%s", newCountry);
        strcpy(records[i].country, newCountry);
        break;
      default:
        printf("\n\t\tInvalid choice. Returning to main menu.\n");
        mainMenu(u);
        return;
      }
      break;
    }
  }

  pf = fopen(RECORDS, "w");
  if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    return;
  }

  for (int i = 0; i < recordCount; i++) {
    updateUserAccountInFile(pf, records[i]);
  }
  fclose(pf);

  if (found) {
    printf("\n\t\tAccount information updated successfully.\n");
    success(u);
  } else {
    printf("\n\t\tNo account found with account number %d.\n", accountNbr);
    stayOrReturn(0, updateAccountInformation, u);
  }
}

void checkAccountDetails(struct User u) {
  int accountNbr;
  struct Record r;
  char userName[50];
  int found = 0;

  system("clear");
  printf("\t\t====== Check Account Details for %s =====\n\n", u.name);
  printf("\n\t\tEnter the account number you wish to check: ");
  scanf("%d", &accountNbr);

  FILE *pf = fopen(RECORDS, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    return;
  }

  while (getAccountFromFile(pf, userName, &r)) {
    if (r.accountNbr == accountNbr && strcmp(userName, u.name) == 0) {
      found = 1;
      break;
    }
  }

  fclose(pf);

  if (!found) {
    printf("\n\t\tNo account found with account number %d.\n", accountNbr);
    stayOrReturn(0, checkAccountDetails, u);
    return;
  }

  printf("\n\t\tAccount number: %d\n\t\tDeposit date: %d/%d/%d\n\t\tCountry: "
         "%s \n\t\tPhone number: %d \n\t\tAmount deposited: $%.2f \n\t\tType "
         "of account: %s\n",
         r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
         r.country, r.phone, r.amount, r.accountType);

  double interestRate;
  if (strcmp(r.accountType, "savings") == 0) {
    interestRate = 0.07;
  } else if (strcmp(r.accountType, "fixed01") == 0) {
    interestRate = 0.04;
  } else if (strcmp(r.accountType, "fixed02") == 0) {
    interestRate = 0.05;
  } else if (strcmp(r.accountType, "fixed03") == 0) {
    interestRate = 0.08;
  } else if (strcmp(r.accountType, "current") == 0) {
    printf("\n\t\tYou will not get interests because the account is of "
           "type current\n");
    success(u);
    return;
  } else {
    printf("\n\t\tInvalid account type.\n");
    stayOrReturn(0, checkAccountDetails, u);
    return;
  }

  double interest = r.amount * interestRate / 12;
  printf("\n\t\tYou will get $%.2f as interest on day %d of every month.\n",
         interest, r.deposit.day);
  success(u);
}

void makeTransaction(struct User u) {
  int accountNbr;
  double amount;
  char transactionType[10]; // either "deposit" or "withdraw"

  system("clear");
  printf("\t\t====== Make Transaction for %s =====\n\n", u.name);
  printf("\n\t\tEnter the account number for the transaction: ");
  scanf("%d", &accountNbr);

  struct Record records[MAX_RECORDS];
  int recordCount = 0;

  FILE *pf = fopen(RECORDS, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    return;
  }

  while (getAccountFromFile(pf, records[recordCount].name,
                            &records[recordCount])) {
    recordCount++;
  }
  fclose(pf);

  int found = 0;
  for (int i = 0; i < recordCount; i++) {
    if (records[i].accountNbr == accountNbr &&
        strcmp(records[i].name, u.name) == 0) {
      if (strcmp(records[i].accountType, "fixed01") == 0 ||
          strcmp(records[i].accountType, "fixed02") == 0 ||
          strcmp(records[i].accountType, "fixed03") == 0) {
        printf("\n\t\t✖ Transactions are not allowed for fixed accounts!\n");
        stayOrReturn(0, makeTransaction, u);
        return;
      }

      printf("\n\t\tEnter the transaction type (deposit/withdraw): ");
      scanf("%s", transactionType);
      printf("\n\t\tEnter the amount: ");
      scanf("%lf", &amount);

      if (strcmp(transactionType, "withdraw") == 0) {
        if (records[i].amount < amount) {
          printf("\n\t\t✖ Not enough balance for withdrawal.\n");
          stayOrReturn(0, makeTransaction, u);
          return;
        }
        records[i].amount -= amount;
      } else if (strcmp(transactionType, "deposit") == 0) {
        records[i].amount += amount;
      } else {
        printf("\n\t\t✖ Invalid transaction type.\n");
        stayOrReturn(0, makeTransaction, u);
        return;
      }

      found = 1;
      break;
    }
  }

  pf = fopen(RECORDS, "w");
  if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    return;
  }

  for (int i = 0; i < recordCount; i++) {
    updateUserAccountInFile(pf, records[i]);
  }
  fclose(pf);

  if (found) {
    printf("\n\t\t✔ Transaction successful.\n");
    success(u);
  } else {
    printf("\n\t\t✖ No account found with account number %d.\n", accountNbr);
    stayOrReturn(0, makeTransaction, u);
  }
}

void removeAccount(struct User u) {
  int accountNbr;
  system("clear");
  printf("\t\t====== Remove Account for %s =====\n\n", u.name);
  printf("\n\t\tEnter the account number you wish to remove: ");
  scanf("%d", &accountNbr);

  struct Record records[MAX_RECORDS];
  int recordCount = 0;

  FILE *pf = fopen(RECORDS, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    exit(1);
  }

  while (getAccountFromFile(pf, records[recordCount].name,
                            &records[recordCount])) {
    recordCount++;
  }
  fclose(pf);

  int found = 0;
  for (int i = 0; i < recordCount; i++) {
    if (records[i].accountNbr == accountNbr &&
        strcmp(records[i].name, u.name) == 0) {
      found = 1;
      for (int j = i; j < recordCount - 1; j++) {
        records[j] = records[j + 1];
      }
      recordCount--;
      break;
    }
  }

  if (!found) {
    printf("\n\t\tNo account found with account number %d.\n", accountNbr);
    stayOrReturn(0, removeAccount, u);
    return;
  }

  pf = fopen(RECORDS, "w");
  if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    exit(1);
  }

  int newId = 0;
  for (int i = 0; i < recordCount; i++) {
    records[i].id = newId++;
    updateUserAccountInFile(pf, records[i]);
  }
  fclose(pf);

  printf("\n\t\tAccount removed successfully.\n");
  success(u);
}
