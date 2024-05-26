+---------------------------------------------------+
|                    Start programma               |
+---------------------------------------------------+
                           |
                           v
             +-----------------------------+
             | Controleer argc en print    |
             | usage indien nodig          |
             +-----------------------------+
                           |
          +----------------+-------------------+
          |                                    |
          v                                    |
+-----------------------+       +------------------------+
| Verwerk opties en     |       | Lees inhoud van elk    |
| argumenten            |       | opgegeven bestand      |
+-----------------------+       +------------------------+
          |                                    |
          v                                    |
+-----------------------+       +------------------------+
| Zoek en verwerk JSON  |       | Parse JSON-data naar   |
| bestanden             |       | structuur              |
+-----------------------+       +------------------------+
          |                                    |
          v                                    |
+-----------------------+       +------------------------+
| Initialiseer hoofd    |       | Initialiseer teller    |
| van gelinkte lijst    |       | en pointer naar JSON-  |
| en counters           |       | string                 |
+-----------------------+       +------------------------+
          |                                    |
          v                                    |
+-----------------------------+-------------------------+
| Voor elk JSON-bestand     | |    Terwijl teller niet  |
|                           | | gelijk is aan het aantal|
|   +-----------------------+ | items:                  |
|   | Zoek index, naam en   | |   +------------------+  |
|   | URL in JSON-string    | |   | Zoek index, naam |  |
|   +-----------------------+ |   | en URL in JSON-  |  |
|           |                 |   | string           |  |
|           v                 |   +------------------+  |
|   +-----------------------+ |           |             |
|   | Maak nieuwe knoop     | |           v             |
|   | in gelinkte lijst     | |   +--------------------+|
|   | en vul met gevonden   | |   | Maak nieuwe knoop   |
|   | data                  | |   | in gelinkte lijst   |
|   +-----------------------+ |   | en vul met gevonden |
|           |                 |   | data                |
|           v                 |   +--------------------+|
|   +-----------------------+ |           |             |
|   | Voeg nieuwe knoop toe | |           v             |
|   | aan gelinkte lijst    | |   +------------------+  |
|   +-----------------------+ |   | Voeg nieuwe knoop|  |
|           |                 |   | toe aan gelinkte |  |
|           v                 |   | lijst            |  |
|   +-----------------------+ |   +------------------+  |
|   | Verhoog teller        | |           |             |
|   +-----------------------+ |           v             |
|           |                 |   +------------------+  |
|           v                 |   | Verhoog teller   |  |
|   +-----------------------+ |   +------------------+  |
|   | Controleer of alle    | |           |             |
|   | JSON-bestanden zijn   | |           v             |
|   | verwerkt              | |   +------------------+  |
|   +-----------------------+ |   | Controleer of    |  |
|           |                 |   | alle JSON-       |  |
|           v                 |   | bestanden zijn   |  |
|   +-----------------------+ |   | verwerkt         |  |
|   | Maak gelinkte lijst   | |   +------------------+  |
|   | circulair             | |           |             |
|   +-----------------------+ |           v             |
|                             |   +------------------+  |
|                             |   | Maak gelinkte    |  |
|                             |   | lijst circulair  |  |
|                             |   +------------------+  |
|                             |                         |
+----------------------------------+--------------------+
                                             |
                                             v
                              +-----------------------+
                              | Zoek huidig item en   |
                              | toon de items         |
                              +-----------------------+
                                             |
                                             v
               +-----------------------------+------------------------+
               | Toon andere opgegeven opties| Bevrijd geheugen       |
               +-----------------------------+------------------------+
                                             |
                                             v
                                  +--------------------+
                                  |   Einde programma  |
                                  +--------------------+
