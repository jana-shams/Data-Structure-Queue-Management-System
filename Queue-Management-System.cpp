#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// Individual class to represent a person in the queue
class Individual {
private:
    int id;                 // Unique identifier
    string serviceType;     // Type of service (e.g., Cash Withdrawal)
    bool emergencyStatus;   // True if emergency case
    int waitingTime;        // Waiting time in minutes
    double priorityScore;   // Calculated priority score

public:
    // Default constructor
    Individual() {
        id = 0;
        serviceType = "";
        emergencyStatus = false;
        waitingTime = 0;
        priorityScore = 0.0;
    }

    // Parameterized constructor
    Individual(int _id, string _serviceType, bool _emergency, int _waitingTime) {
        id = _id;
        serviceType = _serviceType;
        emergencyStatus = _emergency;
        waitingTime = _waitingTime;
        calculatePriorityScore();
    }

    // Calculate priority score
    void calculatePriorityScore() {
        // Emergency status gives 5 points
        double emergencyFactor = emergencyStatus ? 5.0 : 0.0;

        // Service type points (VIP highest)
        double serviceFactor = 0.0;
        if (serviceType == "VIP") {
            serviceFactor = 6.0;
        }
        else if (serviceType == "Emergency Case") {
            serviceFactor = 3.0;
        }
        else if (serviceType == "Loan Request") {
            serviceFactor = 2.0;
        }
        else if (serviceType == "Money Deposit") {
            serviceFactor = 1.5;
        }
        else if (serviceType == "Cash Withdrawal") {
            serviceFactor = 1.0;
        }

        // Waiting time adds 0.2 points per minute
        double waitingFactor = waitingTime * 0.2;

        // Total priority score
        priorityScore = emergencyFactor + serviceFactor + waitingFactor;
    }

    // Add minutes to waiting time and update score
    void incrementWaitingTime(int minutes) {
        waitingTime += minutes;
        calculatePriorityScore();
    }

    // Get service duration in minutes
    int getServiceDuration() {
        if (serviceType == "Cash Withdrawal") return 2;
        if (serviceType == "Money Deposit") return 5;
        if (serviceType == "Loan Request") return 10;
        if (serviceType == "Emergency Case") return 3;
        if (serviceType == "VIP") return 3;
        return 5; // Default
    }

    // Getters
    int getId() { return id; }
    string getServiceType() { return serviceType; }
    bool isEmergency() { return emergencyStatus; }
    int getWaitingTime() { return waitingTime; }
    double getPriorityScore() { return priorityScore; }

    // Display individual details
    string toString() {
        string status = emergencyStatus ? "Yes" : "No";
        return "ID: " + to_string(id) +
            ", Service: " + serviceType +
            ", Emergency: " + status +
            ", Waiting: " + to_string(waitingTime) + " min" +
            ", Priority: " + to_string(priorityScore);
    }
};

// Node structure for linked list
struct Node {
    Individual item;    // The individual
    Node* next;         // Pointer to next node

    // Constructor
    Node(Individual _item) {
        item = _item;
        next = nullptr;
    }
};

// Priority Queue class using Multi-Level Queue (MLQ) with custom linked lists
class PriorityQueue {
private:
    // Array of 3 linked lists for High (0), Medium (1), Low (2) priorities
    Node* fronts[3];    // Front pointers for each queue
    Node* rears[3];     // Rear pointers for each queue
    int size;           // Total number of individuals

    // Decide which queue to use (0: High, 1: Medium, 2: Low)
    int getQueueIndex(Individual ind) {
        double score = ind.getPriorityScore();
        // High priority: VIP, Emergency Case, or emergency status
        if (ind.getServiceType() == "VIP" || ind.getServiceType() == "Emergency Case" || ind.isEmergency()) {
            return 0;
        }
        // Medium priority: Loan Request, Money Deposit, or score >= 3.0 (but not High)
        else if (ind.getServiceType() == "Loan Request" || ind.getServiceType() == "Money Deposit" || score >= 3.0) {
            return 1;
        }
        // Low priority: Cash Withdrawal or score < 3.0
        else {
            return 2;
        }
    }

public:
    // Constructor
    PriorityQueue() {
        for (int i = 0; i < 3; i++) {
            fronts[i] = nullptr;
            rears[i] = nullptr;
        }
        size = 0;
    }

