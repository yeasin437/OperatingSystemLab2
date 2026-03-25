#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

struct Node
{
    int data;
    struct Node *prev;
    struct Node *next;
};

struct Node *head = NULL;

struct Node *createNode(int data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

void insert(int data)
{
    struct Node *newNode = createNode(data);

    if (head == NULL)
    {
        head = newNode;
        return;
    }

    struct Node *temp = head;

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->prev = temp;
}

void insertAfter(int prevData, int newData)
{
    struct Node *temp = head;

    while (temp != NULL && temp->data != prevData)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Node %d not found.\n", prevData);
        return;
    }

    struct Node *newNode = createNode(newData);

    newNode->next = temp->next;
    newNode->prev = temp;

    if (temp->next != NULL)
    {
        temp->next->prev = newNode;
    }

    temp->next = newNode;
}

void deleteNode(int data)
{
    struct Node *temp = head;

    while (temp != NULL && temp->data != data)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Node %d not found.\n", data);
        return;
    }

    if (temp->prev != NULL)
    {
        temp->prev->next = temp->next;
    }
    else
    {
        head = temp->next;
        if (head != NULL)
        {
            head->prev = NULL;
        }
    }

    if (temp->next != NULL)
    {
        temp->next->prev = temp->prev;
    }

    free(temp);
}

void printForward()
{
    struct Node *temp = head;

    printf("Linked list in forward order: ");

    while (temp != NULL)
    {
        printf("%d ", temp->data);
        temp = temp->next;
    }

    printf("\n");
}

void printReverse()
{
    struct Node *temp = head;

    if (temp == NULL)
    {
        printf("Linked list in reverse order: \n");
        return;
    }

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    printf("Linked list in reverse order: ");

    while (temp != NULL)
    {
        printf("%d ", temp->data);
        temp = temp->prev;
    }

    printf("\n");
}

void freeList()
{
    struct Node *temp = head;
    struct Node *nextNode;

    while (temp != NULL)
    {
        nextNode = temp->next;
        free(temp);
        temp = nextNode;
    }
}

int main(int argc, char *argv[])
{
    int fd;
    char buffer[1024];
    int bytesRead;
    char *token;
    int choice;

    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        printf("Error opening file.\n");
        return 1;
    }

    bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0)
    {
        printf("Error reading file.\n");
        close(fd);
        return 1;
    }

    buffer[bytesRead] = '\0';
    close(fd);

    token = strtok(buffer, " \n\t\r");
    while (token != NULL)
    {
        insert(atoi(token));
        token = strtok(NULL, " \n\t\r");
    }

    printf("Initial linked list:\n");
    printForward();

    do
    {
        printf("\nWhat else do you want?\n");
        printf("To insert a new node, press 1\n");
        printf("To insert a new node after a given node, press 2\n");
        printf("To delete a node, press 3\n");
        printf("To exit, press 0\n");

        scanf("%d", &choice);

        if (choice == 1)
        {
            int num;

            printf("Insert your node number: ");
            scanf("%d", &num);

            insert(num);
            printForward();
        }
        else if (choice == 2)
        {
            int newNum, prevNum;

            printf("Insert your new node number and previous node number: ");
            scanf("%d %d", &newNum, &prevNum);

            insertAfter(prevNum, newNum);
            printForward();
        }
        else if (choice == 3)
        {
            int num;

            printf("Insert the node number you want to delete: ");
            scanf("%d", &num);

            deleteNode(num);
            printForward();
        }
        else if (choice == 0)
        {
            printForward();
            printReverse();
        }
        else
        {
            printf("Invalid choice.\n");
        }

    } while (choice != 0);

    freeList();

    return 0;
}
