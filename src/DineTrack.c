
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITEM_NAME     50
#define MAX_CUSTOMER_NAME 50
#define MAX_DISH_FIELD    800
#define BILLING_CSV       "billing.csv"


typedef struct menuItem {
    int id;
    char name[MAX_ITEM_NAME];
    int price;
    int qty; // available stock
    struct menuItem *next;
} MENUITEM;

MENUITEM *menuHead = NULL;

void addMenuItemSorted(int id, const char *name, int price, int qty) {
    MENUITEM *prev = NULL, *cur = menuHead;
    while (cur && cur->id < id) { prev = cur; cur = cur->next; }
    if (cur && cur->id == id) {
        strncpy(cur->name, name, MAX_ITEM_NAME-1); cur->name[MAX_ITEM_NAME-1] = '\0';
        cur->price = price; cur->qty = qty;
        return;
    }
    MENUITEM *n = (MENUITEM *)malloc(sizeof(MENUITEM));
    n->id = id;
    strncpy(n->name, name, MAX_ITEM_NAME-1); n->name[MAX_ITEM_NAME-1] = '\0';
    n->price = price; n->qty = qty;
    n->next = cur;
    if (!prev) menuHead = n; else prev->next = n;
}

void initMenu15() {
    addMenuItemSorted(1, "Dosa", 40, 30);
    addMenuItemSorted(2, "Idli", 30, 50);
    addMenuItemSorted(3, "Pongal", 50, 20);
    addMenuItemSorted(4, "Tea", 15, 100);
    addMenuItemSorted(5, "Coffee", 20, 100);
    addMenuItemSorted(6, "Meals", 80, 25);
    addMenuItemSorted(7, "Poori", 45, 20);
    addMenuItemSorted(8, "Chapathi", 35, 40);
    addMenuItemSorted(9, "Fried Rice", 90, 15);
    addMenuItemSorted(10, "Noodles", 85, 15);
    addMenuItemSorted(11, "Parotta", 25, 50);
    addMenuItemSorted(12, "Upma", 30, 30);
    addMenuItemSorted(13, "Bonda", 20, 60);
    addMenuItemSorted(14, "Vada", 15, 60);
    addMenuItemSorted(15, "Juice", 60, 25);
}

void displayMenuOnce() {
    printf("\n-------------------- MENU --------------------\n");
    MENUITEM *t = menuHead;
    while (t) {
        printf("%2d. %-18s Rs %4d (Stock: %3d)\n", t->id, t->name, t->price, t->qty);
        t = t->next;
    }
    printf(" 0. Finish adding items\n");
    printf("----------------------------------------------\n");
}

MENUITEM* findMenuById(int id) {
    MENUITEM *t = menuHead;
    while (t) {
        if (t->id == id) return t;
        t = t->next;
    }
    return NULL;
}

typedef struct order {
    int order_id;
    char customerName[MAX_CUSTOMER_NAME];
    char items[MAX_DISH_FIELD]; 
    int totalAmount;
    int isVIP; 
    struct order *left, *right;
} ORDER;

ORDER *ordersHead = NULL;

typedef struct historyNode {
    int order_id;
    char customerName[MAX_CUSTOMER_NAME];
    char items[MAX_DISH_FIELD];
    int totalAmount;
    char status[20];
    char timeStamp[64];
    struct historyNode *next;
} HISTORY;
HISTORY *historyTop = NULL;

typedef struct waitingCustomer {
    char name[MAX_CUSTOMER_NAME];
    struct waitingCustomer *next;
} QUEUE;
QUEUE *qFront = NULL, *qRear = NULL;

