/*
 * Name: Zainab Parveen   Reg No: 550827   Section: BS-AI-2A
 *
 * Chosen: 
 * Students Data:  BST (unique roll = key, O(log n) avg search/insert, sorted display via in-order, no size limit).
 * Help requests: Queue (FIFO: earliest request served first, O(1) ops).
 *
 */

#include <iostream>
#include <string>
using namespace std;

struct Student {
    int roll;
    string name;
    Student *left, *right;
    Student(int r, const string& n) : roll(r), name(n), left(nullptr), right(nullptr) {}
};

//=================BST========================
//Insertion for Node in BST
Student* insert(Student* node, int roll, const string& name, bool& ok) {
    if (!node) { ok = true; return new Student(roll, name); }
    if (roll < node->roll)      node->left  = insert(node->left, roll, name, ok);
    else if (roll > node->roll) node->right = insert(node->right, roll, name, ok);
    else ok = false;                       //to deal with duplicate roll number
    return node;
}

//Searching for Node in BST
Student* search(Student* node, int roll) {
    while (node) {
        if (roll == node->roll) return node;
        node = (roll < node->roll) ? node->left : node->right;
    }
    return nullptr;
}

//Display (in in-order)
void inorder(Student* node) {
    if (!node) return;
    inorder(node->left);
    cout << "  Roll: " << node->roll << "  Name: " << node->name << endl;
    inorder(node->right);
}

//Destroy tree (in post-order)
void destroyTree(Student* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}


//=====================QUEUE========================
struct QNode {
    int roll;
    QNode* next;
    QNode(int r) : roll(r), next(nullptr) {}
};

struct Queue {
    QNode *front, *rear;
    Queue() : front(nullptr), rear(nullptr) {}
};

bool isEmpty(Queue& q) { return q.front == nullptr; }

void enqueue(Queue& q, int roll) {          // O(1)
    QNode* n = new QNode(roll);
    if (q.rear) q.rear->next = n;
    else        q.front = n;
    q.rear = n;
}

int dequeue(Queue& q) {                     // O(1), call only if not empty
    QNode* temp = q.front;
    int roll = temp->roll;
    q.front = q.front->next;
    if (!q.front) q.rear = nullptr;         //queue became empty
    delete temp;
    return roll;
}

void showQueue(Queue& q) {
    if (isEmpty(q)) { cout << "  No pending requests.\n"; return; }
    int i = 1;
    for (QNode* c = q.front; c; c = c->next)
        cout << "  " << i++ << ". Roll " << c->roll << endl;
}

void destroyQueue(Queue& q) {
    while (!isEmpty(q)) dequeue(q);
}

//=====================MAIN========================
int main() {
    Student* root = nullptr;
    Queue requests;
    int choice;

    do {
        cout << "\n===== Student Help Desk =====\n"
             << "1. Add student\n"
             << "2. Search student\n"
             << "3. Display all students\n"
             << "4. Add help request\n"
             << "5. Serve next request\n"
             << "6. Show pending requests\n"
             << "0. Exit\n"
             << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            int roll; string name; bool ok = false;
            cout << "Roll number: "; cin >> roll;
            cout << "Name: "; cin.ignore(); getline(cin, name);
            root = insert(root, roll, name, ok);
            if (ok) cout << "Student added.\n";
            else    cout << "Error: roll " << roll << " already exists.\n";
        }
        else if (choice == 2) {
            int roll;
            cout << "Roll number: "; cin >> roll;
            Student* s = search(root, roll);
            if (s) cout << "Found -> Roll: " << s->roll << ", Name: " << s->name << endl;
            else   cout << "Roll " << roll << " not found.\n";
        }
        else if (choice == 3) {
            if (!root) cout << "No students registered.\n";
            else { cout << "Students (sorted):\n"; inorder(root); }
        }
        else if (choice == 4) {
            int roll;
            cout << "Roll number requesting help: "; cin >> roll;
            if (search(root, roll)) {
                enqueue(requests, roll);    //repeats allowed
                cout << "Request added for roll " << roll << ".\n";
            } else cout << "Rejected: roll " << roll << " is not registered.\n";
        }
        else if (choice == 5) {
            if (isEmpty(requests)) cout << "No pending requests.\n";
            else {
                int roll = dequeue(requests);
                Student* s = search(root, roll);
                cout << "Serving: Roll " << roll << " (" << (s ? s->name : "?") << ")\n";
            }
        }
        else if (choice == 6) {
            cout << "Pending requests (front to rear):\n";
            showQueue(requests);
        }
        else if (choice != 0) cout << "Invalid option.\n";
    } while (choice != 0);

    destroyTree(root);          //free memory before exit
    destroyQueue(requests);     //free request queue memory
    cout << "Goodbye.\n";
    return 0;
}