    // Destructor to free memory
    ~PriorityQueue() {
        Individual temp;
        while (dequeue(temp)) {
            // Keep dequeuing to free all nodes
        }
    }

    // Check if all queues are empty
    bool isEmpty() {
        return size == 0;
    }

    // Get total number of individuals
    int getSize() {
        return size;
    }

    // Add an individual to the correct queue
    bool enqueue(Individual newEntry) {
        Node* newNode = new Node(newEntry);
        int index = getQueueIndex(newEntry);

        // If queue is empty, set front and rear
        if (fronts[index] == nullptr) {
            fronts[index] = newNode;
            rears[index] = newNode;
        }
        else {
            // Add to rear
            rears[index]->next = newNode;
            rears[index] = newNode;
        }

        size++;
        return true;
    }

    // Remove and return the highest-priority individual
    bool dequeue(Individual& frontEntry) {
        if (isEmpty()) {
            return false;
        }

        // Check queues in priority order
        for (int i = 0; i < 3; i++) {
            if (fronts[i] != nullptr) {
                Node* temp = fronts[i];
                frontEntry = temp->item;
                fronts[i] = fronts[i]->next;

                // Update rear if queue becomes empty
                if (fronts[i] == nullptr) {
                    rears[i] = nullptr;
                }

                delete temp;
                size--;
                return true;
            }
        }
        return false;
    }

    // View the highest-priority individual without removing
    bool peek(Individual& frontEntry) {
        if (isEmpty()) {
            return false;
        }

        // Check queues in priority order
        for (int i = 0; i < 3; i++) {
            if (fronts[i] != nullptr) {
                frontEntry = fronts[i]->item;
                return true;
            }
        }
        return false;
    }

    // Update waiting times and reassign individuals
    void updateWaitingTimes(int minutes) {
        if (isEmpty()) {
            return;
        }

        // Temporary array to hold all individuals
        Individual* tempArray = new Individual[size];
        int index = 0;

        // Extract all individuals
        Individual temp;
        while (dequeue(temp)) {
            tempArray[index] = temp;
            index++;
        }

        // Update waiting times and re-enqueue
        for (int i = 0; i < index; i++) {
            tempArray[i].incrementWaitingTime(minutes);
            enqueue(tempArray[i]);
        }

        // Free temporary array
        delete[] tempArray;
    }

    // Print all queues
    void printQueue() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }

        cout << "\nMulti-Level Queue Contents (High to Low Priority):" << endl;
        cout << "+------+----------------------+------------+-------------+---------------+" << endl;
        cout << "| ID   | Service Type         | Emergency  | Wait Time   | Priority Score |" << endl;
        cout << "+------+----------------------+------------+-------------+---------------+" << endl;

        // Print each queue
        for (int i = 0; i < 3; i++) {
            if (fronts[i] != nullptr) {
                string level = (i == 0 ? "High" : (i == 1 ? "Medium" : "Low"));
                cout << "Priority Level: " << level << endl;
                Node* current = fronts[i];
                while (current != nullptr) {
                    Individual ind = current->item;
                    cout << "| " << setw(4) << ind.getId() << " | "
                        << setw(20) << ind.getServiceType() << " | "
                        << setw(10) << (ind.isEmergency() ? "Yes" : "No") << " | "
                        << setw(11) << ind.getWaitingTime() << " | "
                        << setw(13) << fixed << setprecision(2) << ind.getPriorityScore() << " |" << endl;
                    current = current->next;
                }
            }
        }

        cout << "+------+----------------------+------------+-------------+---------------+" << endl;
    }
};

// Analyze time complexity
void analyzeTimeComplexity() {
    cout << "\n=== Time Complexity Analysis ===" << endl;
    cout << "1. Enqueue Operation: O(1)" << endl;
    cout << "   - Adds to the rear of the correct queue based on priority" << endl;
    cout << "2. Dequeue Operation: O(1)" << endl;
    cout << "   - Removes from the front of the highest-priority queue" << endl;
    cout << "3. UpdateWaitingTimes: O(n)" << endl;
    cout << "   - Dequeues all individuals (O(n)), updates times, and re-enqueues (O(n))" << endl;
    cout << "4. Scalability:" << endl;
    cout << "   - Fast enqueue/dequeue for large queues" << endl;
    cout << "   - Linear updates suitable for frequent priority changes" << endl;
    cout << "   - Simple design for lab discussions" << endl;
}