int generateOrderID() {
    static int id = 100;
    return ++id;
}

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void pushHistory(int id, const char *cust, const char *items, int total, const char *status) {
    HISTORY *h = (HISTORY *)malloc(sizeof(HISTORY));
    if (!h) return;
    h->order_id = id;
    strncpy(h->customerName, cust, MAX_CUSTOMER_NAME-1); h->customerName[MAX_CUSTOMER_NAME-1] = '\0';
    strncpy(h->items, items, MAX_DISH_FIELD-1); h->items[MAX_DISH_FIELD-1] = '\0';
    h->totalAmount = total;
    strncpy(h->status, status, sizeof(h->status)-1); h->status[sizeof(h->status)-1] = '\0';
    time_t now = time(NULL);
    char *ts = ctime(&now);
    if (ts) {
        size_t len = strlen(ts); if (len && ts[len-1] == '\n') ts[len-1] = '\0';
        strncpy(h->timeStamp, ts, sizeof(h->timeStamp)-1); h->timeStamp[sizeof(h->timeStamp)-1] = '\0';
    } else strcpy(h->timeStamp, "unknown");
    h->next = historyTop;
    historyTop = h;
}

void enqueueWaiting(const char *cust, int vipFlag) {
    QUEUE *n = (QUEUE *)malloc(sizeof(QUEUE));
    if (!n) { printf("Memory error.\n"); return; }
    strncpy(n->name, cust, MAX_CUSTOMER_NAME-1); n->name[MAX_CUSTOMER_NAME-1] = '\0';
    n->next = NULL;
    if (vipFlag) {
        if (!qFront) { qFront = qRear = n; }
        else { n->next = qFront; qFront = n; }
        printf("VIP customer '%s' added to the front of waiting queue.\n", n->name);
    } else {
        if (!qRear) qFront = qRear = n;
        else { qRear->next = n; qRear = n; }
        printf("Customer '%s' added to waiting queue.\n", n->name);
    }
}

void enqueueCustomerInteractive() {
    char name[MAX_CUSTOMER_NAME];
    clearInput();
    printf("Enter Customer Name: ");
    fgets(name, MAX_CUSTOMER_NAME, stdin);
    name[strcspn(name, "\n")] = '\0';
    char ch; int vip = 0;
    printf("VIP? (y/n): ");
    if (scanf(" %c", &ch) == 1) { if (ch == 'y' || ch == 'Y') vip = 1; }
    enqueueWaiting(name, vip);
}

void dequeueCustomer() {
    if (!qFront) { printf("Waiting queue empty.\n"); return; }
    QUEUE *t = qFront;
    printf("Calling customer: %s\n", t->name);
    qFront = qFront->next;
    if (!qFront) qRear = NULL;
    free(t);
}

void displayQueue() {
    if (!qFront) { printf("Waiting queue empty.\n"); return; }
    printf("\n-- Waiting Queue --\n");
    int i = 1;
    QUEUE *t = qFront;
    while (t) { printf("%d. %s\n", i++, t->name); t = t->next; }
    printf("-------------------\n");
}

ORDER* createOrderNode(const char *cust, int vipFlag, const char *itemsStr) {
    ORDER *n = (ORDER *)malloc(sizeof(ORDER));
    if (!n) return NULL;
    n->order_id = generateOrderID();
    strncpy(n->customerName, cust, MAX_CUSTOMER_NAME-1); n->customerName[MAX_CUSTOMER_NAME-1] = '\0';
    strncpy(n->items, itemsStr, MAX_DISH_FIELD-1); n->items[MAX_DISH_FIELD-1] = '\0';
    n->totalAmount = 0; 
    n->isVIP = vipFlag ? 1 : 0;
    n->left = n->right = NULL;
    return n;
}

ORDER* insertOrder(ORDER *head, ORDER *newnode, int pos) {
    if (!newnode) return head;
    if (!head) { newnode->left = newnode->right = newnode; return newnode; }
    ORDER *last = head->left;
    if (newnode->isVIP || pos == 1) {
        newnode->right = head;
        newnode->left = last;
        last->right = newnode;
        head->left = newnode;
        return newnode; 
    }
    last->right = newnode;
    newnode->left = last;
    newnode->right = head;
    head->left = newnode;
    return head;
}

