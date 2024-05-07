#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Component {
    V,
    S,
    M
};

enum Type {
    sphere,
    cone,
    cylinder,
    line,
    cube
};

struct AreaOfEffect {
    int size;
    enum Type type;
};

struct DamageAtCharacterLevel {
    void* damage_at_character_level;
};

struct DamageAtSlotLevel {
    void* damage_at_slot_level;
};

struct DamageType {
    char* index;
    char* name;
    char* url;
};

union Damage {
    struct DamageAtCharacterLevel at_character_level;
    struct DamageAtSlotLevel at_slot_level;
    struct DamageType type;
};

struct Resource {
    char* index;
    char* name;
    char* url;
};

struct Spell {
    char* index;
    char* name;
    char* url;
    char** desc;
    char** higher_level;
    char* range;
    enum Component components;
    char* material;
    struct AreaOfEffect area_of_effect;
    int ritual;
    char* duration;
    int concentration;
    char* casting_time;
    int level;
    char* attack_type;
    union Damage damage;
    struct Resource school;
    struct Resource classes;
    struct Resource subclasses;
};

// Helper function to allocate memory and copy a string
char* copyString(const char* source) {
    if (source == NULL) return NULL;

    size_t length = strlen(source);
    char* destination = malloc(length + 1);
    if (destination == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    strcpy(destination, source);
    return destination;
}

int main() {
  
    FILE *file = fopen("spells.json", "r");

    if (file == NULL) {
        perror("Error: Failed to open file");
        return 0;
    }
    char buffer[1024];
    size_t len = sizeof(buffer);
    printf("FILE\n");
    while (fgets(buffer, len, file)) {
        printf("%s", buffer);
    }
    printf("-------------------------------------------\n");

    struct Spell spell; // Step 3: Create an instance of struct Spell

    int descCount = 0;
    int levelCount = 0;
    int damageCount = 0;

    // Step 2: Use strtok to extract values from jsonString
    char* token = strtok("spells.json", "{\":,}\n\t");
    while (token != NULL) {
        if (strcmp(token, "index") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.index = copyString(token);
        } else if (strcmp(token, "name") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.name = copyString(token);
        } else if (strcmp(token, "url") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.url = copyString(token);
        } else if (strcmp(token, "desc") == 0) {
            token = strtok(NULL, "{\":,}\n\t[\"");
            while (token != NULL && strcmp(token, "]") != 0) {
                spell.desc = realloc(spell.desc, (descCount + 1) * sizeof(char*));
                spell.desc[descCount] = copyString(token);
                descCount++;
                token = strtok(NULL, "{\":,}\n\t[\"");
            }
        } else if (strcmp(token, "higher_level") == 0) {
            token = strtok(NULL, "{\":,}\n\t[\"");
            while (token != NULL && strcmp(token, "]") != 0) {
                spell.higher_level = realloc(spell.higher_level, (levelCount + 1) * sizeof(char*));
                spell.higher_level[levelCount] = copyString(token);
                levelCount++;
                token = strtok(NULL, "{\":,}\n\t[\"");
            }
        } else if (strcmp(token, "range") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.range = copyString(token);
        } else if (strcmp(token, "components") == 0) {
            token = strtok(NULL, "{\":,}\n\t[\"");
            if (strcmp(token, "V") == 0) {
                spell.components = V;
            } else if (strcmp(token, "S") == 0) {
                spell.components = S;
            } else if (strcmp(token, "M") == 0) {
                spell.components = M;
            }
        } else if (strcmp(token, "ritual") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.ritual = (strcmp(token, "true") == 0) ? 1 : 0;
        } else if (strcmp(token, "duration") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.duration = copyString(token);
        } else if (strcmp(token, "concentration") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.concentration = (strcmp(token, "true") == 0) ? 1 : 0;
        } else if (strcmp(token, "casting_time") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.casting_time = copyString(token);
        } else if (strcmp(token, "level") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.level = atoi(token);
        } else if (strcmp(token, "attack_type") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            spell.attack_type = copyString(token);
        } else if (strcmp(token, "damage") == 0) {
            token = strtok(NULL, "{\":,}\n\t");
            while (token != NULL && strcmp(token, "}") != 0) {
                if (strcmp(token, "damage_at_character_level") == 0) {
                    token = strtok(NULL, "{\":,}\n\t\"");
                    while (token != NULL && strcmp(token, "}") != 0) {
                        spell.damage.at_character_level.damage_at_character_level = realloc(
                                spell.damage.at_character_level.damage_at_character_level,
                                (damageCount + 1) * sizeof(char*)
                        );
                        spell.damage.at_character_level.damage_at_character_level[damageCount] = copyString(token);
                        damageCount++;
                        token = strtok(NULL, "{\":,}\n\t\"");
                    }
                } else if (strcmp(token, "damage_type") == 0) {
                    token = strtok(NULL, "{\":,}\n\t\"");
                    spell.damage.type.index = copyString(token);
                    token = strtok(NULL, "{\":,}\n\t\"");
                    token = strtok(NULL, "{\":,}\n\t\"");
                    spell.damage.type.name = copyString(token);
                    token = strtok(NULL, "{\":,}\n\t\"");
                    token = strtok(NULL, "{\":,}\n\t\"");
                    spell.damage.type.url = copyString(token);
                }
                token = strtok(NULL, "{\":,}\n\t");
            }
        } else if (strcmp(token, "school") == 0) {
            token = strtok(NULL, "{\":,}\n\t\"");
            spell.school.index = copyString(token);
            token = strtok(NULL, "{\":,}\n\t\"");
            token = strtok(NULL, "{\":,}\n\t\"");
            spell.school.name = copyString(token);
            token = strtok(NULL, "{\":,}\n\t\"");
            token = strtok(NULL, "{\":,}\n\t\"");
            spell.school.url = copyString(token);
        } else if (strcmp(token, "classes") == 0) {
            token = strtok(NULL, "{\":,}\n\t[\"");
            int classCount = 0;
            while (token != NULL && strcmp(token, "]") != 0) {
                spell.classes.index = copyString(token);
                token = strtok(NULL, "{\":,}\n\t\"");
                token = strtok(NULL, "{\":,}\n\t\"");
                spell.classes.name = copyString(token);
                token = strtok(NULL, "{\":,}\n\t\"");
                token = strtok(NULL, "{\":,}\n\t\"");
                spell.classes.url = copyString(token);
                token = strtok(NULL, "{\":,}\n\t[\"");
            }
        } else if (strcmp(token, "subclasses") == 0) {
            token = strtok(NULL, "{\":,}\n\t[\"");
            int subclassCount = 0;
            while (token != NULL && strcmp(token, "]") != 0) {
                spell.subclasses.index = copyString(token);
                token = strtok(NULL, "{\":,}\n\t\"");
                token = strtok(NULL, "{\":,}\n\t\"");
                spell.subclasses.name = copyString(token);
                token = strtok(NULL, "{\":,}\n\t\"");
                token = strtok(NULL, "{\":,}\n\t\"");
                spell.subclasses.url = copyString(token);
                token = strtok(NULL, "{\":,}\n\t[\"");
            }
        }

        token = strtok(NULL, "{\":,}\n\t");
    }

    // Step 4: Print the values stored in the struct
    printf("Spell Name: %s\n", spell.name);
    printf("Spell Index: %s\n", spell.index);
    printf("Spell URL: %s\n", spell.url);
    printf("Spell Description:\n");
    for (int i = 0; i < descCount; i++) {
        printf("- %s\n", spell.desc[i]);
    }
    printf("Higher Level:\n");
    for (int i = 0; i < levelCount; i++) {
        printf("- %s\n", spell.higher_level[i]);
    }
    printf("Range: %s\n", spell.range);
    printf("Components: %d\n", spell.components);
    printf("Ritual: %s\n", spell.ritual ? "true" : "false");
    printf("Duration: %s\n", spell.duration);
    printf("Concentration: %s\n", spell.concentration ? "true" : "false");
    printf("Casting Time: %s\n", spell.casting_time);
    printf("Level: %d\n", spell.level);
    printf("Attack Type: %s\n", spell.attack_type);
    printf("Damage Type Index: %s\n", spell.damage.type.index);
    printf("Damage Type Name: %s\n", spell.damage.type.name);
    printf("Damage Type URL: %s\n", spell.damage.type.url);
    printf("Damage at Character Level:\n");
    for (int i = 0; i < damageCount; i++) {
        printf("- %s\n", spell.damage.at_character_level.damage_at_character_level[i]);
    }
    printf("School Index: %s\n", spell.school.index);
    printf("School Name: %s\n", spell.school.name);
    printf("School URL: %s\n", spell.school.url);
    printf("Classes Index: %s\n", spell.classes.index);
    printf("Classes Name: %s\n", spell.classes.name);
    printf("Classes URL: %s\n", spell.classes.url);
    printf("Subclasses Index: %s\n", spell.subclasses.index);
    printf("Subclasses Name: %s\n", spell.subclasses.name);
    printf("Subclasses URL: %s\n", spell.subclasses.url);

    // Step 5: Free allocated memory
    for (int i = 0; i < descCount; i++) {
        free(spell.desc[i]);
    }
    free(spell.desc);

    for (int i = 0; i < levelCount; i++) {
        free(spell.higher_level[i]);
    }
    free(spell.higher_level);

    for (int i = 0; i < damageCount; i++) {
        free((char*)spell.damage.at_character_level.damage_at_character_level[i]);
    }
    free(spell.damage.at_character_level.damage_at_character_level);

    free(spell.index);
    free(spell.name);
    free(spell.url);
    free(spell.range);
    free(spell.attack_type);
    free(spell.damage.type.index);
    free(spell.damage.type.name);
    free(spell.damage.type.url);
    free(spell.school.index);
    free(spell.school.name);
    free(spell.school.url);
    free(spell.classes.index);
    free(spell.classes.name);
    free(spell.classes.url);
    free(spell.subclasses.index);
    free(spell.subclasses.name);
    free(spell.subclasses.url);


    fclose(file);
    return 0;
}