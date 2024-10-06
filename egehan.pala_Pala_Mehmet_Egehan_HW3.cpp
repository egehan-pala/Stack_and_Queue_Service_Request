//CS204 Fall 2023 - HW3: Stacks & Queues and SU services

// Mehmet Egehan Pala 31089 07.11.2023

#include <iostream>
#include <fstream>       // for ifstream
#include <cctype>
#include <string>
#include <vector>
#include <sstream>  // std::stringstream


using namespace std;
//TODO: Modify this part to make sure to impelement the logic for 10 requests for instructors and 1 request for students

struct Command
{
    string text;
    Command* next;
    
    Command() {}
    
    Command(string t, Command* nxt )//constructor 
        :text(t), next(nxt)
    {}
};

struct func
{
    string name;
    Command* commands;
    func* next;
    
    func() {}
    
    func(string n, Command* c, func* nxt )//constructor 
        :name(n), commands(c), next(nxt)
    {}
};

struct instructor
{
    string name;
    int ID;
    string function;
    
    instructor() {}
    
    instructor(string n, int i, string f)//constructor 
        :name(n), ID(i), function(f)
    {}
};

struct student
{
    string name;
    int ID;
    string function;
    
    student() {}
    
    student(string n, int i, string f)//constructor 
        :name(n), ID(i), function(f)
    {}
};


struct priceCalc
{
    string name;
    int ID;
    int price;
    
    priceCalc() {}
    
    priceCalc(string n, int i,int p = 0 )//constructor 
        :name(n), ID(i), price(p)
    {}
};

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//Creating the class for the stacks that will be used

struct StackNode
{
    string decleration;
    StackNode *next;
};

class DynIntStack
{
private:
       StackNode *top;
public:
        DynIntStack(void);
        void push(string);
        void pop(string &);
        bool isEmpty(void);
};

DynIntStack::DynIntStack()
{
    top = nullptr;
}

bool DynIntStack::isEmpty(void)
{
    bool status;
    if (top == nullptr)
    {
        status = true;
    }
    else
    {
        status = false;
    }
    return status;
}
void DynIntStack::push(string decName)
{
    StackNode *newNode;
    // Allocate a new node & store Num
    newNode = new StackNode;
    newNode->decleration = decName;

    // If there are no nodes in the list
    // make newNode the first node
    if (isEmpty())
    {
        top = newNode;
        newNode->next = nullptr;
    }
    else // Otherwise, insert NewNode before top
    {
        newNode->next = top;
        top = newNode;
    }
}

