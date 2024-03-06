#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDERS 100
#define MAX_USERS 10
#define MAX_FOODS 5


typedef struct {
    int orderNumber;
    char customerName[50];
    char itemName[50];
    float itemPrice;
    int isPaid;
} Order;


typedef struct {
    Order orders[MAX_ORDERS];
    int front;
    int rear;
    int count;
} OrderQueue;

// Structure to represent a user
typedef struct {
    char username[50];
    char password[50];
} User;

// Structure to represent a food item
typedef struct {
    char itemName[50];
    float itemPrice;
} FoodItem;


void initializeQueue(OrderQueue* queue);
int isQueueEmpty(OrderQueue* queue);
int isQueueFull(OrderQueue* queue);
void enqueueOrder(OrderQueue* queue, Order newOrder);
Order dequeueOrder(OrderQueue* queue);
void displayOrder(Order order);
void placeOrder(OrderQueue* queue, FoodItem* menu, int menuSize);
void processOrders(OrderQueue* queue);
int login(User* users, int totalUsers);
void confirmPayment(OrderQueue* queue, int orderNumber);
void processNextOrder(OrderQueue* queue);


void initializeQueue(OrderQueue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
}


int isQueueEmpty(OrderQueue* queue) {
    return (queue->count == 0);
}

// Check if the order queue is full
int isQueueFull(OrderQueue* queue) {
    return (queue->count == MAX_ORDERS);
}


void enqueueOrder(OrderQueue* queue, Order newOrder) {
    if (isQueueFull(queue)) {
        printf("Order queue is full. Cannot accept new orders.\n");
        return;
    }

    queue->rear = (queue->rear + 1) % MAX_ORDERS;
    queue->orders[queue->rear] = newOrder;
    queue->count++;
}


Order dequeueOrder(OrderQueue* queue) {
    Order dequeuedOrder;

    if (isQueueEmpty(queue)) {
        printf("No orders in the queue.\n");
        strcpy(dequeuedOrder.customerName, "");
        return dequeuedOrder;
    }

    dequeuedOrder = queue->orders[queue->front];
    queue->front = (queue->front + 1) % MAX_ORDERS;
    queue->count--;

    return dequeuedOrder;
}


void displayOrder(Order order) {
    printf("Order Number: %d\n", order.orderNumber);
    printf("Customer Name: %s\n", order.customerName);
    printf("Item Name: %s\n", order.itemName);
    printf("Item Price: $%.2f\n", order.itemPrice);
    printf("Payment Status: %s\n\n", order.isPaid ? "Paid" : "Not Paid");
}

// Place a new order
void placeOrder(OrderQueue* queue, FoodItem* menu, int menuSize) {
    Order newOrder;

    printf("Enter Customer Name: ");
    scanf("%s", newOrder.customerName);

    printf("Menu:\n");
    int i;
    for (i = 0; i < menuSize; i++) {
        printf("%d. %s ($%.2f)\n", i + 1, menu[i].itemName, menu[i].itemPrice);
    }

    int itemChoice;
    printf("Enter Item Choice: ");
    scanf("%d", &itemChoice);

    // Validate the item choice
    if (itemChoice < 1 || itemChoice > menuSize) {
        printf("Invalid item choice. Order failed.\n");
        return;
    }

    // Copy the selected item details to the order
    strcpy(newOrder.itemName, menu[itemChoice - 1].itemName);
    newOrder.itemPrice = menu[itemChoice - 1].itemPrice;

    newOrder.orderNumber = queue->count + 1;
    newOrder.isPaid = 0;

    enqueueOrder(queue, newOrder);

    printf("Order placed successfully!\n\n");
}


void processOrders(OrderQueue* queue) {
    int choice;
    Order processedOrder;

    while (!isQueueEmpty(queue)) {
        processedOrder = dequeueOrder(queue);

        printf("Processing Order:\n");
        displayOrder(processedOrder);

        printf("1. Confirm Payment\n");
        printf("2. Process Next Order\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);

        switch (choice) {
            case 1:
                confirmPayment(queue, processedOrder.orderNumber);
                break;
            case 2:
                processNextOrder(queue);
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    printf("No more orders in the queue.\n");
}


int login(User* users, int totalUsers) {
    char username[50];
    char password[50];

    printf("Enter your username: ");
    scanf("%s", username);

    printf("Enter your password: ");
    scanf("%s", password);

    int i;
    for (i = 0; i < totalUsers; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }

    return 0;
}

// Confirm payment for an order
void confirmPayment(OrderQueue* queue, int orderNumber) {
    int i;
    for (i = 0; i < queue->count; i++) {
        if (queue->orders[i].orderNumber == orderNumber) {
            queue->orders[i].isPaid = 1;
            printf("Payment confirmed for Order Number %d.\n\n", orderNumber);
            return;
        }
    }

    printf("valid order number. Payment confirmation success.\n\n");
}

// Process the next order in the queue
void processNextOrder(OrderQueue* queue) {
    if (isQueueEmpty(queue)) {
        printf("No more orders in the queue.\n");
        return;
    }

    Order nextOrder = queue->orders[queue->front];
    printf("Processing next order:\n");
    displayOrder(nextOrder);
}

int main() {
    // Sample menu
    FoodItem menu[MAX_FOODS] = {
        {"Burger", 5.99},
        {"Pizza", 9.99},
        {"Salad", 4.99},
        {"Pasta", 7.99},
        {"Sandwich", 3.99}
    };

    // Sample users
    User users[MAX_USERS] = {
        {"admin", "admin123"},
        {"user1", "pass123"},
        {"user2", "pass456"}
    };

    OrderQueue orderQueue;
    initializeQueue(&orderQueue);

    int loggedIn = 0;
    int totalUsers = sizeof(users) / sizeof(users[0]);

    printf("Welcome to the Order Management System!\n");

    while (!loggedIn) {
        loggedIn = login(users, totalUsers);

        if (!loggedIn) {
            printf("Invalid username or password. Please try again.\n");
        }
    }

    int choice;
    while (1) {
        printf("1. Place Order\n");
        printf("2. Process Orders\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                placeOrder(&orderQueue, menu, MAX_FOODS);
                break;
            case 2:
                processOrders(&orderQueue);
                break;
            case 3:
                printf("Thank you for using the Order Management System. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
