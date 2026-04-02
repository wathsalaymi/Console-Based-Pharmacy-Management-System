# Console-Based Pharmacy Management System

A menu-driven **console-based pharmacy management system** developed in **C** to demonstrate the practical use of **data structures and algorithms** in a real-world style application.

This project was developed for the course **IN 1111 – Data Structures and Algorithms I**. It simulates the core operations of a pharmacy, including inventory management, prescription handling, customer records, driver management, delivery history, refill reminders, sorting, and undo functionality.

---

## Project Overview

Managing a pharmacy requires keeping track of medicines, customers, prescriptions, drivers, payments, and recurring refill requests in an organized way. This system was designed to solve those problems through a single integrated console application.

The program supports:

- Managing medicine inventory
- Processing normal and urgent prescriptions
- Registering and managing customers
- Managing drivers and tracking payments
- Viewing delivery history
- Handling monthly refill reminders
- Sorting useful business data
- Undoing recently saved changes

---

## Objectives

The main objectives of this project are:

- To manage medicine inventory efficiently
- To store and process normal and urgent prescriptions
- To register and manage customer details
- To track customer monthly spending and identify top customers for discounts
- To manage driver details and driver payments
- To record delivery history for completed orders
- To maintain monthly refill reminders
- To apply multiple data structures meaningfully in one system
- To use sorting algorithms for practical management views
- To provide undo functionality for saved changes

---

## Features

### 1. Inventory Management
- Add new medicines
- Update medicine details
- Delete medicines
- View all medicines
- Sort medicines by expiry date
- Sort medicines by stock quantity

### 2. Prescription Management
- Add new prescriptions/orders
- Mark an order as urgent or normal
- Register a customer immediately if not already registered
- Add medicines to an order
- Calculate total payment
- Apply urgent charges automatically for urgent orders
- Store normal prescriptions in a queue
- Process normal prescription queue later
- View urgent and normal orders

### 3. Customer Management
- Add new customers
- Update customer details
- Delete customers
- View customer records
- Sort customers by monthly spending
- Mark top 5 customers as discount eligible
- View customer purchase history
- Manage monthly refill reminders

### 4. Driver Management
- Add drivers
- Update driver details
- Delete drivers
- View drivers
- Sort drivers by total earned payment
- Pay driver current balance
- View driver payment history

### 5. Delivery History
- View completed delivery records
- Store order ID, driver ID, date, and total amount

### 6. Undo Functionality
- Save the previous state before confirmed changes
- View undo history
- Undo the last saved change

---

## Data Structures Used

This project was specially designed to show how different data structures can be used in different real-life situations.

### Dynamic Array
Used for:
- Medicine inventory
- Customer records

Why it was used:
- Supports dynamic growth
- Easy searching and updating
- Good for traversal and sorting
- Suitable for CRUD operations

### Singly Linked List
Used for:
- Delivery history
- Customer purchase history
- Driver payment history

Why it was used:
- Good for records that grow over time
- Efficient for sequential traversal
- Useful for storing historical data

### Queue
Used for:
- Normal prescription orders

Why it was used:
- Follows **FIFO (First In, First Out)**
- Ensures fairness in handling normal orders
- Matches real-world service flow

### Separate Urgent Order Queue
Used for:
- Urgent prescriptions

Why it was used:
- Urgent orders are handled separately
- Helps prioritize urgent deliveries
- Supports quick processing of important orders

### Doubly Linked List
Used for:
- Driver management

Why it was used:
- Easy insertion, deletion, and traversal
- Flexible for driver record management
- Useful when both previous and next links are helpful

### Circular Linked List
Used for:
- Monthly refill reminders

Why it was used:
- Suitable for recurring processes
- Naturally supports repeated cycles
- Good for long-term refill tracking

### Stack
Used for:
- Undo functionality

Why it was used:
- Follows **LIFO (Last In, First Out)**
- Best match for undo operations
- Restores the most recent saved state first

---

## Sorting Algorithms Used

### Bubble Sort
Used to sort:
- Medicines by stock quantity in descending order
- Customers by monthly spending in descending order

### Insertion Sort
Used to sort:
- Medicines by expiry date in ascending order

### Selection Sort
Used to sort:
- Drivers by total earned payment in descending order

---

## System Modules

The system is divided into the following main sections:

1. **Inventory Details**
2. **Prescription Details**
3. **Driver Details**
4. **Customer Details**
5. **Delivery History**
6. **Undo History / Perform Undo**
7. **Exit System**

This structure makes the application easy to use through a simple menu-based interface.

---

## Sample Workflow

A typical system flow is:

1. Start the program
2. Display the main menu
3. Select a module
4. Perform an operation
5. Save changes if needed
6. Store previous state for undo
7. Return to the menu
8. Continue until exit

---

## Technologies Used

- **Programming Language:** C
- **Interface:** Console / Command Line
- **Paradigm:** Menu-driven procedural programming

---

## How to Run the Project

### Requirements
- GCC compiler or any C compiler
- Terminal / Command Prompt

### Compile
```bash
gcc main.c -o pharmacy
```

### Run
```bash
./pharmacy
```

> On Windows:
```bash
gcc main.c -o pharmacy.exe
pharmacy.exe
```

---

## Learning Outcomes

Through this project, we gained a practical understanding of:

- Choosing the correct data structure for a real problem
- Applying queues, stacks, arrays, and linked lists meaningfully
- Using sorting algorithms for management-related tasks
- Designing a modular menu-driven system
- Managing related data across multiple modules
- Implementing safe editing with undo support

---

## Challenges Faced

Some of the main challenges during development were:

- Implementing undo functionality correctly
- Keeping related records consistent after purchases
- Making the console interface user-friendly
- Managing multiple modules in a single integrated system

These challenges were handled by improving the menu flow, using appropriate data structures, and applying save/undo logic carefully.

---

## Future Improvements

This project can be improved further by adding:

- File handling for permanent data storage
- Login and admin authentication
- Better input validation
- Search filters for medicines and customers
- Expiry alerts for medicines
- Billing receipt generation
- GUI version of the system
- Database integration using MySQL or SQLite

---

## Academic Context

This project was developed as part of:

**Course:** IN 1111 – Data Structures and Algorithms I

It demonstrates how data structures and algorithms can be applied to solve practical pharmacy-related problems in a simple but meaningful way.

---

## Contributors

- Ekanayake EMPS
- Wathsala YMI
- Gnanaweera WMKMK
- Liyanaarachchi WKACS
- Heshan WGR
- Ekanayaka KGKN
- Pathirana DHPSNR

---

## Conclusion

The **Console-Based Pharmacy Management System** is a practical academic project that combines multiple data structures and sorting algorithms into one working application. It shows how theoretical concepts from data structures can be used to solve realistic management problems in an organized and efficient way.