void DynIntStack::pop(string &decName)
{
    StackNode *temp;
    if (isEmpty())
    {
        cout << "The stack is empty.\n";
    }
    else // pop value off top of stack
    {
        decName = top->decleration;

        temp = top->next;
        delete top;
        top = temp;
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

//queue class

struct QueueNode
{
    string name;
    int ID;
    string function;
    QueueNode *next;

    QueueNode(string nam, int id, string fnc, QueueNode *ptr = nullptr)
    {
        name = nam;
        ID = id;
        function = fnc;
        next = ptr;
    }
};

class DynIntQueue
{
private:
     // These track the front and rear of the queue.
     QueueNode *front;
     QueueNode *rear;
public:
     DynIntQueue(); // Constructor.
     // Member functions.
     void enqueue(string, int, string);
     void dequeue(string &, int &, string &);
     bool isEmpty() const;
     void clear();
};

DynIntQueue::DynIntQueue() // creating a new queue
{
    front = nullptr;
    rear = nullptr;
    #ifdef _DEBUG
         cout << "An empty queue has been created\n";
    #endif
}

bool DynIntQueue::isEmpty() const 
{
    if (front == nullptr)
    {
         return true;
    }
    else
    {
        return false;
    }
}
void DynIntQueue::enqueue(string personName, int personID, string personFunction)
{
    if (isEmpty()) //if the queue is empty
    { //make it the first element
        front = new QueueNode(personName, personID, personFunction);
        rear = front;
    }
    else //if the queue is not empty
    { //add it after rear
        rear->next = new QueueNode(personName, personID, personFunction);
        rear = rear->next;
    }
    #ifdef _DEBUG
        cout << num << " enqueued\n";
    #endif
}

void DynIntQueue::dequeue(string &personName, int &personID, string &personFunction)
{
    QueueNode *temp;
    if (isEmpty())
    {
        cout << "Attempting to dequeue on empty queue, exiting program...\n";
        exit(1);
    }
    else //if the queue is not empty
    {  //return front's value, advance front and delete old front
        personName = front->name;
        personID = front->ID;
        personFunction = front->function;
        temp = front;
        front = front->next;
        delete temp;
    }
}

void DynIntQueue::clear()
{
    string value1;
    int value2;   // Dummy variable for dequeue
    string value3;
    while(!isEmpty())
    {
         dequeue(value1, value2, value3); //delete all elements
    }
    #ifdef _DEBUG
         cout << "queue cleared\n";
    #endif
}


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


//creating the queues and the stack in global
DynIntStack programStack;
DynIntQueue instructorsQueue;
DynIntQueue studentsQueue;

//creating a vector to store prices
vector<priceCalc> storedPrices;

int countPrice = 0;

// Function to execute a function's sequence of commands
void processWorkload(func* currentFunction, func* head)
{
    int countOfCurrFuncSize = 0;
    Command* currentCommand = currentFunction->commands;
    while (currentCommand != nullptr)
    {
        // Process each command in the function
        string commandText = currentCommand->text;
        if (commandText.substr(0, 6) == "define")
        {
            // If the command is a definition, increment the price counter by 1
            countPrice++;
            // Push the function and command onto the stack
            programStack.push(currentFunction->name + " " + commandText);
            countOfCurrFuncSize++;
        }
        else if (commandText.substr(0, 4) == "call")
        {
             // If the command is a function call, increment the price counter by 5 and execute the called function
            countPrice += 5;
            

            string calledFunctionName = commandText.substr(5, commandText.length() - 6);
            calledFunctionName = calledFunctionName + ":";
            func* calledFunction = head;

            //find the matching function name with the call
            while (calledFunction && calledFunction->name != calledFunctionName)
            {
                calledFunction = calledFunction->next;
            }
            if (calledFunction && calledFunction->name == calledFunctionName)
            {
                
                cout << "Calling " << calledFunction->name << " from " << currentFunction->name << endl;

                // Recursively execute the called function
                processWorkload(calledFunction, head);
            }
            
        }
        else if (commandText == "print stack" || commandText == "print stack;")
        {
            // If the command is to print the stack, display the stack trace
            countPrice += 2;
            cout << "Executing print stack; command from " << currentFunction->name << endl;

            DynIntStack tempStack1;
            while (!programStack.isEmpty())//reversing the stack
            {
                string front;
                programStack.pop(front);
                tempStack1.push(front);

            }
            //printing the reversed stack
            cout << "PRINTING THE STACK TRACE:" << endl;
            while (!tempStack1.isEmpty())
            {
                string defCommand;
                tempStack1.pop(defCommand);
                cout << defCommand << endl;
                programStack.push(defCommand);// filling up the old stack that has been emptied.
            }
            
        }
        currentCommand = currentCommand->next;
    }
    cout << currentFunction->name << " is finished. Clearing the stack from it's data..." << endl;
    for (int i = 0; i < countOfCurrFuncSize; i++)
    {
        string dummyVariable;
        programStack.pop(dummyVariable);
    }
    
}


// Function to process service requests from instructors and students
void processWorkload(func* head)
{
    static int instructorRequestCount = 1; // counting the served instructor requestes 

     // If there are instructor requests and not the 10th request, process an instructor request
     // If instructor request count is multiples of 10 then go to the student queue
    
    if ((!instructorsQueue.isEmpty() && (instructorRequestCount % 11) != 0) || (studentsQueue.isEmpty() && !instructorsQueue.isEmpty()))
    {
        cout << endl;
        cout<<"Processing instructors queue..."<<endl;
        cout << endl;
        string personName;
        int personID = 0;
        string personFunction;

        instructorsQueue.dequeue(personName, personID, personFunction); // deleting the served request

        instructor instructorRequest;

        instructorRequest.name = personName;
        instructorRequest.ID = personID;
        instructorRequest.function = personFunction;
        
        
        cout << "Processing Prof. " << instructorRequest.name << "'s request (with ID " << instructorRequest.ID << ") of service (function):\n" << instructorRequest.function << endl;
        cout<<"-------------------------------------------------------"<<endl;

        func* currentFunction = head;
        
        //finding the matched function in the linkedlist
        while (currentFunction && currentFunction->name != instructorRequest.function)
        {
            currentFunction = currentFunction->next; 
        }
        
        if(currentFunction)
        {
            // if found call the recursive function
            processWorkload(currentFunction, head);
        }

        instructorRequestCount++;//incrementing the served request 

        //stroning the price that has been counted in the recursive function
        for(int i = 0; i < storedPrices.size(); i++)
        {
            if(storedPrices[i].ID == instructorRequest.ID)
            {
                storedPrices[i].price += countPrice;
            }
        }

        countPrice = 0;//setting the price to 0 again

        //clearing the program stack
        while(!programStack.isEmpty())
        {
            string variable;
            programStack.pop(variable);
        }

        cout<<"GOING BACK TO MAIN MENU"<<endl;
        
    }
    // If there are no instructor requests or it's the 10th request, process a student request
    else if (!studentsQueue.isEmpty())
    {
        cout << endl;
        cout<<"Instructors queue is empty. Proceeding with students queue..."<<endl;
        cout << endl;

        string personName;
        int personID = 0;
        string personFunction;

        studentsQueue.dequeue(personName, personID, personFunction);// deleting the served request

        student studentRequest;

        studentRequest.name = personName;
        studentRequest.ID = personID;
        studentRequest.function = personFunction;
        
        cout << "Processing " << studentRequest.name << "'s request (with ID " << studentRequest.ID << ") of service (function):\n" << studentRequest.function << endl;
        
        cout<<"-------------------------------------------------------"<<endl;
        
        func* currentFunction = head;
        
        //finding the matched function in the linkedlist
        while (currentFunction && currentFunction->name != studentRequest.function )
        {
            currentFunction = currentFunction->next;
        }
        
        if (currentFunction)
        {
             // if found call the recursive function
            processWorkload(currentFunction, head);
        }

        //stroning the price that has been counted in the recursive function
        for(int i = 0; i < storedPrices.size(); i++)
        {
            if(storedPrices[i].ID == studentRequest.ID)
            {
                storedPrices[i].price += countPrice;
            }
        }
        countPrice = 0;//setting the price to 0 again


        //clearing the program stack
        cout << "Clearing stack from it's data. " << endl;
        while(!programStack.isEmpty())
        {
            string variable;
            programStack.pop(variable);
        }
        cout<<"GOING BACK TO MAIN MENU"<<endl;
        
        instructorRequestCount++; //incrementing the served request 
        
    }
    else
    {
        cout<<"Both instructor's and student's queue is empty.\nNo request is processed."<<endl<<"GOING BACK TO MAIN MENU"<<endl;
        return;
    }
    
}
 


 // Function to read service definitions from files and create a linked list of functions
func* readInput(vector<vector<string> > &functions)
{
    // Prompt the user to open service defining files and read their content
    string line;
    char file_check;
    
    cout << "If you want to open a service (function) defining file, then press (Y/y) for 'yes', otherwise press any single key" << endl; cin >> file_check;
    // Loop to handle multiple file inputs
    while(file_check == 'y' || file_check == 'Y')
    {
        // Vector to store lines from the file
        vector<string> lines;
        static string fileInputNames;
        string fileInputName;
        cout << "Enter the file input name: "; cin >> fileInputName;

        // Open the file
        ifstream files(fileInputName);
        
        // Check if the file exists
        if(files.fail())
        {
            cout << "File does not exist." << endl; 
            exit(-1);
        }
        if (fileInputNames.find(fileInputName) != string::npos)
        {
            cout << "Entered the same file." << endl;
            exit(-1);
        }
        fileInputNames += (fileInputName + " ");
        // Read each line from the file and store in the 'lines' vector
        while(getline(files, line))
        {
            // Remove carriage return characters if present
            if (line.find("\r") != string::npos)
            {
                line = line.substr(0, line.length() - 1);
            }
            lines.push_back(line);
        }
        
        // Add the lines vector to the functions vector
        functions.push_back(lines);
        
        // Close the file
        files.close();
        
        cout << "Do you want to open another service defining file? Press (Y/y) for 'yes', otherwise press anykey" << endl; cin >> file_check;
    }
    
    // Check if any files were opened
    if (!functions.empty())
    {
        // Create the head of the linked list with the first function
        func* head = new func(functions[0][0], nullptr, nullptr);
        
        // Loop to iterate through each function definition
        func* currentFunction = head;
        for (int i = 0; i < functions.size(); i++)
        {
            
            func* temp = nullptr;
            Command* currentCommand = nullptr;

             // Loop to iterate through each line in a function definition
            for (int j = 0; j < functions[i].size(); j++)
            {
                
                if (j == 0)
                {
                     // If it's the first line, create a new function node
                    if (i > 0)
                    {
                        temp = new func(functions[i][0], nullptr, nullptr);
                        currentFunction->next = temp;
                        currentFunction = temp;
                    }
                    
                }
                
                else
                {
                     // If it's not the first line, create a new command node
                    Command* tempCommand = new Command(functions[i][j], nullptr);
                    
                    if (j == 1)
                    {
                        // If it's the second line, set it as the head of the commands in the function
                        currentFunction->commands = tempCommand;
                        currentCommand = currentFunction->commands;
                    }
                    else
                    {
                        // If it's not the second line, append it to the existing commands
                        currentCommand->next = tempCommand;
                        currentCommand = tempCommand;
                    }
                }
            }
        }
        
        cout << endl;
        cout << "------------------------------------------------------------------" << endl;
        cout << "PRINTIG AVAILABLE SERVICES (FUNCTIONS) TO BE CHOSEN FROM THE USERS" << endl;
        cout << "------------------------------------------------------------------" << endl;
        
        
        currentFunction = head;
        Command* currentCommand = nullptr;
        
        string comm;

        while (currentFunction != nullptr) {
            cout << endl;
            cout << currentFunction->name << endl; 

            currentCommand = currentFunction->commands;
            // Loop to iterate through each command in the function
            while (currentCommand != nullptr)
            {
                string commandText = currentCommand->text;
                // Remove the trailing semicolon, if present
                commandText = commandText.substr(0, commandText.length()-1);
                comm += commandText;
                if (currentCommand->next != nullptr)
                {
                    comm += ", ";
                }
                currentCommand = currentCommand->next;
            }
            // Display the commands for the function
            cout << comm << endl;
            comm = "";
            currentFunction = currentFunction->next;
        }
    return head;
    }
    else
    {
        // If no files are opened, display an error message and exit
        cout << "Did not entered any file" << endl;
        exit(-1);
    }
    
}

// Function to add an instructor's service request to the instructor's queue
void addInstructorWorkload(func* head)
{
    func* currentFunction = head;
    
    string funcToBeUsed;
    string instructorName;
    int instructorID = 0;
    
    // Prompt the user to add a service (function) that the instructor wants to use
    cout << "Add a service (function) that the intructor wants to use: "; cin >> funcToBeUsed;
    
    // Add a colon to match the function name format
    funcToBeUsed = funcToBeUsed + ":";

    // Search for the requested function in the linked list
    while (currentFunction && currentFunction->name != funcToBeUsed)
    {
        currentFunction = currentFunction->next;
    }

    // Check if the requested function exists
    if (currentFunction == nullptr)
    {
        
        cout << "The requested service (function) does not exist. " << endl;
        cout << "GOING BACK TO MAIN MENU" << endl;
        
        return;
    }
    else
    {
        // Prompt the user to enter the instructor's name and ID
        cout << "Give instructor's name: "; cin >> instructorName;
        cout << "Give instructor's ID (an int): "; cin >> instructorID;
        
        // Enqueue the instructor's request to the instructor's queue
        instructorsQueue.enqueue(instructorName, instructorID, funcToBeUsed);

        // Create a priceCalc object and store it in the storedPrices vector

        priceCalc instPrice (instructorName, instructorID, 0);
        storedPrices.push_back(instPrice);
        
        
        cout << "Prof. " << instructorName << "'s service request of " << funcToBeUsed << " has been put in the instructor's queue." << endl;
        cout << " Waiting to be served..." << endl;
    }
}

// Function to add a student's service request to the student's queue
void addStudentWorkload(func* head)
{
    func* currentFunction = head;
    
    string funcToBeUsed;
    string studentName;
    int studentID = 0;
    
    
    cout << "Add a service (function) that the student wants to use: "; cin >> funcToBeUsed;
    
    // Add a colon to match the function name format
    funcToBeUsed = funcToBeUsed + ":";
    
    // Search for the requested function in the linked list

    while (currentFunction && currentFunction->name != funcToBeUsed)
    {
        currentFunction = currentFunction->next;
    }

    // Check if the requested function exists
    if (currentFunction == nullptr)
    {
        
        cout << "The requested service (function) does not exist. " << endl;
        cout << "GOING BACK TO MAIN MENU" << endl;
        
        return;
    }
    else
    {
        cout << "Give student's name: "; cin >> studentName;
        cout << "Give student's ID (an int): "; cin >> studentID;
        
        // Enqueue the student's request to the student's queue
        studentsQueue.enqueue(studentName, studentID, funcToBeUsed);

        // Create a priceCalc object and store it in the storedPrices vector
        priceCalc studPrice (studentName, studentID, 0);
        storedPrices.push_back(studPrice);
        
        cout << studentName << "'s service request of " << funcToBeUsed << " has been put in the student's queue." << endl;
        cout << " Waiting to be served..." << endl;
    }
}


void displayUsers()
{
    if(storedPrices.empty())
    {
        cout << "No payments defined." << endl;
        return;
    }

    string duplicateChecker;

    for (int i = 0; i < storedPrices.size(); i++)
    {

        if (duplicateChecker.find(to_string(storedPrices[i].ID)) == string::npos )
        {
            cout << "Name: " << storedPrices[i].name << " ID: " << storedPrices[i].ID << " " << storedPrices[i].price << " TRY" << endl;
            duplicateChecker += (to_string(storedPrices[i].ID)  + " ");
        }
    }
    

}

//function to delete the function-Command linked list
void deleteLinkedlist(func* head)
{
    func* currentFunction = head;
    Command* currentCommand = nullptr;
    Command* temp = nullptr;
    while(head)
    {
        currentCommand = currentFunction->commands;
        while(currentCommand)
        {
            temp = currentCommand->next;
            currentFunction->commands = temp;

            delete currentCommand;
            currentCommand = temp;
        }
        currentFunction = head->next;
        delete head;
        head = currentFunction;
    }
}


int main()
{
    func* head = nullptr;
    vector<vector<string> > functions;
    
    head = readInput(functions);
    

    
    while (true){
       cout << endl;
         cout<<"**********************************************************************"<<endl
        <<"**************** 0 - EXIT PROGRAM                        *************"<<endl
        <<"**************** 1 - ADD AN INSTRUCTOR SERVICE REQUEST   *************"<<endl
        <<"**************** 2 - ADD A STUDENT SERVICE REQUEST       *************"<<endl
        <<"**************** 3 - SERVE (PROCESS) A REQUEST           *************"<<endl
        <<"**************** 4 - DISPLAY USER PAYMENTS               *************"<<endl
        <<"**********************************************************************"<<endl;
        cout << endl;
        int option;
        cout << "Pick an option from above (int number from 0 to 3): ";
        cin>>option;
        switch (option)
        {
            case 0:
                cout<<"PROGRAM EXITING ... "<<endl;
                deleteLinkedlist(head);
                exit(0);
            case 1:
                addInstructorWorkload(head);
                break;
            case 2:
                addStudentWorkload(head);
                break;
            case 3:
                processWorkload(head);
                break;
            case 4:
                displayUsers();
                break;
            default:
                cout<<"INVALID OPTION!!! Try again"<<endl;
        }
    }
    return 0;
}
