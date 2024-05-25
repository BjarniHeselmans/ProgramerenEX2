#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definieer de structuur voor een item
struct Item {
  char *index;       // Index van het item
  char *name;        // Naam van het item
  char *url;         // URL van het item
  struct Item *next; // Pointer naar het volgende item voor linked list
};

// Functie om een JSON-string te parsen
struct Item *parseJSON(const char *jsonString, int *count) {
  // Tellen van het aantal items
  *count = 0;
  const char *ptr = jsonString;
  while ((ptr = strstr(ptr, "\"index\"")) != NULL) {
    (*count)++; // Incrementeer het aantal items
    ptr++;
  }

  // Alloceren van geheugen voor items
  struct Item *head = NULL; // Pointer naar het eerste item in de lijst
  struct Item *tail = NULL; // Pointer naar het laatste item in de lijst

  // Parseren van JSON en invullen van de items
  ptr = jsonString;
  for (int i = 0; i < *count; i++) {
    struct Item *newItem =
        malloc(sizeof(struct Item)); // Maak geheugen vrij voor een nieuw item
    if (newItem == NULL) { // Controleer of het geheugen correct is toegewezen
      perror("Kan geheugen niet toewijzen");
      exit(EXIT_FAILURE);
    }

    // Zoek "index"
    ptr = strstr(ptr, "\"index\":");
    if (ptr == NULL) {
      fprintf(stderr, "Fout bij het lezen van item Index1\n");
      exit(EXIT_FAILURE);
    }
    ptr += strlen("\"index\":");
    ptr = strchr(ptr, '\"') + 1; // Ga naar het volgende aanhalingsteken
    const char *indexStart = ptr;
    ptr = strchr(ptr, '\"');
    const char *indexEnd = ptr;
    ptr++; // Ga voorbij het aanhalingsteken

    // Kopieer de index naar de struct
    size_t indexLen = indexEnd - indexStart;
    newItem->index = malloc(
        (indexLen + 1) * sizeof(char)); // Alloceren van geheugen voor de index
    strncpy(newItem->index, indexStart,
            indexLen); // Kopieer de index naar de struct
    newItem->index[indexLen] = '\0';

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
    const char *nameEnd = ptr;
    ptr++; // Ga voorbij het aanhalingsteken

    // Kopieer de naam naar de struct
    size_t nameLen = nameEnd - nameStart;
    newItem->name = malloc((nameLen + 1) *
                           sizeof(char)); // Alloceren van geheugen voor de naam
    strncpy(newItem->name, nameStart,
            nameLen); // Kopieer de naam naar de struct
    newItem->name[nameLen] = '\0';

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
    const char *urlEnd = ptr;
    ptr++; // Ga voorbij het aanhalingsteken

    // Kopieer de URL naar de struct
    size_t urlLen = urlEnd - urlStart;
    newItem->url = malloc((urlLen + 1) *
                          sizeof(char)); // Alloceren van geheugen voor de URL
    strncpy(newItem->url, urlStart, urlLen); // Kopieer de URL naar de struct
    newItem->url[urlLen] = '\0';

    // Voeg het nieuwe item toe aan de linked list
    newItem->next = NULL; // Stel de next pointer in op NULL
    if (tail == NULL) {
      head = newItem; // Als dit het eerste item is, stel het in als het hoofd
                      // van de lijst
      tail = newItem; // Stel het ook in als het einde van de lijst
    } else {
      tail->next = newItem; // Koppel het nieuwe item aan het einde van de lijst
      tail =
          newItem; // Stel het nieuwe item in als het nieuwe einde van de lijst
    }
  }

  // Maak de lijst circulair
  if (tail != NULL) {
    tail->next = head; // Maak de laatste node in de lijst circulair door deze
                       // te koppelen aan het hoofd van de lijst
  }

  return head; // Return het hoofd van de lijst
}

// Functie om de inhoud van een bestand te lezen
char *readFile(const char *filename) {
  FILE *file = fopen(filename, "r"); // Open het bestand voor lezen
  if (file == NULL) {
    perror("Kan de file niet openen");
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);  // Ga naar het einde van het bestand
  long length = ftell(file); // Bepaal de lengte van het bestand
  fseek(file, 0, SEEK_SET);  // Ga terug naar het begin van het bestand

  char *buffer = malloc(
      length +
      1); // Alloceren van geheugen voor een buffer om het bestand in te lezen
  if (buffer == NULL) {
    perror("Kan geheugen niet toewijzen");
    exit(EXIT_FAILURE);
  }

  fread(buffer, 1, length, file); // Lees de inhoud van het bestand in de buffer
  fclose(file);                   // Sluit het bestand
  buffer[length] =
      '\0'; // Voeg een null terminator toe aan het einde van de buffer

  return buffer; // Return de gelezen inhoud van het bestand
}

