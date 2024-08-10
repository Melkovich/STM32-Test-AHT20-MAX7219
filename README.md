# STM32-Test-AHT20-MAX7219
Petit projet pour m'entrainer à lire un capteur de température AHT20 en I2C et d'afficher le résultat sur une matrice à LED en cascade contrôlé par un registre à décalage MAX7219.

Le fonctionnement du projet est le suivant :

Version 1:
- un appui sur un bouton poussoir permet de mettre à jour l'affichage de la température
- possibilité d'afficher la température et l'humidité sur une console telle que putty ou TeraTerm

Version 2:
- Ajout fonction animation data up pour faire glisser par le haut 2 données à afficher
- Exemple du fonctionnement de cette animation avec Task_Display_3

PS : Il y a des fonctionnalités qui n'ont rien à voir avec le projet, c'est seulement pour m'entrainer je me sers de ce projet comme une sand-box :)

