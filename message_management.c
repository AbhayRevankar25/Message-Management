#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_SIZE 256

// structure which represents a single message   ( QUEUE STRUCTURE)

typedef struct message 
{
    char sender[50];
    char content[MAX_SIZE];
    struct message* next;
}MESSAGE;


// structure to repesent a Queue of messages   ( QUEUE STRUCTURE )

typedef struct messageQueue 
{
    MESSAGE* front;
    MESSAGE* rear;

}MESSAGEQueue;


// structure to represent a User ( AVL TREE STRUCTURE)

typedef struct user 
{
    char username[50];
    MESSAGEQueue inbox;
    struct user* left;
    struct user* right;
    int height;  

}User;


// to create a new message node

MESSAGE* createMESSAGE(const char* sender, const char* content) 
{
    MESSAGE* newMESSAGE = (MESSAGE*)malloc(sizeof(MESSAGE));
    if (newMESSAGE == NULL) {
        printf("  Memory allocation failed.\n");
        exit(0);
    }
    strncpy(newMESSAGE->sender, sender, sizeof(newMESSAGE->sender) - 1);
    newMESSAGE->sender[sizeof(newMESSAGE->sender) - 1] = '\0'; // Ensure null-termination
    strncpy(newMESSAGE->content, content, MAX_SIZE);
    newMESSAGE->content[MAX_SIZE - 1] = '\0'; // Ensure null-termination
    newMESSAGE->next = NULL;
    return newMESSAGE;
}


// function used to enqueue or insert message into the queue

void enqueueMESSAGE(MESSAGEQueue* queue, const char* sender, const char* content) 
{
    MESSAGE* newMESSAGE = createMESSAGE(sender, content);
    if (queue->front == NULL) {
        queue->front = queue->rear = newMESSAGE;
    } else {
        queue->rear->next = newMESSAGE;
        queue->rear = newMESSAGE;
    }
}


// function to display all messages present in the queue ( means of each user)

void displayMESSAGEs(MESSAGEQueue* queue) 
{
    MESSAGE* current = queue->front;
    while (current != NULL) 
    {
        printf("  From: %s\n", current->sender);
        printf("  Content: %s\n\n", current->content);
        current = current->next;
    }
}
 
// function to create a new user node in avl tree 

