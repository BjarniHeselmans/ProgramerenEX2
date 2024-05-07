#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the struct to hold the parsed data
typedef struct {
  char index[100];
  char name[100];
  char url[100];
} SpellInfo;

// Define the linked list node
typedef struct Node {
  SpellInfo spell;
  struct Node *next;
} Node;

// Function to add a new node to the linked list
void addNode(Node **head, SpellInfo spell) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->spell = spell;
  newNode->next = NULL;

  if (*head == NULL) {
    *head = newNode;
  } else {
    Node *current = *head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = newNode;
  }
}

// Function to parse JSON and extract the required values
void parseJSON(char *jsonString, Node **head) {
  SpellInfo spell;

  char *jsonCopy = strdup(jsonString); // Make a copy of the JSON string
  char *token = strtok(jsonCopy, ",{}\":");
  while (token != NULL) {
    if (strcmp(token, "index") == 0) {
      token = strtok(NULL, ",{}\": ");
      strcpy(spell.index, token);
    } else if (strcmp(token, "name") == 0) {
      token = strtok(NULL, ",{}\": ");
      strcpy(spell.name, token);
    } else if (strcmp(token, "url") == 0) {
      token = strtok(NULL, ",{}\": ");
      strcpy(spell.url, token);
      // Add the parsed data to the linked list
      addNode(head, spell);
    }
    token = strtok(NULL, ",{}\":");
  }
  free(jsonCopy); // Free the copied string
}

// Function to print the linked list
void printList(Node *head) {
  Node *current = head;
  while (current != NULL) {
    printf("Index: %s\n", current->spell.index);
    printf("Name: %s\n", current->spell.name);
    printf("URL: %s\n\n", current->spell.url);
    current = current->next;
  }
}

int main() {
  // Create a linked list to store the parsed data
  Node *head = NULL;

    // JSON file names
  char jsonFiles[3][100] = {"sacred-flame.json", "acid-splash.json",
                            "file3.json"};

  // Parse each JSON file and populate the linked list
  for (int i = 0; i < 3; i++) {
    FILE *file = fopen(jsonFiles[i], "r");
    if (file == NULL) {
            printf("Error opening file: %s\n", jsonFiles[i]);
      continue;
    }
    // Determine the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the JSON string
    char *jsonString = (char *)malloc(fileSize + 1);

    // Read the file content into the JSON string
    fread(jsonString, 1, fileSize, file);
    jsonString[fileSize] = '\0'; // Null-terminate the string

    // Close the file
    fclose(file);

    // Parse the JSON and populate the linked list
    parseJSON(jsonString, &head);

    // Free the allocated memory
    free(jsonString);
  }

  // Print the linked list
  printList(head);
  return 0;
}