ORDER* findOrderByID(ORDER *head, int id) {
    if (!head) return NULL;
    ORDER *t = head;
    do {
        if (t->order_id == id) return t;
        t = t->right;
    } while (t != head);
    return NULL;
}

ORDER* removeFrontOrder(ORDER *head) {
    if (!head) { printf("No active orders.\n"); return NULL; }
    ORDER *t = head;
    printf("Serving Order ID %d | %s | Items: %s\n", t->order_id, t->customerName, t->items);

    if (t->totalAmount == 0) {
    }

    pushHistory(t->order_id, t->customerName, t->items, t->totalAmount, "Served");
    if (t->right == t) { free(t); return NULL; }
    ORDER *newhead = t->right;
    t->left->right = t->right;
    t->right->left = t->left;
    free(t);
    return newhead;
}

ORDER* removeOrderByID(ORDER *head, int id, const char *status) {
    if (!head) { printf("No active orders.\n"); return NULL; }
    ORDER *t = head, *found = NULL;
    do {
        if (t->order_id == id) { found = t; break; }
        t = t->right;
    } while (t != head);
    if (!found) { printf("Order ID %d not found.\n", id); return head; }
    pushHistory(found->order_id, found->customerName, found->items, found->totalAmount, status);
    if (found->right == found) { free(found); return NULL; }
    ORDER *newhead = head;
    if (found == head) newhead = found->right;
    found->left->right = found->right;
    found->right->left = found->left;
    free(found);
    return newhead;
}

void displayOrders() {
    if (!ordersHead) { printf("\nNo active orders.\n"); return; }
    printf("\n---- Active Orders ----\n");
    ORDER *t = ordersHead;
    do {
        printf("ID:%4d | %-15s | VIP:%-3s | Items: %-40s\n",
               t->order_id, t->customerName, t->isVIP ? "YES" : "NO", t->items);
        t = t->right;
    } while (t != ordersHead);
    printf("------------------------\n");
}

void saveBillCSV_oneLine(int id, const char *cust, const char *items, int total) {
    int needHeader = 0;
    FILE *fr = fopen(BILLING_CSV, "r");
    if (!fr) needHeader = 1; else fclose(fr);
    FILE *fp = fopen(BILLING_CSV, "a");
    if (!fp) { printf("Error opening CSV for writing.\n"); return; }
    if (needHeader) fprintf(fp, "OrderID,Customer,Items,TotalAmount\n");

    char custSafe[MAX_CUSTOMER_NAME]; strncpy(custSafe, cust, MAX_CUSTOMER_NAME-1); custSafe[MAX_CUSTOMER_NAME-1] = '\0';
    for (char *p = custSafe; *p; ++p) if (*p == ',') *p = ';';
    char itemsSafe[MAX_DISH_FIELD]; strncpy(itemsSafe, items, MAX_DISH_FIELD-1); itemsSafe[MAX_DISH_FIELD-1] = '\0';
    for (char *p = itemsSafe; *p; ++p) if (*p == ',') *p = ';';
    fprintf(fp, "%d,%s,%s,%d\n", id, custSafe, itemsSafe, total);
    fclose(fp);
}