// Justify data structure choice
void justifyDataStructure() {
    cout << "\n=== Data Structure Justification ===" << endl;
    cout << "1. Multi-Level Queue (MLQ):" << endl;
    cout << "   - Uses 3 linked lists for High, Medium, Low priorities" << endl;
    cout << "   - Fast O(1) enqueue/dequeue operations" << endl;
    cout << "   - Supports VIP and emergency prioritization" << endl;
    cout << "2. Fairness:" << endl;
    cout << "   - Waiting time (0.2/min) moves low-priority individuals up" << endl;
    cout << "   - Updates after each service prevent long waits" << endl;
    cout << "3. Alternatives:" << endl;
    cout << "   - Single Linked List: Slow O(n) enqueue, O(n²) updates" << endl;
    cout << "   - Heap: O(log n) operations, more complex for lab" << endl;
    cout << "4. Why MLQ:" << endl;
    cout << "   - Simple and efficient for priority-based queues" << endl;
}

// Test case with 8 individuals
void runAdvancedTestCase() {
    cout << "\n=== Advanced Test Case ===" << endl;

    // Create priority queue
    PriorityQueue priorityQueue;

    // Create 8 test individuals
    Individual ind1(1, "Cash Withdrawal", false, 10);  // Low priority
    Individual ind2(2, "Loan Request", false, 15);     // Medium priority
    Individual ind3(3, "Emergency Case", true, 5);     // High priority
    Individual ind4(4, "Money Deposit", false, 20);    // Medium priority
    Individual ind5(5, "VIP", false, 2);               // High priority
    Individual ind6(6, "Loan Request", false, 30);     // Medium priority
    Individual ind7(7, "Cash Withdrawal", false, 5);   // Low priority
    Individual ind8(8, "Cash Withdrawal", false, 15);  // Low priority

    // Enqueue all individuals
    cout << "Enqueueing 8 individuals with different priorities..." << endl;
    priorityQueue.enqueue(ind1);
    priorityQueue.enqueue(ind2);
    priorityQueue.enqueue(ind3);
    priorityQueue.enqueue(ind4);
    priorityQueue.enqueue(ind5);
    priorityQueue.enqueue(ind6);
    priorityQueue.enqueue(ind7);
    priorityQueue.enqueue(ind8);

    // Show initial queue
    cout << "\nInitial Queue State:" << endl;
    priorityQueue.printQueue();

    // Process queue with service times
    cout << "\nProcessing the queue with service times..." << endl;
    Individual served;
    int count = 1;
    int totalTime = 0;

    while (priorityQueue.dequeue(served)) {
        int serviceTime = served.getServiceDuration();
        cout << "\nServing #" << count << " (Time: " << totalTime << " min): "
            << served.toString() << ", Service Time: " << serviceTime << " min" << endl;

        // Update waiting times
        priorityQueue.updateWaitingTimes(serviceTime);
        totalTime += serviceTime;

        // Show remaining queue
        cout << "Remaining queue:" << endl;
        priorityQueue.printQueue();
        count++;
    }

    cout << "\nAll individuals served in " << totalTime << " minutes." << endl;
}

// Main function
int main() {
    cout << "===== Smart Queue Management System - Phase 2 (MLQ) =====" << endl;
    cout << "This system uses a multi-level priority queue with:" << endl;
    cout << "1. VIP status (6.0 points)" << endl;
    cout << "2. Emergency status (5.0 points)" << endl;
    cout << "3. Service type (1.0-3.0 points)" << endl;
    cout << "4. Waiting time (0.2 points/min)" << endl;
    cout << "5. Service times (2-10 min)" << endl;

    // Run test case
    runAdvancedTestCase();

    // Analyze and justify
    analyzeTimeComplexity();
    justifyDataStructure();

    return 0;
}