// Functie om door de items te lopen en te tonen
void printItems(struct Item *head, int numberOfItems) {
  struct Item *current = head; // Start vanaf het hoofd van de lijst
  int count = 0;
  while (current != NULL &&
         count < numberOfItems) { // Blijf doorgaan zolang er items zijn en het
                                  // aantal te tonen items niet is overschreden
    printf("Item %d:\n", count + 1); // Print het itemnummer
    printf("Index: %s\n",
           current->index);              // Print de index van het huidige item
    printf("Naam: %s\n", current->name); // Print de naam van het huidige item
    printf("URL: %s\n", current->url);   // Print de URL van het huidige item
    printf("\n");
    current = current->next; // Ga naar het volgende item in de lijst
    count++;                 // Verhoog de telling van het aantal getoonde items
  }
}

// Functie om geheugen vrij te maken
void freeItems(struct Item *head) {
  if (head == NULL)
    return; // Als de lijst leeg is, doe niets

  struct Item *current = head; // Start vanaf het hoofd van de lijst
  struct Item *nextItem = NULL;
  do {
    nextItem = current->next; // Bewaar het volgende item voordat het huidige
                              // item wordt vrijgegeven
    free(current->index);     // Vrijgeven van geheugen voor de index van het
                              // huidige item
    free(
        current
            ->name); // Vrijgeven van geheugen voor de naam van het huidige item
    free(current
             ->url); // Vrijgeven van geheugen voor de URL van het huidige item
    free(current);   // Vrijgeven van geheugen voor het huidige item
    current = nextItem; // Ga naar het volgende item in de lijst
  } while (current != head &&
           current != NULL); // Blijf doorgaan totdat het einde van de lijst is
                             // bereikt of als de lijst leeg is

  // Let op: in een circulaire lijst zal 'current' uiteindelijk terugkomen bij
  // het hoofd van de lijst, maar we moeten toch controleren of 'current' niet
  // NULL is om te voorkomen dat we proberen geheugen vrij te maken voor een
  // ongeldige pointer.
}