User* createUser(const char* username) 
{
    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        printf( "  Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(newUser->username, username, sizeof(newUser->username) - 1);
    newUser->username[sizeof(newUser->username) - 1] = '\0'; // Ensure null-termination
    newUser->inbox.front = newUser->inbox.rear = NULL;
    newUser->left = newUser->right = NULL;
    newUser->height = 1; // New node is initially at height 1
    return newUser;
}


// function to get the height of node ( user in avl tree)

int height(User* node) {
    return (node == NULL) ? 0 : node->height;
}

// function to perform a right rotation

User* rightRotate(User* y) 
{
    User* x = y->left;
    User* T2 = x->right;

    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    return x;
}

// function to perform a left rotation

User* leftRotate(User* x) 
{
    User* y = x->right;
    User* T2 = y->left;

    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    return y;
}


// function to get the balance factor of a node

int getBalance(User* node) 
{
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

// function to insert a user into the AVL tree
User* insertUser(User* root, const char* username) 
{
    if (root == NULL) 
    {
        return createUser(username);
    }

    
    // inserting a node to left or right based on user name

    if (strcmp(username, root->username) < 0) 
    {
        root->left = insertUser(root->left, username);
    } 
    else if (strcmp(username, root->username) > 0) 
    {
        root->right = insertUser(root->right, username);
    } 
    else 
    {
        // duplicate user 
        return root;
    }

    // update height of the current node
    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));


    // get the balance factor to check if rotation is required
    int balance = getBalance(root);

    // left left case in avl tree
    if (balance > 1 && strcmp(username, root->left->username) < 0) 
    {
        return rightRotate(root);
    }

    // right right case in avl tree
    if (balance < -1 && strcmp(username, root->right->username) > 0) 
    {
        return leftRotate(root);
    }

    // left right case in avl tree
    if (balance > 1 && strcmp(username, root->left->username) > 0) 
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // right left case in avl tree
    if (balance < -1 && strcmp(username, root->right->username) < 0) 
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


// function to send a message to a user

void sendMESSAGE(User* root, const char* sender, const char* receiver, const char* content) 
{
    // to search the receiver in the AVL tree
    User* receiverNode = root;
    while (receiverNode != NULL) 
    {
        if (strcmp(receiver, receiverNode->username) < 0) 
        {
            receiverNode = receiverNode->left;
        } 
        else if (strcmp(receiver, receiverNode->username) > 0) 
        {
            receiverNode = receiverNode->right;
        } 
        else 
        {
            // found the receiver, enqueue the message to the inbox
            enqueueMESSAGE(&receiverNode->inbox, sender, content);
            printf("  The MESSAGE has been sent successfully\n");
            return;
        }
    }

    printf("  User '%s' not found.\n", receiver);
}


// indox box ( to display messages of user )

void displayInbox(User* root, const char* username) 
{
    // search the user in the AVL tree
    User* userNode = root;
    while (userNode != NULL) 
    {
        if (strcmp(username, userNode->username) < 0) 
        {
            userNode = userNode->left;
        } 
        else if (strcmp(username, userNode->username) > 0) 
        {
            userNode = userNode->right;
        } 
        else 
        {
            // the user is found and calling display message to display information of message
            displayMESSAGEs(&userNode->inbox);
            return;
        }
    }

    printf("  User '%s' not found.\n", username);
}


// function to display all messages of all user 

void displayAllMESSAGEs(User* root) 
{
    if (root != NULL) 
    {
        // traverse to the left sub tree
        displayAllMESSAGEs(root->left);

        // display message of current user
        printf("  User: %s\n", root->username);
        displayMESSAGEs(&root->inbox);
        printf("\n");

        // transvrese to right sub tree
        displayAllMESSAGEs(root->right);
    }
}

// filtering the content based on word search

void searchMESSAGEs(User* root, const char* username, const char* searchTerm) 
{
    // searc the user in the AVL tree
    User* userNode = root;
    while (userNode != NULL) 
    {
        if (strcmp(username, userNode->username) < 0) 
        {
            userNode = userNode->left;
        } 
        else if (strcmp(username, userNode->username) > 0) 
        {
            userNode = userNode->right;
        } 
        else 
        {
            //  the user is found , search for the word in MESSAGEES
            MESSAGE* current = userNode->inbox.front;
            int found = 0;

            printf("  Search results for '%s' in Message for user '%s':\n", searchTerm, username);

            while (current != NULL) 
            {
                if (strstr(current->content, searchTerm) != NULL) 
                {  
                    printf("  Sender: %s\n", current->sender);
                    printf("  Content: %s\n\n", current->content);
                    found = 1;
                }
                current = current->next;
            }

            if (!found) 
            {
                printf("  No Message found containing the term '%s'.\n", searchTerm);
            }

            return;
        }
    }

    printf("                     User '%s' not found.\n", username);
}


// function to free the memeory of avl and queue in it
void freeTree(User* root) 
{
    if (root != NULL) 
    {
        // Free  node
        while (root->inbox.front != NULL) 
        {
            MESSAGE* temp = root->inbox.front;
            root->inbox.front = root->inbox.front->next;
            free(temp);
        }

        // free the left and right sub tree
        freeTree(root->left);
        freeTree(root->right);

        // free node
        free(root);
    }
}

int main() 
{
    // create a user root node ( avl head tree node )
    User* root = NULL;

    int choice = 0;
    while(choice != 4)
    {
        // 1. to create the user to send and recieve the message in this program
        // 2. message box which contains option to send , see and search message
        // 3. displays all the messages ( user , from , message)
        // 4. Exit 

        system("cls");
        system("COLOR 30");
        printf("\n                                                              Welcome to message system\n\n");
        printf("\n  1. Create A User \n");
        printf("\n  2. Message Box\n");
        printf("\n  3. Display Message of all user \n");
        printf("\n  4. Exit\n\n\n");
        printf("=>  ");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
            {
                system("cls");
                system("COLOR 90");
                char name[50];
                printf("\n\n                                                   Creating a new User \n\n\n\n");
                printf("  Enter your name : ");
                scanf("%s",name);
                root = insertUser(root,name);
                printf("\n\n\n                                                     User has been created\n");
                printf("\n  Press to Continue.....\n");
                getch();
            }
            break;
            case 2:
            {
                // 1. to sent message to user if user is there then he recieves message if not then a message is printed that the user is not there
                // 2. to see the revieved message
                // 3. search for message on the particular word
                system("cls");
                system("COLOR B0");
                printf("\n\n                                                MESSAGE BOX \n\n\n");
                char name[50];
                printf("  Enter Your Name : ");
                scanf("%s",name);
                int userChoice = 0;
                while(userChoice != 4)
                {
                    printf("\n\n\n  1. Send a message\n");
                    printf("\n  2. See the recieved message\n");
                    printf("\n  3. Search message based on word\n");
                    printf("\n  4. Exit\n\n\n");
                    printf("\n=> ");
                    scanf("%d",&userChoice);
                    switch(userChoice)
                    {
                        case 1:
                        {
                            char reciever[50];
                            char MESSAGE[MAX_SIZE];
                            printf("\n\n\n  Enter the name of Reciever : ");
                            scanf("%s",reciever);
                            printf("\n  Enter the Message \n");
                            printf("\n=> ");
                            getchar();
                            fgets(MESSAGE,sizeof(MESSAGE),stdin);
                            printf("\n\n\n");
                            sendMESSAGE(root,name,reciever,MESSAGE);
                            printf("\n  Press to continue...\n");
                            getch();
                        }
                        break;
                        case 2:
                        {
                            
                            printf("  Indox for %s \n\n",name);
                            displayInbox(root ,name);
                            printf("\n  Press to continue.....\n");
                            getch();
                        }
                        break;
                        case 3:
                        {
                           
                            char searchString[50];
                            printf("  Enter the word to search Message\n");
                            printf(" => ");
                            scanf("%s",searchString);
                            searchMESSAGEs(root,name,searchString);
                            printf("\n   Press to Continue");
                            getch();
                        }
                        break;
                    }
                }  
            }
             break;
            case 3:
            {
                system("cls");
                system("COLOR E0");
                printf("\n\n                                            Message of all User \n\n\n");
                if(root == NULL)
                {
                    printf("\n\n  No Message \n");
                }
                else
                {
                displayAllMESSAGEs(root);
                }
                printf("\n  Press to continue");
                getch();
            }
            break;
        }
    }
    freeTree(root);
    system("cls");
    system("COLOR  F0");
    printf("\n\n\n                      Thanking for using our application       \n\n");

    return 0;
}
