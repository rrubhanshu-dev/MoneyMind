#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "transactions.txt"

typedef struct {
    int id;
    char type[12];
    char date[11];
    char category[30];
    char note[80];
    double amount;
} Transaction;

void clearInput(void);
int loadData(Transaction **items);
void saveData(Transaction *items, int count);
void addTransaction(void);
void listTransactions(void);
void searchTransaction(void);
void editTransaction(void);
void deleteTransaction(void);
void summary(void);
void categoryReport(void);
void setBudget(void);
void checkBudget(void);
void exportCSV(void);
void importCSV(void);
void sortByAmount(void);
void normalizeType(char type[]);
int normalizeDate(char date[]);

int main(void) {
    int choice;

    do {
        printf("\n\033[1;35m========================================\033[0m\n");
        printf("\033[1;36m       MoneyMind ADVANCED - C\033[0m\n");
        printf("\033[1;35m========================================\033[0m\n");
        printf("1. Add income/expense\n");
        printf("2. View all transactions\n");
        printf("3. Search transaction by ID\n");
        printf("4. Edit transaction amount\n");
        printf("5. Delete transaction\n");
        printf("6. Financial summary\n");
        printf("7. Category report\n");
        printf("8. Set monthly budget\n");
        printf("9. Check budget\n");
        printf("10. Sort by amount\n");
        printf("11. Export CSV for Excel\n");
        printf("12. Import CSV from Excel\n");
        printf("0. Exit\n");
        printf("Choose: ");

        if (scanf("%d", &choice) != 1) {
            clearInput();
            choice = -1;
        }

        switch (choice) {
            case 1: addTransaction(); break;
            case 2: listTransactions(); break;
            case 3: searchTransaction(); break;
            case 4: editTransaction(); break;
            case 5: deleteTransaction(); break;
            case 6: summary(); break;
            case 7: categoryReport(); break;
            case 8: setBudget(); break;
            case 9: checkBudget(); break;
            case 10: sortByAmount(); break;
            case 11: exportCSV(); break;
            case 12: importCSV(); break;
            case 0: printf("Your data remains saved in files.\n"); break;
            default: printf("\033[1;31mInvalid choice.\033[0m\n");
        }
    } while (choice != 0);

    return 0;
}

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void normalizeType(char type[]) {
    int i;
    for (i = 0; type[i] != '\0'; i++) {
        if (type[i] >= 'A' && type[i] <= 'Z')
            type[i] = (char)(type[i] - 'A' + 'a');
    }
}

int normalizeDate(char date[]) {
    int a, b, c;
    char result[11];

    if (sscanf(date, "%d-%d-%d", &a, &b, &c) != 3)
        return 0;

    if (a >= 1900 && a <= 9999 && b >= 1 && b <= 12 && c >= 1 && c <= 31) {
        snprintf(result, sizeof(result), "%04d-%02d-%02d", a, b, c);
    } else if (a >= 1 && a <= 31 && b >= 1 && b <= 12 && c >= 1900 && c <= 9999) {
        snprintf(result, sizeof(result), "%04d-%02d-%02d", c, b, a);
    } else {
        return 0;
    }

    strcpy(date, result);
    return 1;
}

int loadData(Transaction **items) {
    FILE *fp = fopen(DATA_FILE, "r");
    Transaction t, *data = NULL, *temp;
    int count = 0, capacity = 0;

    *items = NULL;
    if (fp == NULL)
        return 0;

    while (fscanf(fp, "%d|%11[^|]|%10[^|]|%29[^|]|%79[^|]|%lf\n",
                  &t.id, t.type, t.date, t.category, t.note, &t.amount) == 6) {
        if (count == capacity) {
            capacity = capacity == 0 ? 10 : capacity * 2;
            temp = realloc(data, (size_t)capacity * sizeof(Transaction));
            if (temp == NULL) {
                free(data);
                fclose(fp);
                return 0;
            }
            data = temp;
        }
        data[count++] = t;
    }

    fclose(fp);
    *items = data;
    return count;
}

void saveData(Transaction *items, int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    int i;

    if (fp == NULL) {
        printf("Could not save transactions.txt\n");
        return;
    }

    for (i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%s|%s|%.2f\n",
                items[i].id, items[i].type, items[i].date,
                items[i].category, items[i].note, items[i].amount);
    }

    fclose(fp);
}