int main(int argc, char *argv[]) {
  if (argc < 3) { // Controleert of er voldoende argumenten zijn door het aantal
                  // ontvangen argumenten te controleren
    printf("Usage: %s [equipment-files] [number-of-items] [-w max-weight] [-m "
           "money] [-c camp-file]\n",
           argv[0]); // Als er onvoldoende argumenten zijn, wordt het juiste
                     // gebruik weergegeven
    return 1; // Geeft een foutcode terug om aan te geven dat er een fout is
              // opgetreden
  }

  char *equipmentFiles[argc]; // Initialisatie van een array om de bestandsnamen
                              // op te slaan
  int numberOfItems = 1;      // Initialisatie van het aantal items
  char *maxWeightFlag =
      NULL; // Initialisatie van de vlag voor het maximale gewicht
  char *maxWeight =
      NULL; // Initialisatie van de waarde van het maximale gewicht
  char *moneyFlag = NULL;    // Initialisatie van de vlag voor het geldbedrag
  char *money = NULL;        // Initialisatie van de waarde van het geldbedrag
  char *campFileFlag = NULL; // Initialisatie van de vlag voor het kampbestand
  char *campFile = NULL;     // Initialisatie van de waarde van het kampbestand
  int currentItem = 1;       // Initialisatie van het huidige itemnummer

  int equipmentFileCount =
      0; // Initialisatie van de teller voor het aantal opgegeven bestanden
  for (int i = 1; i < argc;
       i++) { // Lus om door alle ontvangen argumenten te gaan
    if (strcmp(argv[i], "-w") == 0) { // Controleert of het huidige argument de
                                      // vlag voor het maximale gewicht is
      maxWeightFlag = argv[i]; // Wijs de vlag voor het maximale gewicht toe
      maxWeight = argv[++i]; // Wijs de waarde van het maximale gewicht toe aan
                             // de volgende argument
    } else if (strcmp(argv[i], "-m") ==
               0) { // Controleert of het huidige argument de vlag voor het
                    // geldbedrag is
      moneyFlag = argv[i]; // Wijs de vlag voor het geldbedrag toe
      money = argv[++i];   // Wijs de waarde van het geldbedrag toe aan de
                           // volgende argument
    } else if (strcmp(argv[i], "-c") ==
               0) { // Controleert of het huidige argument de vlag voor het
                    // kampbestand is
      campFileFlag = argv[i]; // Wijs de vlag voor het kampbestand toe
      campFile = argv[++i];   // Wijs de waarde van het kampbestand toe aan de
                              // volgende argument
    } else if (isdigit(argv[i][0])) { // Controleert of het eerste karakter van
                                      // het huidige argument een cijfer is
      numberOfItems = atoi(argv[i]);  // Zet het aantal items om van een string
                                      // naar een geheel getal
    } else if (strcmp(argv[i], "-item") ==
               0) { // Controleert of het huidige argument de vlag voor het
                    // huidige itemnummer is
      currentItem = atoi(argv[++i]); // Wijs het huidige itemnummer toe
    } else { // Als het huidige argument geen optie is, wordt het beschouwd als
             // een bestandsnaam
      equipmentFiles[equipmentFileCount++] =
          argv[i]; // Voeg het huidige argument toe aan de lijst van
                   // bestandsnamen
    }
  }

  struct Item *head = NULL; // Initialisatie van het hoofd van de gelinkte lijst
  struct Item *tail = NULL; // Initialisatie van het einde van de gelinkte lijst

  for (int i = 0; i < equipmentFileCount;
       i++) { // Lus om door elk opgegeven bestand te gaan
    char *filename = equipmentFiles[i]; // Haal de bestandsnaam op uit de lijst
                                        // van bestandsnamen
    char *fileContent =
        readFile(filename); // Lees de inhoud van het huidige bestand

    int count; // Variabele om het aantal items in het bestand bij te houden
    struct Item *items =
        parseJSON(fileContent, &count); // Parsen van de JSON-gegevens en
                                        // omzetten naar een lijst van items

    if (head == NULL) { // Als de gelinkte lijst leeg is
      head = items;     // Stel het hoofd van de lijst in op de items
      tail = items;     // Stel het einde van de lijst in op de items
      for (int j = 0; j < count - 1;
           j++) {          // Lus om naar het einde van de lijst te gaan
        tail = tail->next; // Ga naar het volgende item in de lijst
      }
    } else {              // Als de gelinkte lijst niet leeg is
      tail->next = items; // Koppel de items aan het einde van de lijst
      for (int j = 0; j < count;
           j++) { // Lus om naar het einde van de toegevoegde items te gaan
        tail = tail->next; // Ga naar het volgende item in de lijst
      }
      tail->next = head; // Maak de lijst circulair door
    }

    free(fileContent); // Vrijgeven van geheugen voor de inhoud van het bestand
  }

  struct Item *current = head; // Initialisatie van een pointer naar het huidige
                               // item, beginnend bij het hoofd van de lijst
  for (int i = 1; i < currentItem && current != NULL;
       i++) { // Lus om het huidige item te vinden op basis van het opgegeven
              // itemnummer
    current = current->next; // Ga naar het volgende item in de lijst
  }

  if (current != NULL) {      // Controleert of het huidige item is gevonden
    printf("Huidig item:\n"); // Drukt het label voor het huidige item af
    printf("Index: %s\n",
           current->index); // Drukt de index van het huidige item af
    printf("Naam: %s\n",
           current->name);             // Drukt de naam van het huidige item af
    printf("URL: %s\n", current->url); // Drukt de URL van het huidige item af
    printf("\n"); // Voegt een lege regel toe voor leesbaarheid
  } else {        // Als het huidige item niet is gevonden
    printf("Huidig itemnummer buiten bereik.\n"); // Drukt een foutmelding af
  }

  if (maxWeightFlag !=
      NULL) { // Controleert of de vlag voor het maximale gewicht is ingesteld
    printf("Maximaal gewicht: %s\n",
           maxWeight); // Drukt het maximale gewicht af
  }
  if (moneyFlag !=
      NULL) { // Controleert of de vlag voor het geldbedrag is ingesteld
    printf("Beschikbaar geld: %s\n", money); // Drukt het geldbedrag af
  }
  if (campFileFlag !=
      NULL) { // Controleert of de vlag voor het kampbestand is ingesteld
    printf("Kampbestand: %s\n",
           campFile); // Drukt de naam van het kampbestand af
  }

  freeItems(head); // Vrijgeven van geheugen dat is toegewezen aan de gelinkte
                   // lijst van items

  return 0;
}