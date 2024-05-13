#include <ctype.h>
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
  // Tellen van het aantal items
  *count = 0;
  const char *ptr = jsonString;
  while ((ptr = strstr(ptr, "\"index\"")) != NULL) {
    (*count)++;
    ptr++;
  }

  // Alloceren van geheugen voor items
  struct Item *items = malloc(*count * sizeof(struct Item));
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
      fprintf(stderr, "Fout bij het lezen van item Index1\n");
      exit(EXIT_FAILURE);
    }
    ptr += strlen("\"index\":");
    ptr = strchr(ptr, '\"') + 1; // Ga naar het volgende aanhalingsteken
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item Index2\n");
      exit(EXIT_FAILURE);
    }
    const char *indexStart = ptr;
    ptr = strchr(ptr, '\"');
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item Index3\n");
      exit(EXIT_FAILURE);
    }
    const char *indexEnd = ptr;
    ptr++; // Ga voorbij het aanhalingsteken

    // Kopieer de index naar de struct
    size_t indexLen = indexEnd - indexStart;
    items[i].index = malloc((indexLen + 1) * sizeof(char));
    if (items[i].index == NULL) {
      perror("Kan geheugen niet toewijzen voor index");
      exit(EXIT_FAILURE);
    }
    strncpy(items[i].index, indexStart, indexLen);
    items[i].index[indexLen] = '\0';

    // Zoek "name"
    ptr = strstr(ptr, "\"name\":");
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item Name\n");
      exit(EXIT_FAILURE);
    }
    ptr += strlen("\"name\":");
    ptr = strchr(ptr, '\"') + 1; // Ga naar het volgende aanhalingsteken
    const char *nameStart = ptr;
    ptr = strchr(ptr, '\"');
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item Name\n");
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
      fprintf(stderr, "Fout bij het lezen van item URL\n");
      exit(EXIT_FAILURE);
    }
    ptr += strlen("\"url\":");
    ptr = strchr(ptr, '\"') + 1; // Ga naar het volgende aanhalingsteken
    const char *urlStart = ptr;
    ptr = strchr(ptr, '\"');
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item URL\n");
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
    perror("Kan de file niet openen");
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

int main(int argc, char *argv[]) {
  // Controleren of er voldoende argumenten zijn
  if (argc < 2) {
    printf("Usage: %s [equipment-files] [number-of-items] [-w max-weight] [-m "
           "money] [-c camp-file]\n",
           argv[0]);
    return 1;
  }

  // Initialiseren van variabelen
  char *equipmentFiles[argc];
  int numberOfItems = 1;
  char *maxWeightFlag = NULL;
  char *maxWeight = NULL;
  char *moneyFlag = NULL;
  char *money = NULL;
  char *campFileFlag = NULL;
  char *campFile = NULL;

  // Verwerken van de opgegeven opties
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-w") == 0) {
      maxWeightFlag = argv[i];
      maxWeight = argv[++i];
    } else if (strcmp(argv[i], "-m") == 0) {
      moneyFlag = argv[i];
      money = argv[++i];
    } else if (strcmp(argv[i], "-c") == 0) {
      campFileFlag = argv[i];
      campFile = argv[++i];
    } else if (isdigit(argv[i][0])) {
      numberOfItems = atoi(argv[i]);
    } else {
      equipmentFiles[i] = argv[i];
    }
  }

  // Lees de inhoud van elk opgegeven bestand
  for (int i = 1; i < argc; i++) {
    char *filename = equipmentFiles[i];
    char *fileContent = readFile(filename);

    // Parse de JSON en verkrijg de items
    int count;
    struct Item *items = parseJSON(fileContent, &count);

    printf("File: %s\n", filename);
    printf("Aantal items in deze file: %d\n", count);

    // Toon de eerste 'numberOfItems' items
    for (int j = 0; j < numberOfItems && j < count; j++) {
      printf("Item %d:\n", j + 1);
      printf("Index: %s\n", items[j].index);
      printf("Naam: %s\n", items[j].name);
      printf("URL: %s\n", items[j].url);
      printf("\n");
    }

    // free het geheugen
    for (int j = 0; j < count; j++) {
      free(items[j].index);
      free(items[j].name);
      free(items[j].url);
    }
    free(items);
    free(fileContent);
  }

  // Verwerken van andere opgegeven opties
  if (maxWeightFlag != NULL) {
    printf("Maximaal gewicht: %s\n", maxWeight);
  }
  if (moneyFlag != NULL) {
    printf("Beschikbaar geld: %s\n", money);
  }
  if (campFileFlag != NULL) {
    printf("Kampbestand: %s\n", campFile);
  }

  return 0;
}