int addItemsToBuffer(char *buffer) {
    int anyAdded = 0;
    while (1) {
        int choice;
        printf("Select Menu Item ID to add (0 to finish): ");
        if (scanf("%d", &choice) != 1) { printf("Invalid input.\n"); clearInput(); continue; }
        if (choice == 0) break;
        MENUITEM *mi = findMenuById(choice);
        if (!mi) { printf("Invalid Menu ID. Try again.\n"); continue; }
        if (mi->qty <= 0) { printf("Item '%s' is OUT OF STOCK.\n", mi->name); continue; }
        int q;
        printf("Enter quantity for %s (available %d): ", mi->name, mi->qty);
        if (scanf("%d", &q) != 1) { printf("Invalid qty.\n"); clearInput(); continue; }
        if (q <= 0) { printf("Quantity must be positive.\n"); continue; }
        if (q > mi->qty) { printf("Only %d available for %s. Try smaller qty.\n", mi->qty, mi->name); continue; }

        mi->qty -= q;
        char tmp[128];
        if (strlen(buffer) > 0) strncat(buffer, ",", MAX_DISH_FIELD - strlen(buffer) - 1);
        snprintf(tmp, sizeof(tmp), "%s(%d)", mi->name, q);
        strncat(buffer, tmp, MAX_DISH_FIELD - strlen(buffer) - 1);
        anyAdded = 1;
        printf("Added %s x %d to order.\n", mi->name, q);
    }
    return anyAdded;
}


void placeOrderFlow() {
    clearInput();
    char cust[MAX_CUSTOMER_NAME];
    printf("Enter Customer Name: ");
    fgets(cust, MAX_CUSTOMER_NAME, stdin);
    cust[strcspn(cust, "\n")] = '\0';

    char vipch; int vipFlag = 0;
    printf("Is this a VIP order? (y/n): ");
    if (scanf(" %c", &vipch) == 1) { if (vipch == 'y' || vipch == 'Y') vipFlag = 1; }

    displayMenuOnce();

    char itemsBuf[MAX_DISH_FIELD]; itemsBuf[0] = '\0';
    int added = addItemsToBuffer(itemsBuf);
    if (!added) { printf("No items added. Cancelling order.\n"); return; }

    ORDER *node = createOrderNode(cust, vipFlag, itemsBuf);
    if (vipFlag) ordersHead = insertOrder(ordersHead, node, 1); else ordersHead = insertOrder(ordersHead, node, 9999);

    printf("Order placed: ID %d | Customer %s | VIP:%s | Items: %s\n",
           node->order_id, node->customerName, node->isVIP ? "YES" : "NO", node->items);
}

int calculateBillFromItems(const char *itemsStr) {
    if (!itemsStr || strlen(itemsStr) == 0) return 0;
    int total = 0;
    char copy[MAX_DISH_FIELD];
    strncpy(copy, itemsStr, MAX_DISH_FIELD-1); copy[MAX_DISH_FIELD-1] = '\0';
    char *token = strtok(copy, ",");
    while (token) {
        char namePart[MAX_ITEM_NAME]; int qty = 0;
        char *popen = strchr(token, '(');
        char *pclose = strchr(token, ')');
        if (popen && pclose && popen < pclose) {
            size_t nlen = popen - token;
            while (nlen > 0 && token[nlen-1] == ' ') nlen--;
            strncpy(namePart, token, nlen); namePart[nlen] = '\0';
            char qstr[16] = {0};
            size_t len = pclose - popen - 1;
            if (len >= sizeof(qstr)) len = sizeof(qstr)-1;
            strncpy(qstr, popen+1, len); qstr[len] = '\0';
            qty = atoi(qstr);
        } else {
            strncpy(namePart, token, MAX_ITEM_NAME-1); namePart[MAX_ITEM_NAME-1] = '\0';
            qty = 1;
        }
        char *s = namePart; while (*s == ' ') s++;
        char nameTrim[MAX_ITEM_NAME]; strncpy(nameTrim, s, MAX_ITEM_NAME-1); nameTrim[MAX_ITEM_NAME-1] = '\0';
        MENUITEM *m = menuHead;
        while (m) {
            if (strcmp(m->name, nameTrim) == 0) { total += qty * m->price; break; }
            m = m->next;
        }
        token = strtok(NULL, ",");
    }
    return total;
}

