#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50
#define BATCH_LEN 30
#define DATE_LEN 20
#define PHONE_LEN 20
#define EMAIL_LEN 100
#define ACTION_LEN 100
#define MAX_ORDER_ITEMS 20
#define URGENT_EXTRA_CHARGE 500.0f

/* =========================================================
   HELPERS
   ========================================================= */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void printLine(void) {
    printf("====================================================================\n");
}

void printTitle(const char *title) {
    printf("\n");
    printLine();
    printf("%s\n", title);
    printLine();
}

int getIntInput(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clearInputBuffer();
            return value;
        }
        printf("Invalid input. Enter a valid whole number.\n");
        clearInputBuffer();
    }
}

float getFloatInput(const char *prompt) {
    float value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &value) == 1) {
            clearInputBuffer();
            return value;
        }
        printf("Invalid input. Enter a valid number.\n");
        clearInputBuffer();
    }
}

void getStringInput(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void pressEnterToContinue(void) {
    printf("\nPress Enter to continue...");
    getchar();
}

int askSaveChanges(void) {
    char ch;
    printf("Save changes? (Y/N): ");
    scanf(" %c", &ch);
    clearInputBuffer();
    return (ch == 'Y' || ch == 'y');
}

/* =========================================================
   INVENTORY - DYNAMIC ARRAY
   ========================================================= */
typedef struct {
    int id;
    char name[NAME_LEN];
    char batch[BATCH_LEN];
    char expiry[DATE_LEN];
    float price;
    int quantity;
} Medicine;

typedef struct {
    Medicine *items;
    int size;
    int capacity;
} Inventory;

void initInventory(Inventory *inv) {
    inv->size = 0;
    inv->capacity = 5;
    inv->items = (Medicine *)malloc(inv->capacity * sizeof(Medicine));
    if (inv->items == NULL) {
        printf("Memory allocation failed for inventory.\n");
        exit(1);
    }
}

void freeInventory(Inventory *inv) {
    free(inv->items);
    inv->items = NULL;
    inv->size = 0;
    inv->capacity = 0;
}

int findMedicineIndexById(const Inventory *inv, int id) {
    for (int i = 0; i < inv->size; i++) {
        if (inv->items[i].id == id) return i;
    }
    return -1;
}

void addMedicine(Inventory *inv, Medicine med) {
    if (inv->size == inv->capacity) {
        inv->capacity *= 2;
        Medicine *temp = (Medicine *)realloc(inv->items, inv->capacity * sizeof(Medicine));
        if (temp == NULL) {
            printf("Memory reallocation failed.\n");
            exit(1);
        }
        inv->items = temp;
    }
    inv->items[inv->size++] = med;
}

void updateMedicine(Inventory *inv, int id, float newPrice, int newQty) {
    int index = findMedicineIndexById(inv, id);
    if (index == -1) {
        printf("Medicine ID not found.\n");
        return;
    }
    inv->items[index].price = newPrice;
    inv->items[index].quantity = newQty;
}

void deleteMedicine(Inventory *inv, int id) {
    int index = findMedicineIndexById(inv, id);
    if (index == -1) {
        printf("Medicine ID not found.\n");
        return;
    }
    for (int i = index; i < inv->size - 1; i++) {
        inv->items[i] = inv->items[i + 1];
    }
    inv->size--;
}

void displayInventory(const Inventory *inv) {
    printTitle("INVENTORY DETAILS");
    if (inv->size == 0) {
        printf("No medicines in inventory.\n");
        return;
    }

    printf("%-5s %-18s %-12s %-12s %-10s %-8s\n", "ID", "Name", "Batch", "Expiry", "Price", "Qty");
    printLine();

    for (int i = 0; i < inv->size; i++) {
        printf("%-5d %-18s %-12s %-12s %-10.2f %-8d\n",
               inv->items[i].id,
               inv->items[i].name,
               inv->items[i].batch,
               inv->items[i].expiry,
               inv->items[i].price,
               inv->items[i].quantity);
    }
}

int reduceMedicineStock(Inventory *inv, int id, int qty) {
    int index = findMedicineIndexById(inv, id);
    if (index == -1) return 0;
    if (qty <= 0) return 0;
    if (inv->items[index].quantity < qty) return 0;
    inv->items[index].quantity -= qty;
    return 1;
}

/* Bubble Sort */
void bubbleSortByQuantity(Inventory *inv) {
    for (int i = 0; i < inv->size - 1; i++) {
        for (int j = 0; j < inv->size - i - 1; j++) {
            if (inv->items[j].quantity < inv->items[j + 1].quantity) {
                Medicine temp = inv->items[j];
                inv->items[j] = inv->items[j + 1];
                inv->items[j + 1] = temp;
            }
        }
    }
    printf("Inventory sorted by stock quantity.\n");
}

/* Insertion Sort */
void insertionSortByExpiry(Inventory *inv) {
    for (int i = 1; i < inv->size; i++) {
        Medicine key = inv->items[i];
        int j = i - 1;
        while (j >= 0 && strcmp(inv->items[j].expiry, key.expiry) > 0) {
            inv->items[j + 1] = inv->items[j];
            j--;
        }
        inv->items[j + 1] = key;
    }
    printf("Inventory sorted by expiry date.\n");
}

/* =========================================================
   CUSTOMERS - DYNAMIC ARRAY
   ========================================================= */
typedef struct {
    int id;
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    float totalSpending;
    float monthlySpending;
    int purchaseCount;
    int registered;
    int discountEligible;
} Customer;

typedef struct {
    Customer *items;
    int size;
    int capacity;
} CustomerArray;

void initCustomers(CustomerArray *arr) {
    arr->size = 0;
    arr->capacity = 5;
    arr->items = (Customer *)malloc(arr->capacity * sizeof(Customer));
    if (arr->items == NULL) {
        printf("Memory allocation failed for customers.\n");
        exit(1);
    }
}

void freeCustomers(CustomerArray *arr) {
    free(arr->items);
    arr->items = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

int findCustomerIndexById(const CustomerArray *arr, int id) {
    for (int i = 0; i < arr->size; i++) {
        if (arr->items[i].id == id) return i;
    }
    return -1;
}

void addCustomer(CustomerArray *arr, Customer customer) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        Customer *temp = (Customer *)realloc(arr->items, arr->capacity * sizeof(Customer));
        if (temp == NULL) {
            printf("Memory reallocation failed.\n");
            exit(1);
        }
        arr->items = temp;
    }
    arr->items[arr->size++] = customer;
}

void updateCustomer(CustomerArray *arr, int id, const char *phone, const char *email) {
    int index = findCustomerIndexById(arr, id);
    if (index == -1) {
        printf("Customer ID not found.\n");
        return;
    }
    strcpy(arr->items[index].phone, phone);
    strcpy(arr->items[index].email, email);
}

void deleteCustomer(CustomerArray *arr, int id) {
    int index = findCustomerIndexById(arr, id);
    if (index == -1) {
        printf("Customer ID not found.\n");
        return;
    }
    for (int i = index; i < arr->size - 1; i++) {
        arr->items[i] = arr->items[i + 1];
    }
    arr->size--;
}