void addTransaction(void) {
    Transaction *items, t, *temp;
    int count = loadData(&items), i, maxID = 0;

    printf("Type (income/expense): ");
    scanf("%11s", t.type);
    normalizeType(t.type);
    if (strcmp(t.type, "income") != 0 && strcmp(t.type, "expense") != 0) {
        printf("Enter income or expense.\n");
        free(items);
        return;
    }

    printf("Date (YYYY-MM-DD or DD-MM-YYYY): ");
    scanf("%10s", t.date);
    if (!normalizeDate(t.date)) {
        printf("Invalid date format.\n");
        free(items);
        return;
    }

    printf("Category (one word, no | symbol): ");
    scanf("%29s", t.category);
    printf("Note (one word, no | symbol): ");
    scanf("%79s", t.note);
    printf("Amount: ");
    if (scanf("%lf", &t.amount) != 1 || t.amount <= 0) {
        printf("Enter a positive amount.\n");
        clearInput();
        free(items);
        return;
    }

    for (i = 0; i < count; i++)
        if (items[i].id > maxID) maxID = items[i].id;

    t.id = maxID + 1;
    temp = realloc(items, (size_t)(count + 1) * sizeof(Transaction));
    if (temp == NULL) {
        printf("Not enough memory.\n");
        free(items);
        return;
    }

    items = temp;
    items[count++] = t;
    saveData(items, count);
    printf("\033[1;32mTransaction saved. ID: %d\033[0m\n", t.id);
    free(items);
}

