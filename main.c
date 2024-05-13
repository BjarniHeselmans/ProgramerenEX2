#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definieer de structuur voor een item
struct Item {
  char *index;
  char *name;
  char *url;
};

// Functie om een JSON-string te parsen
struct Item *parseJSON(const char *jsonString, int *count) {
  struct Item *items = NULL;

  // Tellen van het aantal items
  *count = 0;
  const char *ptr = jsonString;
  while ((ptr = strstr(ptr, "\"index\"")) != NULL) {
    (*count)++;
    ptr++;
  }

  // Alloceren van geheugen voor items
  items = malloc(*count * sizeof(struct Item));
  if (items == NULL) {
    perror("Kan geheugen niet toewijzen");
    exit(EXIT_FAILURE);
  }

  // Parseren van JSON en invullen van de items
  ptr = jsonString;
  for (int i = 0; i < *count; i++) {
    // Zoek "index"
    ptr = strstr(ptr, "\"index\":");
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item %d\n", i);
      exit(EXIT_FAILURE);
    }
    ptr += strlen("\"index\":");
    ptr = strchr(ptr, '\"') + 1; // Ga naar het volgende aanhalingsteken
    const char *indexStart = ptr;
    ptr = strchr(ptr, '\"');
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item %d\n", i);
      exit(EXIT_FAILURE);
    }
    const char *indexEnd = ptr;
    ptr++; // Ga voorbij het aanhalingsteken

    // Kopieer de index naar de struct
    size_t indexLen = indexEnd - indexStart;
    items[i].index = malloc((indexLen + 1) * sizeof(char));
    strncpy(items[i].index, indexStart, indexLen);
    items[i].index[indexLen] = '\0';

    // Zoek "name"
    ptr = strstr(ptr, "\"name\":");
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item %d\n", i);
      exit(EXIT_FAILURE);
    }
    ptr += strlen("\"name\":");
    ptr = strchr(ptr, '\"') + 1; // Ga naar het volgende aanhalingsteken
    const char *nameStart = ptr;
    ptr = strchr(ptr, '\"');
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item %d\n", i);
      exit(EXIT_FAILURE);
    }
    const char *nameEnd = ptr;
    ptr++; // Ga voorbij het aanhalingsteken

    // Kopieer de naam naar de struct
    size_t nameLen = nameEnd - nameStart;
    items[i].name = malloc((nameLen + 1) * sizeof(char));
    strncpy(items[i].name, nameStart, nameLen);
    items[i].name[nameLen] = '\0';

    // Zoek "url"
    ptr = strstr(ptr, "\"url\":");
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item %d\n", i);
      exit(EXIT_FAILURE);
    }
    ptr += strlen("\"url\":");
    ptr = strchr(ptr, '\"') + 1; // Ga naar het volgende aanhalingsteken
    const char *urlStart = ptr;
    ptr = strchr(ptr, '\"');
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item %d\n", i);
      exit(EXIT_FAILURE);
    }
    const char *urlEnd = ptr;
    ptr++; // Ga voorbij het aanhalingsteken

    // Kopieer de URL naar de struct
    size_t urlLen = urlEnd - urlStart;
    items[i].url = malloc((urlLen + 1) * sizeof(char));
    strncpy(items[i].url, urlStart, urlLen);
    items[i].url[urlLen] = '\0';
  }

  return items;
}

// Functie om de inhoud van een bestand te lezen
char *readFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Kan het bestand niet openen");
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = malloc(length + 1);
  if (buffer == NULL) {
    perror("Kan geheugen niet toewijzen");
    exit(EXIT_FAILURE);
  }

  fread(buffer, 1, length, file);
  fclose(file);
  buffer[length] = '\0';

  return buffer;
}

int main() {
  // Lees de inhoud van het bestand "tekst.JSON"
  char *jsonString = readFile("tekst.JSON");

  int count;
  struct Item *items = parseJSON(jsonString, &count);

  // Druk de geparste items af
  printf("Aantal items: %d\n", count);
  for (int i = 0; i < count; i++) {
    printf("Index: %s, Name: %s, URL: %s\n", items[i].index, items[i].name,
           items[i].url);
    free(items[i].index);
    free(items[i].name);
    free(items[i].url);
  }
  free(items);
  free(jsonString);

  return 0;
}