void displayCustomers(const CustomerArray *arr) {
    printTitle("CUSTOMER DETAILS");
    if (arr->size == 0) {
        printf("No customers found.\n");
        return;
    }

    printf("%-5s %-16s %-14s %-22s %-10s %-10s %-10s\n",
           "ID", "Name", "Phone", "Email", "Month", "Total", "Discount");
    printLine();

    for (int i = 0; i < arr->size; i++) {
        printf("%-5d %-16s %-14s %-22s %-10.2f %-10.2f %-10s\n",
               arr->items[i].id,
               arr->items[i].name,
               arr->items[i].phone,
               arr->items[i].email,
               arr->items[i].monthlySpending,
               arr->items[i].totalSpending,
               arr->items[i].discountEligible ? "Yes" : "No");
    }
}

void sortCustomersAndMarkTop5Discount(CustomerArray *arr) {
    for (int i = 0; i < arr->size - 1; i++) {
        for (int j = i + 1; j < arr->size; j++) {
            if (arr->items[j].monthlySpending > arr->items[i].monthlySpending) {
                Customer temp = arr->items[i];
                arr->items[i] = arr->items[j];
                arr->items[j] = temp;
            }
        }
    }

    for (int i = 0; i < arr->size; i++) {
        arr->items[i].discountEligible = 0;
    }

    int top = arr->size < 5 ? arr->size : 5;
    for (int i = 0; i < top; i++) {
        arr->items[i].discountEligible = 1;
    }

    printf("Customers sorted by monthly spending. Top 5 marked for discount.\n");
}

/* =========================================================
   CUSTOMER PURCHASE HISTORY - SINGLY LINKED LIST
   ========================================================= */
typedef struct PurchaseHistoryNode {
    int orderId;
    int customerId;
    float total;
    char date[DATE_LEN];
    struct PurchaseHistoryNode *next;
} PurchaseHistoryNode;

void addCustomerPurchaseHistory(PurchaseHistoryNode **head, int orderId, int customerId, float total, const char *date) {
    PurchaseHistoryNode *newNode = (PurchaseHistoryNode *)malloc(sizeof(PurchaseHistoryNode));
    if (newNode == NULL) return;

    newNode->orderId = orderId;
    newNode->customerId = customerId;
    newNode->total = total;
    strcpy(newNode->date, date);
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        PurchaseHistoryNode *temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
}

void displayCustomerPurchaseHistory(PurchaseHistoryNode *head) {
    printTitle("CUSTOMER PURCHASE HISTORY");
    if (head == NULL) {
        printf("No customer purchase history found.\n");
        return;
    }

    printf("%-8s %-10s %-12s %-12s\n", "OrderID", "CustID", "Total", "Date");
    printLine();

    while (head != NULL) {
        printf("%-8d %-10d %-12.2f %-12s\n",
               head->orderId, head->customerId, head->total, head->date);
        head = head->next;
    }
}

void freeCustomerPurchaseHistory(PurchaseHistoryNode *head) {
    PurchaseHistoryNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* =========================================================
   DRIVERS - DOUBLY LINKED LIST
   ========================================================= */
typedef struct DriverNode {
    int driverId;
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    float paymentPercent;
    float currentBalance;
    float totalEarned;
    struct DriverNode *prev;
    struct DriverNode *next;
} DriverNode;

void addDriver(DriverNode **head, int driverId, const char *name, const char *phone, float paymentPercent) {
    DriverNode *newNode = (DriverNode *)malloc(sizeof(DriverNode));
    if (newNode == NULL) return;

    newNode->driverId = driverId;
    strcpy(newNode->name, name);
    strcpy(newNode->phone, phone);
    newNode->paymentPercent = paymentPercent;
    newNode->currentBalance = 0.0f;
    newNode->totalEarned = 0.0f;
    newNode->prev = NULL;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        DriverNode *temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
        newNode->prev = temp;
    }
}

DriverNode *findDriver(DriverNode *head, int driverId) {
    while (head != NULL) {
        if (head->driverId == driverId) return head;
        head = head->next;
    }
    return NULL;
}

void updateDriver(DriverNode *head, int driverId, const char *phone, float paymentPercent) {
    DriverNode *driver = findDriver(head, driverId);
    if (driver == NULL) {
        printf("Driver ID not found.\n");
        return;
    }
    strcpy(driver->phone, phone);
    driver->paymentPercent = paymentPercent;
}

void deleteDriver(DriverNode **head, int driverId) {
    DriverNode *driver = findDriver(*head, driverId);
    if (driver == NULL) {
        printf("Driver ID not found.\n");
        return;
    }

    if (driver->prev != NULL) driver->prev->next = driver->next;
    else *head = driver->next;

    if (driver->next != NULL) driver->next->prev = driver->prev;

    free(driver);
}

void displayDrivers(DriverNode *head) {
    printTitle("DRIVER DETAILS");
    if (head == NULL) {
        printf("No drivers found.\n");
        return;
    }

    printf("%-5s %-16s %-14s %-10s %-12s %-12s\n",
           "ID", "Name", "Phone", "Pay%", "Balance", "Earned");
    printLine();

    while (head != NULL) {
        printf("%-5d %-16s %-14s %-10.2f %-12.2f %-12.2f\n",
               head->driverId,
               head->name,
               head->phone,
               head->paymentPercent,
               head->currentBalance,
               head->totalEarned);
        head = head->next;
    }
}

DriverNode *assignFirstDriver(DriverNode *head) {
    return head;
}

/* Selection Sort */
void selectionSortDriversByPayment(DriverNode *head) {
    for (DriverNode *i = head; i != NULL; i = i->next) {
        DriverNode *maxNode = i;
        for (DriverNode *j = i->next; j != NULL; j = j->next) {
            if (j->totalEarned > maxNode->totalEarned) {
                maxNode = j;
            }
        }

        if (maxNode != i) {
            int tempId = i->driverId;
            char tempName[NAME_LEN];
            char tempPhone[PHONE_LEN];
            float tempPercent = i->paymentPercent;
            float tempBalance = i->currentBalance;
            float tempEarned = i->totalEarned;

            strcpy(tempName, i->name);
            strcpy(tempPhone, i->phone);

            i->driverId = maxNode->driverId;
            strcpy(i->name, maxNode->name);
            strcpy(i->phone, maxNode->phone);
            i->paymentPercent = maxNode->paymentPercent;
            i->currentBalance = maxNode->currentBalance;
            i->totalEarned = maxNode->totalEarned;

            maxNode->driverId = tempId;
            strcpy(maxNode->name, tempName);
            strcpy(maxNode->phone, tempPhone);
            maxNode->paymentPercent = tempPercent;
            maxNode->currentBalance = tempBalance;
            maxNode->totalEarned = tempEarned;
        }
    }

    printf("Drivers sorted by total earned payment.\n");
}

