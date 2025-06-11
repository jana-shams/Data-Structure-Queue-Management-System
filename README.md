# Priority-Based Multi-Level Queue System (C++)

Deployment: https://queuesystemm.web.app/

## Overview

This project implements a priority-based Multi-Level Queue (MLQ) system in C++. The queue is used to simulate a real-world service scenario such as a bank, where individuals are served based on their priority level determined by emergency status, service type, and waiting time.

## Features

- **Multi-Level Queue Structure**: Supports three priority levels (High, Medium, Low).
- **Dynamic Priority Calculation**: Based on emergency status, type of service, and waiting time.
- **Efficient Operations**: Constant time `enqueue` and `dequeue` operations.
- **Waiting Time Updates**: Re-evaluates priority dynamically as waiting time increases.
- **Advanced Test Case**: Includes demonstration with 8 different service scenarios.
- **Time Complexity & Justification**: Included analysis for educational purposes.

## Data Structures Used

- Custom `Individual` class to store user details and calculate priority.
- Linked list implementation of three priority queues.
- `PriorityQueue` class manages the queues and operations.

## Priority Assignment

| Condition                                | Priority |
|------------------------------------------|----------|
| VIP, Emergency Case, or Emergency Status | High     |
| Loan Request, Money Deposit, or Score ≥3 | Medium   |
| Cash Withdrawal or Score < 3             | Low      |

## Operations

- `enqueue`: Inserts an individual into the appropriate queue.
- `dequeue`: Removes and returns the highest-priority individual.
- `peek`: Views the front individual without removal.
- `updateWaitingTimes`: Increases waiting time and re-evaluates priority.
- `printQueue`: Displays the entire queue with formatted output.

## Example Usage

The main function includes:
- Enqueueing 8 test individuals.
- Printing the queue.
- Processing the queue with individual service times.
- Showing real-time queue updates and priority changes.

## How to Compile

Use any C++ compiler that supports C++11 or later. Example:

```bash
g++ -o priority_queue main.cpp
./priority_queue
```

## Educational Value

This implementation is ideal for understanding:
- Priority queues
- Linked list-based queue design
- Dynamic data structure behavior
- Time complexity analysis
- Real-world queue systems

## License

MIT License