void generateBillFlow() {
    if (!ordersHead) { printf("No active orders.\n"); return; }
    int id;
    printf("Enter Order ID to generate bill: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); clearInput(); return; }
    ORDER *o = findOrderByID(ordersHead, id);
    if (!o) { printf("Order not found.\n"); return; }

    while (1) {
        char ch; printf("Extend order before billing? (y/n): ");
        if (scanf(" %c", &ch) != 1) { clearInput(); break; }
        if (ch == 'y' || ch == 'Y') {
            displayMenuOnce();
            int added = addItemsToBuffer(o->items);
            if (added) printf("Order updated: %s\n", o->items);
        } else break;
    }

    int total = calculateBillFromItems(o->items);
    o->totalAmount = total;
    printf("\n----- FINAL BILL -----\nOrder ID: %d\nCustomer: %s\nItems: %s\nTotal Amount: Rs %d\n----------------------\n",
           o->order_id, o->customerName, o->items, o->totalAmount);

    saveBillCSV_oneLine(o->order_id, o->customerName, o->items, o->totalAmount);
    pushHistory(o->order_id, o->customerName, o->items, o->totalAmount, "Billed");
}

void extendOrderByID_main() {
    if (!ordersHead) { printf("No active orders.\n"); return; }
    int id;
    printf("Enter Order ID to extend: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); clearInput(); return; }
    ORDER *o = findOrderByID(ordersHead, id);
    if (!o) { printf("Order not found.\n"); return; }
    displayMenuOnce();
    int added = addItemsToBuffer(o->items);
    if (added) {
        printf("Order extended. New items: %s\n", o->items);
        pushHistory(o->order_id, o->customerName, o->items, o->totalAmount, "Extended");
    } else printf("No items were added.\n");
}

void serveFrontOrder() {
    ordersHead = removeFrontOrder(ordersHead);
}

void removeOrderByID_main() {
    if (!ordersHead) { printf("No active orders.\n"); return; }
    int id; printf("Enter Order ID to remove/cancel: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); clearInput(); return; }
    ordersHead = removeOrderByID(ordersHead, id, "Cancelled");
}

void showHistory() {
    if (!historyTop) { printf("\nNo history.\n"); return; }
    printf("\n-- History (most recent first) --\n");
    HISTORY *h = historyTop;
    while (h) {
        printf("ID:%d | %s | %s | Rs %d | %s | %s\n",
               h->order_id, h->customerName, h->items, h->totalAmount, h->status, h->timeStamp);
        h = h->next;
    }
    printf("---------------------------------\n");
}

void showMenuStocks() { displayMenuOnce(); }

void initAll() { initMenu15(); }

int main() {
    initAll();
    int choice;
    while (1) {
        printf("\n\n--- WELCOME TO DINETRACK ---\n");
        printf("1  -> Display Menu & Stock\n");
        printf("2  -> Add Order (asks VIP -> front if VIP else append)\n");
        printf("3  -> Show Active Orders\n");
        printf("4  -> Show History (timestamps)\n");
        printf("5  -> Extend an existing order (no billing)\n");
        printf("6  -> Generate Bill for an Order (asks to extend before billing)\n");
        printf("7  -> Serve Front Order (mark served)\n");
        printf("8  -> Remove Order by ID (cancel)\n");
        printf("9  -> Add Customer to Waiting Queue\n");
        printf("10 -> Call Customer from Waiting Queue\n");
        printf("11 -> View Waiting Queue\n");
        printf("0  -> Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clearInput();
            continue;
        }

        switch (choice) {
            case 1: showMenuStocks(); break;
            case 2: placeOrderFlow(); break;
            case 3: displayOrders(); break;
            case 4: showHistory(); break;
            case 5: extendOrderByID_main(); break;
            case 6: generateBillFlow(); break;
            case 7: serveFrontOrder(); break;
            case 8: removeOrderByID_main(); break;
            case 9: enqueueCustomerInteractive(); break;
            case 10: dequeueCustomer(); break;
            case 11: displayQueue(); break;
            case 0: printf("Exiting. Goodbye!\n"); exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}