void freeDrivers(DriverNode *head) {
    DriverNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* =========================================================
   DRIVER PAYMENT HISTORY - SINGLY LINKED LIST
   ========================================================= */
typedef struct DriverPaymentHistoryNode {
    int driverId;
    float amountPaid;
    char date[DATE_LEN];
    struct DriverPaymentHistoryNode *next;
} DriverPaymentHistoryNode;

void addDriverPaymentHistory(DriverPaymentHistoryNode **head, int driverId, float amountPaid, const char *date) {
    DriverPaymentHistoryNode *newNode = (DriverPaymentHistoryNode *)malloc(sizeof(DriverPaymentHistoryNode));
    if (newNode == NULL) return;

    newNode->driverId = driverId;
    newNode->amountPaid = amountPaid;
    strcpy(newNode->date, date);
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        DriverPaymentHistoryNode *temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
}

void displayDriverPaymentHistory(DriverPaymentHistoryNode *head) {
    printTitle("DRIVER PAYMENT HISTORY");
    if (head == NULL) {
        printf("No driver payment history found.\n");
        return;
    }

    printf("%-8s %-12s %-12s\n", "DriverID", "Amount", "Date");
    printLine();

    while (head != NULL) {
        printf("%-8d %-12.2f %-12s\n",
               head->driverId, head->amountPaid, head->date);
        head = head->next;
    }
}

void freeDriverPaymentHistory(DriverPaymentHistoryNode *head) {
    DriverPaymentHistoryNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* =========================================================
   DELIVERY HISTORY - SINGLY LINKED LIST
   ========================================================= */
typedef struct DeliveryNode {
    int orderId;
    int driverId;
    char date[DATE_LEN];
    float total;
    struct DeliveryNode *next;
} DeliveryNode;

void addDeliveryHistory(DeliveryNode **head, int orderId, int driverId, const char *date, float total) {
    DeliveryNode *newNode = (DeliveryNode *)malloc(sizeof(DeliveryNode));
    if (newNode == NULL) return;

    newNode->orderId = orderId;
    newNode->driverId = driverId;
    strcpy(newNode->date, date);
    newNode->total = total;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        DeliveryNode *temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
}

void displayDeliveryHistory(DeliveryNode *head) {
    printTitle("DELIVERY HISTORY");
    if (head == NULL) {
        printf("No delivery history available.\n");
        return;
    }

    printf("%-8s %-8s %-12s %-12s\n", "OrderID", "DriverID", "Date", "Total");
    printLine();

    while (head != NULL) {
        printf("%-8d %-8d %-12s %-12.2f\n",
               head->orderId, head->driverId, head->date, head->total);
        head = head->next;
    }
}

void freeDeliveryHistory(DeliveryNode *head) {
    DeliveryNode *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

/* =========================================================
   ORDER STRUCTURES
   ========================================================= */
typedef struct {
    int medicineId;
    int quantity;
    float lineTotal;
} OrderItem;

typedef struct {
    int orderId;
    int customerId;
    char customerName[NAME_LEN];
    char date[DATE_LEN];
    OrderItem items[MAX_ORDER_ITEMS];
    int itemCount;
    float subtotal;
    float extraCharge;
    float total;
    int driverId;
} Order;

/* =========================================================
   QUEUE - NORMAL ORDERS
   ========================================================= */
typedef struct OrderQueueNode {
    Order order;
    struct OrderQueueNode *next;
} OrderQueueNode;

typedef struct {
    OrderQueueNode *front;
    OrderQueueNode *rear;
} OrderQueue;

void initOrderQueue(OrderQueue *q) {
    q->front = NULL;
    q->rear = NULL;
}

void enqueueOrder(OrderQueue *q, Order order) {
    OrderQueueNode *newNode = (OrderQueueNode *)malloc(sizeof(OrderQueueNode));
    if (newNode == NULL) return;
    newNode->order = order;
    newNode->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeueOrder(OrderQueue *q, Order *order) {
    if (q->front == NULL) return 0;

    OrderQueueNode *temp = q->front;
    *order = temp->order;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    return 1;
}

void displayNormalOrders(OrderQueue *q) {
    printTitle("NORMAL ORDERS QUEUE");
    if (q->front == NULL) {
        printf("No normal orders pending.\n");
        return;
    }

    printf("%-8s %-10s %-18s %-12s %-10s\n", "OrderID", "CustID", "Customer", "Date", "Total");
    printLine();

    OrderQueueNode *temp = q->front;
    while (temp != NULL) {
        printf("%-8d %-10d %-18s %-12s %-10.2f\n",
               temp->order.orderId,
               temp->order.customerId,
               temp->order.customerName,
               temp->order.date,
               temp->order.total);
        temp = temp->next;
    }
}

void freeOrderQueue(OrderQueue *q) {
    OrderQueueNode *temp;
    while (q->front != NULL) {
        temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
}

/* =========================================================
   PRIORITY QUEUE - URGENT ORDERS
   ========================================================= */
typedef struct UrgentOrderNode {
    Order order;
    int priority;
    struct UrgentOrderNode *next;
} UrgentOrderNode;

typedef struct {
    UrgentOrderNode *front;
} UrgentOrderPQ;

void initUrgentOrderPQ(UrgentOrderPQ *pq) {
    pq->front = NULL;
}

void enqueueUrgentOrder(UrgentOrderPQ *pq, Order order, int priority) {
    UrgentOrderNode *newNode = (UrgentOrderNode *)malloc(sizeof(UrgentOrderNode));
    if (newNode == NULL) return;
    newNode->order = order;
    newNode->priority = priority;
    newNode->next = NULL;

    if (pq->front == NULL || priority < pq->front->priority) {
        newNode->next = pq->front;
        pq->front = newNode;
    } else {
        UrgentOrderNode *temp = pq->front;
        while (temp->next != NULL && temp->next->priority <= priority) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

void displayUrgentOrders(UrgentOrderPQ *pq) {
    printTitle("URGENT ORDERS");
    if (pq->front == NULL) {
        printf("No urgent orders found.\n");
        return;
    }

    printf("%-8s %-10s %-18s %-12s %-10s\n", "OrderID", "CustID", "Customer", "Date", "Total");
    printLine();

    UrgentOrderNode *temp = pq->front;
    while (temp != NULL) {
        printf("%-8d %-10d %-18s %-12s %-10.2f\n",
               temp->order.orderId,
               temp->order.customerId,
               temp->order.customerName,
               temp->order.date,
               temp->order.total);
        temp = temp->next;
    }
}

void freeUrgentOrderPQ(UrgentOrderPQ *pq) {
    UrgentOrderNode *temp;
    while (pq->front != NULL) {
        temp = pq->front;
        pq->front = pq->front->next;
        free(temp);
    }
}

/* =========================================================
   REMINDERS - CIRCULAR LINKED LIST
   ========================================================= */
typedef struct ReminderNode {
    int customerId;
    char customerName[NAME_LEN];
    char email[EMAIL_LEN];
    int medicineId;
    int addedMonth;
    int active;
    struct ReminderNode *next;
} ReminderNode;

void addReminder(ReminderNode **head, int customerId, const char *customerName, const char *email, int medicineId, int addedMonth) {
    ReminderNode *newNode = (ReminderNode *)malloc(sizeof(ReminderNode));
    if (newNode == NULL) return;

    newNode->customerId = customerId;
    strcpy(newNode->customerName, customerName);
    strcpy(newNode->email, email);
    newNode->medicineId = medicineId;
    newNode->addedMonth = addedMonth;
    newNode->active = 1;

    if (*head == NULL) {
        *head = newNode;
        newNode->next = newNode;
    } else {
        ReminderNode *temp = *head;
        while (temp->next != *head) temp = temp->next;
        temp->next = newNode;
        newNode->next = *head;
    }
}

void displayReminders(ReminderNode *head) {
    printTitle("MONTHLY REFILL REMINDERS");
    if (head == NULL) {
        printf("No refill reminders available.\n");
        return;
    }

    printf("%-8s %-16s %-22s %-8s %-10s %-8s\n",
           "CustID", "Customer", "Email", "MedID", "AddMonth", "Active");
    printLine();

    ReminderNode *temp = head;
    do {
        printf("%-8d %-16s %-22s %-8d %-10d %-8s\n",
               temp->customerId, temp->customerName, temp->email,
               temp->medicineId, temp->addedMonth, temp->active ? "Yes" : "No");
        temp = temp->next;
    } while (temp != head);
}

void discontinueReminder(ReminderNode *head, int customerId) {
    if (head == NULL) {
        printf("No reminders available.\n");
        return;
    }

    ReminderNode *temp = head;
    do {
        if (temp->customerId == customerId) {
            temp->active = 0;
            printf("Refill reminder discontinued for customer ID %d.\n", customerId);
            return;
        }
        temp = temp->next;
    } while (temp != head);

    printf("Reminder customer ID not found.\n");
}

void sendDueReminders(ReminderNode *head, int currentMonth) {
    printTitle("SENDING DUE MONTHLY REMINDERS");
    if (head == NULL) {
        printf("No reminders available.\n");
        return;
    }

    int found = 0;
    ReminderNode *temp = head;
    do {
        if (temp->active && currentMonth > temp->addedMonth) {
            printf("Reminder sent to %s (%s) for medicine ID %d\n",
                   temp->customerName, temp->email, temp->medicineId);
            found = 1;
        }
        temp = temp->next;
    } while (temp != head);

    if (!found) {
        printf("No reminders due this month.\n");
    }
}

void freeReminders(ReminderNode *head) {
    if (head == NULL) return;

    ReminderNode *temp = head->next;
    while (temp != head) {
        ReminderNode *nextNode = temp->next;
        free(temp);
        temp = nextNode;
    }
    free(head);
}

/* =========================================================
   STACK - REAL UNDO SNAPSHOT
   ========================================================= */
typedef struct Snapshot {
    Inventory inventory;
    CustomerArray customers;
    DriverNode *drivers;
    DeliveryNode *deliveryHistory;
    PurchaseHistoryNode *customerHistory;
    DriverPaymentHistoryNode *driverPaymentHistory;
    OrderQueue normalOrders;
    UrgentOrderPQ urgentOrders;
    ReminderNode *reminders;
} Snapshot;

typedef struct UndoNode {
    Snapshot snapshot;
    char action[ACTION_LEN];
    struct UndoNode *next;
} UndoNode;

typedef struct {
    UndoNode *top;
} UndoStack;

void initUndoStack(UndoStack *stack) {
    stack->top = NULL;
}

void copyInventory(const Inventory *src, Inventory *dest) {
    dest->size = src->size;
    dest->capacity = src->capacity;
    dest->items = (Medicine *)malloc(dest->capacity * sizeof(Medicine));
    for (int i = 0; i < src->size; i++) {
        dest->items[i] = src->items[i];
    }
}

void copyCustomers(const CustomerArray *src, CustomerArray *dest) {
    dest->size = src->size;
    dest->capacity = src->capacity;
    dest->items = (Customer *)malloc(dest->capacity * sizeof(Customer));
    for (int i = 0; i < src->size; i++) {
        dest->items[i] = src->items[i];
    }
}

DriverNode *copyDriversList(DriverNode *src) {
    DriverNode *newHead = NULL, *tail = NULL;
    while (src != NULL) {
        DriverNode *node = (DriverNode *)malloc(sizeof(DriverNode));
        *node = *src;
        node->prev = tail;
        node->next = NULL;

        if (newHead == NULL) newHead = node;
        if (tail != NULL) tail->next = node;
        tail = node;
        src = src->next;
    }
    return newHead;
}

DeliveryNode *copyDeliveryHistoryList(DeliveryNode *src) {
    DeliveryNode *head = NULL, *tail = NULL;
    while (src != NULL) {
        DeliveryNode *node = (DeliveryNode *)malloc(sizeof(DeliveryNode));
        *node = *src;
        node->next = NULL;

        if (head == NULL) head = node;
        else tail->next = node;
        tail = node;

        src = src->next;
    }
    return head;
}

PurchaseHistoryNode *copyPurchaseHistoryList(PurchaseHistoryNode *src) {
    PurchaseHistoryNode *head = NULL, *tail = NULL;
    while (src != NULL) {
        PurchaseHistoryNode *node = (PurchaseHistoryNode *)malloc(sizeof(PurchaseHistoryNode));
        *node = *src;
        node->next = NULL;

        if (head == NULL) head = node;
        else tail->next = node;
        tail = node;

        src = src->next;
    }
    return head;
}

DriverPaymentHistoryNode *copyDriverPaymentHistoryList(DriverPaymentHistoryNode *src) {
    DriverPaymentHistoryNode *head = NULL, *tail = NULL;
    while (src != NULL) {
        DriverPaymentHistoryNode *node = (DriverPaymentHistoryNode *)malloc(sizeof(DriverPaymentHistoryNode));
        *node = *src;
        node->next = NULL;

        if (head == NULL) head = node;
        else tail->next = node;
        tail = node;

        src = src->next;
    }
    return head;
}

void copyOrderQueue(const OrderQueue *src, OrderQueue *dest) {
    dest->front = dest->rear = NULL;
    OrderQueueNode *temp = src->front;
    while (temp != NULL) {
        enqueueOrder(dest, temp->order);
        temp = temp->next;
    }
}

void copyUrgentOrderPQ(const UrgentOrderPQ *src, UrgentOrderPQ *dest) {
    dest->front = NULL;
    UrgentOrderNode *temp = src->front;
    while (temp != NULL) {
        enqueueUrgentOrder(dest, temp->order, temp->priority);
        temp = temp->next;
    }
}

ReminderNode *copyReminderCircular(ReminderNode *src) {
    if (src == NULL) return NULL;

    ReminderNode *newHead = NULL;
    ReminderNode *temp = src;

    do {
        ReminderNode *node = (ReminderNode *)malloc(sizeof(ReminderNode));
        *node = *temp;
        node->next = NULL;

        if (newHead == NULL) {
            newHead = node;
            node->next = newHead;
        } else {
            ReminderNode *last = newHead;
            while (last->next != newHead) last = last->next;
            last->next = node;
            node->next = newHead;
        }

        temp = temp->next;
    } while (temp != src);

    return newHead;
}

void freeSnapshot(Snapshot *s) {
    freeInventory(&s->inventory);
    freeCustomers(&s->customers);
    freeDrivers(s->drivers);
    freeDeliveryHistory(s->deliveryHistory);
    freeCustomerPurchaseHistory(s->customerHistory);
    freeDriverPaymentHistory(s->driverPaymentHistory);
    freeOrderQueue(&s->normalOrders);
    freeUrgentOrderPQ(&s->urgentOrders);
    freeReminders(s->reminders);
}

void createSnapshot(
    Snapshot *snap,
    const Inventory *inventory,
    const CustomerArray *customers,
    DriverNode *drivers,
    DeliveryNode *deliveryHistory,
    PurchaseHistoryNode *customerHistory,
    DriverPaymentHistoryNode *driverPaymentHistory,
    const OrderQueue *normalOrders,
    const UrgentOrderPQ *urgentOrders,
    ReminderNode *reminders
) {
    copyInventory(inventory, &snap->inventory);
    copyCustomers(customers, &snap->customers);
    snap->drivers = copyDriversList(drivers);
    snap->deliveryHistory = copyDeliveryHistoryList(deliveryHistory);
    snap->customerHistory = copyPurchaseHistoryList(customerHistory);
    snap->driverPaymentHistory = copyDriverPaymentHistoryList(driverPaymentHistory);
    copyOrderQueue(normalOrders, &snap->normalOrders);
    copyUrgentOrderPQ(urgentOrders, &snap->urgentOrders);
    snap->reminders = copyReminderCircular(reminders);
}

void pushUndoSnapshot(
    UndoStack *stack,
    const char *action,
    const Inventory *inventory,
    const CustomerArray *customers,
    DriverNode *drivers,
    DeliveryNode *deliveryHistory,
    PurchaseHistoryNode *customerHistory,
    DriverPaymentHistoryNode *driverPaymentHistory,
    const OrderQueue *normalOrders,
    const UrgentOrderPQ *urgentOrders,
    ReminderNode *reminders
) {
    UndoNode *newNode = (UndoNode *)malloc(sizeof(UndoNode));
    if (newNode == NULL) return;

    strcpy(newNode->action, action);
    createSnapshot(&newNode->snapshot, inventory, customers, drivers, deliveryHistory,
                   customerHistory, driverPaymentHistory, normalOrders, urgentOrders, reminders);
    newNode->next = stack->top;
    stack->top = newNode;
}

int popUndoSnapshot(
    UndoStack *stack,
    Inventory *inventory,
    CustomerArray *customers,
    DriverNode **drivers,
    DeliveryNode **deliveryHistory,
    PurchaseHistoryNode **customerHistory,
    DriverPaymentHistoryNode **driverPaymentHistory,
    OrderQueue *normalOrders,
    UrgentOrderPQ *urgentOrders,
    ReminderNode **reminders
) {
    if (stack->top == NULL) return 0;

    UndoNode *temp = stack->top;
    stack->top = temp->next;

    freeInventory(inventory);
    freeCustomers(customers);
    freeDrivers(*drivers);
    freeDeliveryHistory(*deliveryHistory);
    freeCustomerPurchaseHistory(*customerHistory);
    freeDriverPaymentHistory(*driverPaymentHistory);
    freeOrderQueue(normalOrders);
    freeUrgentOrderPQ(urgentOrders);
    freeReminders(*reminders);

    *inventory = temp->snapshot.inventory;
    *customers = temp->snapshot.customers;
    *drivers = temp->snapshot.drivers;
    *deliveryHistory = temp->snapshot.deliveryHistory;
    *customerHistory = temp->snapshot.customerHistory;
    *driverPaymentHistory = temp->snapshot.driverPaymentHistory;
    *normalOrders = temp->snapshot.normalOrders;
    *urgentOrders = temp->snapshot.urgentOrders;
    *reminders = temp->snapshot.reminders;

    printf("Undo successful: %s\n", temp->action);
    free(temp);
    return 1;
}

void displayUndoHistory(UndoStack *stack) {
    printTitle("UNDO STACK");
    if (stack->top == NULL) {
        printf("No undo history available.\n");
        return;
    }

    UndoNode *temp = stack->top;
    int i = 1;
    while (temp != NULL) {
        printf("%d. %s\n", i++, temp->action);
        temp = temp->next;
    }
}

void freeUndoStack(UndoStack *stack) {
    UndoNode *temp;
    while (stack->top != NULL) {
        temp = stack->top;
        stack->top = stack->top->next;
        freeSnapshot(&temp->snapshot);
        free(temp);
    }
}

/* =========================================================
   ORDER PROCESSING
   ========================================================= */
float calculateOrderSubtotal(const Inventory *inventory, OrderItem items[], int itemCount) {
    float subtotal = 0.0f;
    for (int i = 0; i < itemCount; i++) {
        int idx = findMedicineIndexById(inventory, items[i].medicineId);
        if (idx != -1) {
            items[i].lineTotal = inventory->items[idx].price * items[i].quantity;
            subtotal += items[i].lineTotal;
        }
    }
    return subtotal;
}

int validateStockForOrder(const Inventory *inventory, OrderItem items[], int itemCount) {
    for (int i = 0; i < itemCount; i++) {
        int idx = findMedicineIndexById(inventory, items[i].medicineId);
        if (idx == -1) {
            printf("Medicine ID %d not found.\n", items[i].medicineId);
            return 0;
        }
        if (inventory->items[idx].quantity < items[i].quantity) {
            printf("Not enough stock for medicine ID %d.\n", items[i].medicineId);
            return 0;
        }
    }
    return 1;
}

void applyOrderStockUpdate(Inventory *inventory, OrderItem items[], int itemCount) {
    for (int i = 0; i < itemCount; i++) {
        reduceMedicineStock(inventory, items[i].medicineId, items[i].quantity);
    }
}

void updateCustomerPurchase(CustomerArray *customers, int customerId, float total) {
    int index = findCustomerIndexById(customers, customerId);
    if (index != -1) {
        customers->items[index].totalSpending += total;
        customers->items[index].monthlySpending += total;
        customers->items[index].purchaseCount += 1;
    }
}

void assignDriverAndFinancials(DriverNode *drivers, Order *order) {
    DriverNode *driver = assignFirstDriver(drivers);
    if (driver == NULL) {
        order->driverId = 0;
        printf("No driver available. Order has no assigned driver.\n");
        return;
    }

    order->driverId = driver->driverId;
    float driverShare = order->total * (driver->paymentPercent / 100.0f);
    driver->currentBalance += driverShare;
    printf("Driver %s assigned to order.\n", driver->name);
    printf("Driver balance increased by %.2f\n", driverShare);
}

/* =========================================================
   NEW HELPER: REGISTER CUSTOMER DURING ORDER
   ========================================================= */
int registerCustomerDuringOrder(
    CustomerArray *customers,
    UndoStack *undoStack,
    const Inventory *inventory,
    DriverNode *drivers,
    DeliveryNode *deliveryHistory,
    PurchaseHistoryNode *customerHistory,
    DriverPaymentHistoryNode *driverPaymentHistory,
    const OrderQueue *normalOrders,
    const UrgentOrderPQ *urgentOrders,
    ReminderNode *reminders,
    int *customerIdOut,
    char *customerNameOut
) {
    Customer newCustomer;

    printf("Please register the customer first.\n");

    newCustomer.id = getIntInput("Enter new customer ID: ");

    if (findCustomerIndexById(customers, newCustomer.id) != -1) {
        printf("Customer ID already exists.\n");
        return 0;
    }

    getStringInput("Enter customer name: ", newCustomer.name, NAME_LEN);
    getStringInput("Enter phone number: ", newCustomer.phone, PHONE_LEN);
    getStringInput("Enter email: ", newCustomer.email, EMAIL_LEN);

    newCustomer.totalSpending = 0.0f;
    newCustomer.monthlySpending = 0.0f;
    newCustomer.purchaseCount = 0;
    newCustomer.registered = 1;
    newCustomer.discountEligible = 0;

    if (askSaveChanges()) {
        pushUndoSnapshot(undoStack, "Registered new customer during order", inventory, customers, drivers,
                         deliveryHistory, customerHistory, driverPaymentHistory,
                         normalOrders, urgentOrders, reminders);

        addCustomer(customers, newCustomer);
        printf("Customer registered successfully.\n");

        *customerIdOut = newCustomer.id;
        strcpy(customerNameOut, newCustomer.name);
        return 1;
    } else {
        printf("Customer registration cancelled.\n");
        return 0;
    }
}

/* =========================================================
   MENUS
   ========================================================= */
void mainMenu(void) {
    printTitle("WELCOME TO PHARMACY SYSTEM");
    printf("1. Visit Inventory Details\n");
    printf("2. Visit Prescription Details\n");
    printf("3. Visit Driver Details\n");
    printf("4. Visit Customer Details\n");
    printf("5. Visit Delivery History\n");
    printf("6. Visit Undo History / Perform Undo\n");
    printf("7. Exit System\n");
    printLine();
}

void inventoryMenu(void) {
    printTitle("INVENTORY SECTION");
    printf("1. Edit Inventory\n");
    printf("2. Sort Inventory\n");
    printf("3. View Inventory\n");
    printf("4. Go Back\n");
    printLine();
}

void inventoryEditMenu(void) {
    printTitle("EDIT INVENTORY");
    printf("1. Add Medicine\n");
    printf("2. Update Medicine\n");
    printf("3. Delete Medicine\n");
    printf("4. Go Back\n");
    printLine();
}

void inventorySortMenu(void) {
    printTitle("SORT INVENTORY");
    printf("1. Sort by Expiry Date\n");
    printf("2. Sort by Stock Quantity\n");
    printf("3. Go Back\n");
    printLine();
}

void prescriptionMenu(void) {
    printTitle("PRESCRIPTION SECTION");
    printf("1. Add New Order / Prescription\n");
    printf("2. Process Normal Prescription Queue\n");
    printf("3. View Normal Orders\n");
    printf("4. View Urgent Orders\n");
    printf("5. Go Back\n");
    printLine();
}

void driverMenu(void) {
    printTitle("DRIVER SECTION");
    printf("1. Edit Driver Details\n");
    printf("2. Sort Drivers by Payment\n");
    printf("3. View Drivers\n");
    printf("4. Pay Driver Current Balance\n");
    printf("5. View Driver Payment History\n");
    printf("6. Go Back\n");
    printLine();
}

void driverEditMenu(void) {
    printTitle("EDIT DRIVER DETAILS");
    printf("1. Add Driver\n");
    printf("2. Update Driver\n");
    printf("3. Delete Driver\n");
    printf("4. Go Back\n");
    printLine();
}

void customerMenu(void) {
    printTitle("CUSTOMER SECTION");
    printf("1. Edit Customer Details\n");
    printf("2. Sort Customers / Mark Top 5 Discount\n");
    printf("3. View Customers\n");
    printf("4. View Customer Purchase History\n");
    printf("5. Monthly Refill Options\n");
    printf("6. Go Back\n");
    printLine();
}

void customerEditMenu(void) {
    printTitle("EDIT CUSTOMER DETAILS");
    printf("1. Add Customer\n");
    printf("2. Update Customer\n");
    printf("3. Delete Customer\n");
    printf("4. Go Back\n");
    printLine();
}

void customerReminderMenu(void) {
    printTitle("MONTHLY REFILL OPTIONS");
    printf("1. Register Customer for Monthly Refill\n");
    printf("2. Discontinue Refill\n");
    printf("3. View Refill Customers\n");
    printf("4. Send Due Monthly Reminders\n");
    printf("5. Go Back\n");
    printLine();
}

void deliveryMenu(void) {
    printTitle("DELIVERY HISTORY SECTION");
    printf("1. View Delivery History\n");
    printf("2. Go Back\n");
    printLine();
}

void undoMenu(void) {
    printTitle("UNDO SECTION");
    printf("1. View Undo History\n");
    printf("2. Undo Last Saved Change\n");
    printf("3. Go Back\n");
    printLine();
}

/* =========================================================
   MAIN
   ========================================================= */
int main(void) {
    Inventory inventory;
    CustomerArray customers;
    DriverNode *drivers = NULL;
    DeliveryNode *deliveryHistory = NULL;
    PurchaseHistoryNode *customerHistory = NULL;
    DriverPaymentHistoryNode *driverPaymentHistory = NULL;
    OrderQueue normalOrders;
    UrgentOrderPQ urgentOrders;
    ReminderNode *reminders = NULL;
    UndoStack undoStack;

    initInventory(&inventory);
    initCustomers(&customers);
    initOrderQueue(&normalOrders);
    initUrgentOrderPQ(&urgentOrders);
    initUndoStack(&undoStack);

    int mainChoice;

    do {
        mainMenu();
        mainChoice = getIntInput("Choose where you want to visit: ");

        if (mainChoice == 1) {
            int invChoice;
            do {
                inventoryMenu();
                invChoice = getIntInput("Choose an inventory option: ");

                if (invChoice == 1) {
                    int editChoice;
                    do {
                        inventoryEditMenu();
                        editChoice = getIntInput("Choose an edit option: ");

                        if (editChoice == 1) {
                            Medicine med;
                            printTitle("ADD NEW MEDICINE");
                            med.id = getIntInput("Enter medicine ID: ");
                            if (findMedicineIndexById(&inventory, med.id) != -1) {
                                printf("Medicine ID already exists.\n");
                            } else {
                                getStringInput("Enter medicine name: ", med.name, NAME_LEN);
                                getStringInput("Enter batch: ", med.batch, BATCH_LEN);
                                getStringInput("Enter expiry date (YYYY-MM-DD): ", med.expiry, DATE_LEN);
                                med.price = getFloatInput("Enter price: ");
                                med.quantity = getIntInput("Enter quantity: ");

                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Added medicine", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    addMedicine(&inventory, med);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        } else if (editChoice == 2) {
                            printTitle("UPDATE MEDICINE");
                            int id = getIntInput("Enter medicine ID: ");
                            if (findMedicineIndexById(&inventory, id) == -1) {
                                printf("Medicine ID not found. Cannot edit.\n");
                            } else {
                                float price = getFloatInput("Enter new price: ");
                                int qty = getIntInput("Enter new quantity: ");

                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Updated medicine", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    updateMedicine(&inventory, id, price, qty);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        } else if (editChoice == 3) {
                            printTitle("DELETE MEDICINE");
                            int id = getIntInput("Enter medicine ID: ");
                            if (findMedicineIndexById(&inventory, id) == -1) {
                                printf("Medicine ID not found. Cannot delete.\n");
                            } else {
                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Deleted medicine", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    deleteMedicine(&inventory, id);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        }

                        if (editChoice != 4) pressEnterToContinue();

                    } while (editChoice != 4);
                }

                else if (invChoice == 2) {
                    int sortChoice;
                    do {
                        inventorySortMenu();
                        sortChoice = getIntInput("Choose a sort option: ");

                        if (sortChoice == 1) {
                            insertionSortByExpiry(&inventory);
                            displayInventory(&inventory);
                            pressEnterToContinue();
                        } else if (sortChoice == 2) {
                            bubbleSortByQuantity(&inventory);
                            displayInventory(&inventory);
                            pressEnterToContinue();
                        }

                    } while (sortChoice != 3);
                }

                else if (invChoice == 3) {
                    displayInventory(&inventory);
                    pressEnterToContinue();
                }

            } while (invChoice != 4);
        }

        else if (mainChoice == 2) {
            int presChoice;
            do {
                prescriptionMenu();
                presChoice = getIntInput("Choose a prescription option: ");

                if (presChoice == 1) {
                    printTitle("ADD NEW ORDER / PRESCRIPTION");

                    int urgent = getIntInput("Is this urgent? (1 = Yes, 0 = No) - Urgent requests include extra charges: ");
                    int registered = getIntInput("Is customer registered? (1 = Yes, 0 = No): ");

                    int customerId = 0;
                    char customerName[NAME_LEN];

                    if (registered == 1) {
                        customerId = getIntInput("Enter customer ID: ");
                        int cIndex = findCustomerIndexById(&customers, customerId);
                        if (cIndex == -1) {
                            printf("Customer not found.\n");
                            pressEnterToContinue();
                            continue;
                        }
                        strcpy(customerName, customers.items[cIndex].name);
                    } else {
                        int registeredNow = registerCustomerDuringOrder(
                            &customers, &undoStack, &inventory, drivers, deliveryHistory,
                            customerHistory, driverPaymentHistory, &normalOrders, &urgentOrders,
                            reminders, &customerId, customerName
                        );

                        if (!registeredNow) {
                            pressEnterToContinue();
                            continue;
                        }
                    }

                    Order order;
                    order.orderId = getIntInput("Enter order ID: ");
                    order.customerId = customerId;
                    strcpy(order.customerName, customerName);
                    getStringInput("Enter order date (YYYY-MM-DD): ", order.date, DATE_LEN);
                    order.itemCount = 0;
                    order.subtotal = 0;
                    order.extraCharge = 0;
                    order.total = 0;
                    order.driverId = 0;

                    while (1) {
                        int medId = getIntInput("Enter medicine ID or press 0 to calculate total: ");
                        if (medId == 0) break;

                        int idx = findMedicineIndexById(&inventory, medId);
                        if (idx == -1) {
                            printf("Medicine ID not found.\n");
                            continue;
                        }

                        int qty = getIntInput("Enter quantity: ");
                        if (order.itemCount >= MAX_ORDER_ITEMS) {
                            printf("Maximum order items reached.\n");
                            break;
                        }

                        order.items[order.itemCount].medicineId = medId;
                        order.items[order.itemCount].quantity = qty;
                        order.items[order.itemCount].lineTotal = 0;
                        order.itemCount++;
                    }

                    if (order.itemCount == 0) {
                        printf("No items added to order.\n");
                        pressEnterToContinue();
                        continue;
                    }

                    order.subtotal = calculateOrderSubtotal(&inventory, order.items, order.itemCount);

                    if (urgent == 1) {
                        order.extraCharge = URGENT_EXTRA_CHARGE;
                    }

                    order.total = order.subtotal + order.extraCharge;

                    int cIndex = findCustomerIndexById(&customers, customerId);
                    if (cIndex != -1 && customers.items[cIndex].discountEligible) {
                        float discount = order.total * 0.10f;
                        order.total -= discount;
                        printf("Top customer discount applied: -%.2f\n", discount);
                    }

                    printf("\nOrder Summary:\n");
                    printf("Subtotal     : %.2f\n", order.subtotal);
                    printf("Extra Charge : %.2f\n", order.extraCharge);
                    printf("Total        : %.2f\n", order.total);

                    if (!validateStockForOrder(&inventory, order.items, order.itemCount)) {
                        printf("Order cannot be saved because of invalid stock.\n");
                        pressEnterToContinue();
                        continue;
                    }

                    if (askSaveChanges()) {
                        pushUndoSnapshot(&undoStack, "Added order", &inventory, &customers, drivers,
                                         deliveryHistory, customerHistory, driverPaymentHistory,
                                         &normalOrders, &urgentOrders, reminders);

                        if (urgent == 1) {
                            applyOrderStockUpdate(&inventory, order.items, order.itemCount);
                            updateCustomerPurchase(&customers, customerId, order.total);
                            assignDriverAndFinancials(drivers, &order);
                            addDeliveryHistory(&deliveryHistory, order.orderId, order.driverId, order.date, order.total);
                            addCustomerPurchaseHistory(&customerHistory, order.orderId, customerId, order.total, order.date);
                            enqueueUrgentOrder(&urgentOrders, order, 1);

                            printf("Urgent order saved and processed immediately.\n");
                        } else {
                            enqueueOrder(&normalOrders, order);
                            printf("Normal order saved to queue.\n");
                        }
                    } else {
                        printf("Changes discarded.\n");
                    }

                    pressEnterToContinue();
                }

                else if (presChoice == 2) {
                    printTitle("PROCESS NORMAL PRESCRIPTION QUEUE");
                    Order order;

                    if (!dequeueOrder(&normalOrders, &order)) {
                        printf("No normal orders in queue.\n");
                    } else {
                        pushUndoSnapshot(&undoStack, "Processed normal order", &inventory, &customers, drivers,
                                         deliveryHistory, customerHistory, driverPaymentHistory,
                                         &normalOrders, &urgentOrders, reminders);

                        if (!validateStockForOrder(&inventory, order.items, order.itemCount)) {
                            printf("Cannot process order due to stock issues.\n");
                        } else {
                            applyOrderStockUpdate(&inventory, order.items, order.itemCount);
                            updateCustomerPurchase(&customers, order.customerId, order.total);
                            assignDriverAndFinancials(drivers, &order);
                            addDeliveryHistory(&deliveryHistory, order.orderId, order.driverId, order.date, order.total);
                            addCustomerPurchaseHistory(&customerHistory, order.orderId, order.customerId, order.total, order.date);

                            printf("Normal order processed successfully.\n");
                        }
                    }

                    pressEnterToContinue();
                }

                else if (presChoice == 3) {
                    displayNormalOrders(&normalOrders);
                    pressEnterToContinue();
                }

                else if (presChoice == 4) {
                    displayUrgentOrders(&urgentOrders);
                    pressEnterToContinue();
                }

            } while (presChoice != 5);
        }

        else if (mainChoice == 3) {
            int drvChoice;
            do {
                driverMenu();
                drvChoice = getIntInput("Choose a driver option: ");

                if (drvChoice == 1) {
                    int editChoice;
                    do {
                        driverEditMenu();
                        editChoice = getIntInput("Choose an edit option: ");

                        if (editChoice == 1) {
                            printTitle("ADD DRIVER");
                            int id = getIntInput("Enter driver ID: ");
                            if (findDriver(drivers, id) != NULL) {
                                printf("Driver ID already exists.\n");
                            } else {
                                char name[NAME_LEN], phone[PHONE_LEN];
                                float percent;
                                getStringInput("Enter driver name: ", name, NAME_LEN);
                                getStringInput("Enter phone number: ", phone, PHONE_LEN);
                                percent = getFloatInput("Enter payment percentage: ");

                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Added driver", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    addDriver(&drivers, id, name, phone, percent);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        } else if (editChoice == 2) {
                            printTitle("UPDATE DRIVER");
                            int id = getIntInput("Enter driver ID: ");
                            if (findDriver(drivers, id) == NULL) {
                                printf("Driver ID not found. Cannot edit.\n");
                            } else {
                                char phone[PHONE_LEN];
                                float percent;
                                getStringInput("Enter new phone number: ", phone, PHONE_LEN);
                                percent = getFloatInput("Enter new payment percentage: ");

                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Updated driver", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    updateDriver(drivers, id, phone, percent);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        } else if (editChoice == 3) {
                            printTitle("DELETE DRIVER");
                            int id = getIntInput("Enter driver ID: ");
                            if (findDriver(drivers, id) == NULL) {
                                printf("Driver ID not found. Cannot delete.\n");
                            } else {
                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Deleted driver", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    deleteDriver(&drivers, id);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        }

                        if (editChoice != 4) pressEnterToContinue();

                    } while (editChoice != 4);
                }

                else if (drvChoice == 2) {
                    selectionSortDriversByPayment(drivers);
                    displayDrivers(drivers);
                    pressEnterToContinue();
                }

                else if (drvChoice == 3) {
                    displayDrivers(drivers);
                    pressEnterToContinue();
                }

                else if (drvChoice == 4) {
                    printTitle("PAY DRIVER CURRENT BALANCE");
                    int driverId = getIntInput("Enter driver ID: ");
                    DriverNode *driver = findDriver(drivers, driverId);

                    if (driver == NULL) {
                        printf("Driver ID not found.\n");
                    } else if (driver->currentBalance <= 0) {
                        printf("Driver has no current balance to pay.\n");
                    } else {
                        char date[DATE_LEN];
                        getStringInput("Enter payment date (YYYY-MM-DD): ", date, DATE_LEN);

                        if (askSaveChanges()) {
                            pushUndoSnapshot(&undoStack, "Paid driver balance", &inventory, &customers, drivers,
                                             deliveryHistory, customerHistory, driverPaymentHistory,
                                             &normalOrders, &urgentOrders, reminders);

                            addDriverPaymentHistory(&driverPaymentHistory, driverId, driver->currentBalance, date);
                            driver->totalEarned += driver->currentBalance;
                            driver->currentBalance = 0.0f;

                            printf("Driver payment completed. Current balance is now zero.\n");
                        } else {
                            printf("Changes discarded.\n");
                        }
                    }

                    pressEnterToContinue();
                }

                else if (drvChoice == 5) {
                    displayDriverPaymentHistory(driverPaymentHistory);
                    pressEnterToContinue();
                }

            } while (drvChoice != 6);
        }

        else if (mainChoice == 4) {
            int custChoice;
            do {
                customerMenu();
                custChoice = getIntInput("Choose a customer option: ");

                if (custChoice == 1) {
                    int editChoice;
                    do {
                        customerEditMenu();
                        editChoice = getIntInput("Choose an edit option: ");

                        if (editChoice == 1) {
                            printTitle("ADD CUSTOMER");
                            Customer customer;
                            customer.id = getIntInput("Enter customer ID: ");

                            if (findCustomerIndexById(&customers, customer.id) != -1) {
                                printf("Customer ID already exists.\n");
                            } else {
                                getStringInput("Enter customer name: ", customer.name, NAME_LEN);
                                getStringInput("Enter phone number: ", customer.phone, PHONE_LEN);
                                getStringInput("Enter email: ", customer.email, EMAIL_LEN);
                                customer.totalSpending = 0.0f;
                                customer.monthlySpending = 0.0f;
                                customer.purchaseCount = 0;
                                customer.registered = 1;
                                customer.discountEligible = 0;

                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Added customer", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    addCustomer(&customers, customer);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        }

                        else if (editChoice == 2) {
                            printTitle("UPDATE CUSTOMER");
                            int id = getIntInput("Enter customer ID: ");
                            if (findCustomerIndexById(&customers, id) == -1) {
                                printf("Customer ID not found. Cannot edit.\n");
                            } else {
                                char phone[PHONE_LEN], email[EMAIL_LEN];
                                getStringInput("Enter new phone number: ", phone, PHONE_LEN);
                                getStringInput("Enter new email: ", email, EMAIL_LEN);

                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Updated customer", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    updateCustomer(&customers, id, phone, email);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        }

                        else if (editChoice == 3) {
                            printTitle("DELETE CUSTOMER");
                            int id = getIntInput("Enter customer ID: ");
                            if (findCustomerIndexById(&customers, id) == -1) {
                                printf("Customer ID not found. Cannot delete.\n");
                            } else {
                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Deleted customer", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    deleteCustomer(&customers, id);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        }

                        if (editChoice != 4) pressEnterToContinue();

                    } while (editChoice != 4);
                }

                else if (custChoice == 2) {
                    sortCustomersAndMarkTop5Discount(&customers);
                    displayCustomers(&customers);
                    pressEnterToContinue();
                }

                else if (custChoice == 3) {
                    displayCustomers(&customers);
                    pressEnterToContinue();
                }

                else if (custChoice == 4) {
                    displayCustomerPurchaseHistory(customerHistory);
                    pressEnterToContinue();
                }

                else if (custChoice == 5) {
                    int remChoice;
                    do {
                        customerReminderMenu();
                        remChoice = getIntInput("Choose a refill option: ");

                        if (remChoice == 1) {
                            printTitle("REGISTER MONTHLY REFILL");
                            int customerId = getIntInput("Enter customer ID: ");
                            int cIndex = findCustomerIndexById(&customers, customerId);

                            if (cIndex == -1) {
                                printf("Customer ID not found.\n");
                            } else {
                                int medicineId = getIntInput("Enter medicine ID: ");
                                int addedMonth = getIntInput("Enter added month number (1-12): ");

                                if (askSaveChanges()) {
                                    pushUndoSnapshot(&undoStack, "Registered monthly refill", &inventory, &customers, drivers,
                                                     deliveryHistory, customerHistory, driverPaymentHistory,
                                                     &normalOrders, &urgentOrders, reminders);
                                    addReminder(&reminders,
                                                customers.items[cIndex].id,
                                                customers.items[cIndex].name,
                                                customers.items[cIndex].email,
                                                medicineId,
                                                addedMonth);
                                    printf("Changes saved.\n");
                                } else {
                                    printf("Changes discarded.\n");
                                }
                            }
                        }

                        else if (remChoice == 2) {
                            printTitle("DISCONTINUE REFILL");
                            int customerId = getIntInput("Enter customer ID: ");
                            if (askSaveChanges()) {
                                pushUndoSnapshot(&undoStack, "Discontinued monthly refill", &inventory, &customers, drivers,
                                                 deliveryHistory, customerHistory, driverPaymentHistory,
                                                 &normalOrders, &urgentOrders, reminders);
                                discontinueReminder(reminders, customerId);
                                printf("Changes saved.\n");
                            } else {
                                printf("Changes discarded.\n");
                            }
                        }

                        else if (remChoice == 3) {
                            displayReminders(reminders);
                        }

                        else if (remChoice == 4) {
                            int currentMonth = getIntInput("Enter current month number (1-12): ");
                            sendDueReminders(reminders, currentMonth);
                        }

                        if (remChoice != 5) pressEnterToContinue();

                    } while (remChoice != 5);
                }

            } while (custChoice != 6);
        }

        else if (mainChoice == 5) {
            int delChoice;
            do {
                deliveryMenu();
                delChoice = getIntInput("Choose a delivery history option: ");

                if (delChoice == 1) {
                    displayDeliveryHistory(deliveryHistory);
                    pressEnterToContinue();
                }

            } while (delChoice != 2);
        }

        else if (mainChoice == 6) {
            int uChoice;
            do {
                undoMenu();
                uChoice = getIntInput("Choose an undo option: ");

                if (uChoice == 1) {
                    displayUndoHistory(&undoStack);
                    pressEnterToContinue();
                } else if (uChoice == 2) {
                    if (!popUndoSnapshot(&undoStack, &inventory, &customers, &drivers,
                                         &deliveryHistory, &customerHistory, &driverPaymentHistory,
                                         &normalOrders, &urgentOrders, &reminders)) {
                        printf("No undo history available.\n");
                    }
                    pressEnterToContinue();
                }

            } while (uChoice != 3);
        }

    } while (mainChoice != 7);

    freeInventory(&inventory);
    freeCustomers(&customers);
    freeDrivers(drivers);
    freeDeliveryHistory(deliveryHistory);
    freeCustomerPurchaseHistory(customerHistory);
    freeDriverPaymentHistory(driverPaymentHistory);
    freeOrderQueue(&normalOrders);
    freeUrgentOrderPQ(&urgentOrders);
    freeReminders(reminders);
    freeUndoStack(&undoStack);

    printTitle("EXITING SYSTEM");
    printf("Thank you for using the Pharmacy System.\n");

    return 0;
}