void listTransactions(void) {
    Transaction *items;
    int count = loadData(&items), i;

    printf("\nID  TYPE       DATE        CATEGORY       NOTE                         AMOUNT\n");
    printf("-------------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++)
        printf("%-3d %-10s %-11s %-14s %-27s %.2f\n",
               items[i].id, items[i].type, items[i].date,
               items[i].category, items[i].note, items[i].amount);

    if (count == 0) printf("No transactions found.\n");
    free(items);
}

void searchTransaction(void) {
    Transaction *items;
    int count = loadData(&items), id, i;

    printf("Enter transaction ID: ");
    if (scanf("%d", &id) != 1) {
        clearInput();
        free(items);
        printf("Invalid ID.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        if (items[i].id == id) {
            printf("ID: %d\nType: %s\nDate: %s\nCategory: %s\nNote: %s\nAmount: %.2f\n",
                   items[i].id, items[i].type, items[i].date,
                   items[i].category, items[i].note, items[i].amount);
            free(items);
            return;
        }
    }

    printf("Transaction not found.\n");
    free(items);
}

void editTransaction(void) {
    Transaction *items;
    int count = loadData(&items), id, i;
    double amount;

    printf("Enter ID to edit: ");
    if (scanf("%d", &id) != 1) {
        clearInput();
        free(items);
        printf("Invalid ID.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        if (items[i].id == id) {
            printf("Current amount: %.2f\nNew amount: ", items[i].amount);
            if (scanf("%lf", &amount) == 1 && amount > 0) {
                items[i].amount = amount;
                saveData(items, count);
                printf("Transaction updated.\n");
            } else {
                printf("Invalid amount.\n");
                clearInput();
            }
            free(items);
            return;
        }
    }

    printf("Transaction not found.\n");
    free(items);
}

void deleteTransaction(void) {
    Transaction *items;
    int count = loadData(&items), id, i, j;

    printf("Enter ID to delete: ");
    if (scanf("%d", &id) != 1) {
        clearInput();
        free(items);
        printf("Invalid ID.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        if (items[i].id == id) {
            for (j = i; j < count - 1; j++)
                items[j] = items[j + 1];
            saveData(items, count - 1);
            printf("Transaction deleted.\n");
            free(items);
            return;
        }
    }

    printf("Transaction not found.\n");
    free(items);
}

void summary(void) {
    Transaction *items;
    int count = loadData(&items), i;
    double income = 0, expense = 0;

    for (i = 0; i < count; i++) {
        if (strcmp(items[i].type, "income") == 0)
            income += items[i].amount;
        else
            expense += items[i].amount;
    }

    printf("\nTotal income : %.2f\n", income);
    printf("Total expense: %.2f\n", expense);
    printf("Balance      : %.2f\n", income - expense);
    free(items);
}

void categoryReport(void) {
    Transaction *items;
    int count = loadData(&items), i, j, found;
    double total;

    printf("\nExpense by category\n");
    for (i = 0; i < count; i++) {
        if (strcmp(items[i].type, "expense") != 0)
            continue;

        found = 0;
        for (j = 0; j < i; j++) {
            if (strcmp(items[j].type, "expense") == 0 &&
                strcmp(items[j].category, items[i].category) == 0) {
                found = 1;
                break;
            }
        }
        if (found) continue;

        total = 0;
        for (j = 0; j < count; j++) {
            if (strcmp(items[j].type, "expense") == 0 &&
                strcmp(items[j].category, items[i].category) == 0)
                total += items[j].amount;
        }

        printf("%-20s %.2f\n", items[i].category, total);
    }

    free(items);
}

void setBudget(void) {
    FILE *fp = fopen("budget.txt", "w");
    double budget;

    if (fp == NULL) {
        printf("Could not open budget.txt\n");
        return;
    }

    printf("Enter monthly budget: ");
    if (scanf("%lf", &budget) != 1 || budget < 0) {
        printf("Invalid budget.\n");
        clearInput();
        fclose(fp);
        return;
    }

    fprintf(fp, "%.2f\n", budget);
    fclose(fp);
    printf("Budget saved.\n");
}

void checkBudget(void) {
    FILE *fp = fopen("budget.txt", "r");
    Transaction *items;
    int count, i;
    double budget, expenses = 0;

    if (fp == NULL || fscanf(fp, "%lf", &budget) != 1) {
        printf("Set a budget first.\n");
        if (fp != NULL) fclose(fp);
        return;
    }
    fclose(fp);

    count = loadData(&items);
    for (i = 0; i < count; i++)
        if (strcmp(items[i].type, "expense") == 0)
            expenses += items[i].amount;

    printf("Budget: %.2f\nExpenses: %.2f\nRemaining: %.2f\n",
           budget, expenses, budget - expenses);

    if (expenses > budget)
        printf("\033[1;31mBudget exceeded!\033[0m\n");
    else
        printf("\033[1;32mYou are within budget.\033[0m\n");

    free(items);
}

void sortByAmount(void) {
    Transaction *items, temp;
    int count = loadData(&items), i, j;

    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (items[j].amount > items[j + 1].amount) {
                temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }

    printf("\nTransactions sorted by amount\n");
    for (i = 0; i < count; i++)
        printf("%d %-10s %-14s %.2f\n", items[i].id, items[i].type,
               items[i].category, items[i].amount);

    free(items);
}

void exportCSV(void) {
    Transaction *items;
    int count = loadData(&items), i;
    FILE *fp = fopen("transactions.csv", "w");

    if (fp == NULL) {
        printf("Could not create transactions.csv\n");
        free(items);
        return;
    }

    fprintf(fp, "ID,Type,Date,Category,Note,Amount\n");
    for (i = 0; i < count; i++)
        fprintf(fp, "%d,%s,%s,%s,%s,%.2f\n",
                items[i].id, items[i].type, items[i].date,
                items[i].category, items[i].note, items[i].amount);

    fclose(fp);
    free(items);
    printf("Exported to transactions.csv. Open it in Excel.\n");
}

void importCSV(void) {
    char filename[260], line[512];
    char type[12], date[30], category[30], note[80];
    Transaction *items, t, *temp;
    int count = loadData(&items), imported = 0, skipped = 0, i, maxID = 0;
    double amount;
    FILE *fp;

    printf("CSV filename or full path: ");
    scanf(" %259[^\n]", filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("\033[1;31mCannot open file: %s\033[0m\n", filename);
        printf("Place the CSV beside moneymind.exe or enter its full path.\n");
        free(items);
        return;
    }

    for (i = 0; i < count; i++)
        if (items[i].id > maxID) maxID = items[i].id;

    if (fgets(line, sizeof(line), fp) == NULL) {
        printf("CSV file is empty.\n");
        fclose(fp);
        free(items);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        int id, fields;
        size_t len = strlen(line);

        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[--len] = '\0';
        if (line[0] == '\0') continue;

        fields = sscanf(line, "%d,%11[^,],%29[^,],%29[^,],%79[^,],%lf",
                        &id, type, date, category, note, &amount);

        if (fields != 6) {
            skipped++;
            continue;
        }

        normalizeType(type);
        if (strcmp(type, "income") != 0 && strcmp(type, "expense") != 0) {
            skipped++;
            continue;
        }

        if (!normalizeDate(date) || amount <= 0) {
            skipped++;
            continue;
        }

        t.id = id;
        for (i = 0; i < count; i++)
            if (items[i].id == t.id) break;
        if (i < count) t.id = ++maxID;
        if (t.id > maxID) maxID = t.id;

        strcpy(t.type, type);
        strcpy(t.date, date);
        strcpy(t.category, category);
        strcpy(t.note, note);
        t.amount = amount;

        temp = realloc(items, (size_t)(count + 1) * sizeof(Transaction));
        if (temp == NULL) {
            printf("Not enough memory; import stopped.\n");
            skipped++;
            break;
        }

        items = temp;
        items[count++] = t;
        imported++;
    }

    fclose(fp);
    saveData(items, count);
    free(items);

    printf("\033[1;32mImported: %d\033[0m\n", imported);
    printf("\033[1;33mSkipped: %d\033[0m\n", skipped);
    printf("Imported records saved in transactions.txt\n");
}
