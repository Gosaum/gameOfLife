# Jeu de la Vie

## Description
Le **Jeu de la Vie** est une simulation d'automate cellulaire proposée par John Conway. Ce projet implémente ce concept avec une interface console et graphique. Lors de l'exécution, un menu interactif vous permet de choisir le mode d'exécution.

## Fonctionnalités
- **Mode Console** : Exécute la simulation avec des étapes affichées dans le terminal.
- **Mode Graphique** : Offre une interface visuelle interactive avec SFML.
- **Tests Unitaires** : Vérifie le comportement de la grille et des cellules.
- Support des grilles standards et toriques.
- Gestion d'obstacles dans la grille.
- Sauvegarde et chargement des grilles.

## Prérequis
- **C++17 ou supérieur**.
- Bibliothèque **SFML 2.5+**.
- **La police Arial.ttf** doit être placée dans le répertoire où le programme est exécuté.

### Installation de SFML
- **Sous Linux** :
  ```bash
  sudo apt-get install libsfml-dev
  ```
- **Sous Windows** :
  1. Téléchargez SFML depuis le site officiel.
  2. Configurez votre projet pour inclure les fichiers de bibliothèque.

## Compilation et Exécution
### Compilation
```bash
g++ -std=c++17 -o GameOfLife main.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

### Exécution
```bash
./GameOfLife
```

Le programme affiche un menu interactif.

## Menu Principal
```
=== Jeu de la Vie ===
1. Mode Console
2. Mode Graphique (GUI)
3. Quitter
4. Tests Unitaires
```
- **1 - Mode Console** : Exécution dans le terminal.
- **2 - Mode Graphique** : Lance une interface interactive.
- **3 - Quitter** : Ferme le programme.
- **4 - Tests Unitaires** : Exécute des tests automatiques.

## Tests Unitaires
Pour lancer les tests :
1. Lancez le programme.
2. Choisissez l'option `4 - Tests Unitaires`.

Les tests vérifient la logique des cellules, les mises à jour de grille, et leur cohérence.

## Options Configurables
### Page de Configuration
- **Nombre d'itérations** : Défini entre **1 et 10 000**.
- **Mode Torique** : Les bordures se connectent (activable/désactivable).
- **Dimensions de la Grille** :
  - Longueur et largeur entre **5 et 999** en GUI.
  - Illimitées en console.

### Limites GUI
- Temps entre itérations : Configurable entre **10s** et **0,01s**.

## Mode Graphique (GUI)
### Actions Disponibles
- **Créer une grille** ou **charger une grille existante**.
- Configurer les dimensions, le nombre d'itérations, et le mode torique.
- Visualiser la grille avec des cellules vivantes, mortes, et des obstacles.
- Contrôler la simulation en temps réel.

### Contrôles
#### Page de Configuration
- **ENTREE** : Passe à la page suivante après configuration.

#### Page de Simulation
- **ESPACE** : Met en pause ou reprend la simulation.
- **Flèche Haut** : Augmente la vitesse.
- **Flèche Bas** : Réduit la vitesse.
- **S** : Sauvegarde l'état actuel.
- **1** : Sélectionne l'état "vivant" pour les cellules.
- **2** : Sélectionne l'état "mort".
- **3** : Sélectionne l'état "obstacle".

## Format des Fichiers d'Entrée
Les fichiers doivent respecter le format suivant :
1. Dimensions (`lignes colonnes`) en première ligne.
2. États des cellules :
   - `0` : Mort.
   - `1` : Vivant.
   - `2` : Obstacle.

**Exemple** :
```
5 5
0 1 0 0 0
1 1 1 0 0
0 0 0 0 0
0 2 2 0 0
0 0 0 0 0
```

## Auteurs
- **Clément Richard (Sp2ctrale)**
- **Alexendre Danjou (Gosaum)**
[GitHub](https://github.com/Gosaum/gameOfLife/tree/main)

