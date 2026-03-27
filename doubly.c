#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct node
{
    int data;
    struct node *prev;
    struct node *next;
};

struct node *head = NULL;

/* Insert at the end */
void insertAtEnd(int data)
{
    struct node *newNode;
    newNode = (struct node *)malloc(sizeof(struct node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (head == NULL)
    {
        head = newNode;
        return;
    }

    struct node *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->prev = temp;
}

/* Insert after a given node value */
void insertAfter(int data, int previous)
{
    struct node *newNode;
    newNode = (struct node *)malloc(sizeof(struct node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;

    struct node *currentNode = head;

    while (currentNode != NULL && currentNode->data != previous)
    {
        currentNode = currentNode->next;
    }

    if (currentNode == NULL)
    {
        printf("Previous node %d not found.\n", previous);
        free(newNode);
        return;
    }

    newNode->next = currentNode->next;
    newNode->prev = currentNode;

    if (currentNode->next != NULL)
    {
        currentNode->next->prev = newNode;
    }

    currentNode->next = newNode;
}

/* Delete at head */
void deleteAtHead()
{
    if (head == NULL)
    {
        return;
    }

    struct node *temp = head;
    head = head->next;

    if (head != NULL)
    {
        head->prev = NULL;
    }

    free(temp);
}

/* Delete by value */
void deleteByValue(int data)
{
    struct node *currentNode = head;

    if (currentNode == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    if (currentNode->data == data)
    {
        deleteAtHead();
        return;
    }

    while (currentNode != NULL && currentNode->data != data)
    {
        currentNode = currentNode->next;
    }

    if (currentNode == NULL)
    {
        printf("Node %d not found.\n", data);
        return;
    }

    if (currentNode->prev != NULL)
    {
        currentNode->prev->next = currentNode->next;
    }

    if (currentNode->next != NULL)
    {
        currentNode->next->prev = currentNode->prev;
    }

    free(currentNode);
}

/* Print forward */
void printList()
{
    struct node *temp = head;

    if (temp == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    while (temp != NULL)
    {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

/* Print reverse */
void printReverse()
{
    struct node *temp = head;

    if (temp == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    while (temp != NULL)
    {
        printf("%d ", temp->data);
        temp = temp->prev;
    }
    printf("\n");
}

/* Build list from file using open() and read() */
void buildListFromFile(char *filename)
{
    int fd;
    char buffer[1024];
    int bytesRead;
    char numberBuffer[50];
    int i, j = 0;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        printf("Unable to open file.\n");
        exit(1);
    }

    bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead < 0)
    {
        printf("Unable to read file.\n");
        close(fd);
        exit(1);
    }

    buffer[bytesRead] = '\0';
    close(fd);

    for (i = 0; i <= bytesRead; i++)
    {
        if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == '\0')
        {
            if (j > 0)
            {
                numberBuffer[j] = '\0';
                insertAtEnd(atoi(numberBuffer));
                j = 0;
            }
        }
        else
        {
            numberBuffer[j] = buffer[i];
            j++;
        }
    }
}

int main(int argc, char *argv[])
{
    int choice;
    int data, previous;

    if (argc != 2)
    {
        printf("Usage: %s input.txt\n", argv[0]);
        return 1;
    }

    buildListFromFile(argv[1]);

    printf("Initial linked list:\n");
    printList();

    while (1)
    {
        printf("\nWhat else do you want?\n");
        printf("To insert a new node, press 1\n");
        printf("To insert a new node after a given node, press 2\n");
        printf("To delete a node, press 3\n");
        printf("To exit, press 0\n");

        scanf("%d", &choice);

        if (choice == 1)
        {
            printf("Insert your node number: ");
            scanf("%d", &data);
            insertAtEnd(data);
            printf("Updated linked list:\n");
            printList();
        }
        else if (choice == 2)
        {
            printf("Insert your new node number and previous node number: ");
            scanf("%d %d", &data, &previous);
            insertAfter(data, previous);
            printf("Updated linked list:\n");
            printList();
        }
        else if (choice == 3)
        {
            printf("Insert the node number you want to delete: ");
            scanf("%d", &data);
            deleteByValue(data);
            printf("Updated linked list:\n");
            printList();
        }
        else if (choice == 0)
        {
            printf("Linked list in forward order:\n");
            printList();

            printf("Linked list in reverse order:\n");
            printReverse();

            break;
        }
        else
        {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
