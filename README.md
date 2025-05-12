# Spicy – A Two-Player Bluffing Card Game (C Implementation)

This project is a terminal-based implementation of a two-player bluffing card game inspired by the commercial card game **Spicy**. It was developed as part of the ESP (Einführung in die Systemnahe Programmierung) practicals at TU Graz, with the goal of applying basic system programming concepts in C.

## 🎮 Game Concept

Spicy is a game of bluffing and memory. Each player takes turns placing cards face-down, claiming a certain card and value. The opponent can either accept the move or challenge it. Successful bluffs are rewarded; failed ones are punished. The goal is to get rid of your hand before your opponent does — or to catch them in a lie.

This version simplifies and adapts the original rules to a 1v1, console-based format.

## ✨ Features

- Terminal-based interface
- Configurable deck setup via file input
- Simple bluff mechanic with challenge options
- Dynamic memory usage for decks and hands
- Structured modular C code with header files

## 🧠 Gameplay Overview

Each player is dealt an equal number of cards from a shuffled deck. On their turn, a player:

1. Chooses a card from their hand
2. Declares a **color** and **value** (not necessarily truthfully)
3. Plays the card face down

The opponent then decides whether to:

- **Accept** the play (no challenge)
- **Challenge** the claimed color
- **Challenge** the claimed value

If the challenge is correct, the bluffing player is penalized. If the challenge fails, the challenger is penalized. Points are awarded for successful plays or successful challenges.

## ⚙️ Compilation and Execution

To compile the project, use a C compiler like `gcc`. Here's how to compile and run:

```bash
gcc -Wall -Werror -std=c99 -o spicy main.c game.c io.c card.c
./spicy config.txt
```

The game reads its deck and initial setup from a configuration file.

## 🗂️ Project Structure

- `main.c` – Program entry point and game loop
- `game.c/.h` – Core game logic and rule enforcement
- `io.c/.h` – User input and output functions
- `card.c/.h` – Data structures for cards and decks

## 🧾 Config File Format

The game expects a config file (`config.txt`) structured like this:

```
RED 3
BLUE 5
YELLOW 2
```

Each line represents a number of cards of a specific color. These are loaded into the draw pile.

## 🎯 Game Commands

During your turn, you'll be prompted to:

- Choose a card from your hand
- Enter a claimed color (e.g., `RED`, `BLUE`, `YELLOW`)
- Enter a claimed value (integer)

The opponent then chooses to:

- `accept`
- `challenge color`
- `challenge value`

The game continues until one player has no cards left.

## 🧪 Sample Session

```
Player 1, your hand: [RED 5, BLUE 3, YELLOW 2]
Choose card to play: 1
Claim color: RED
Claim value: 7

Player 2, do you:
1) Accept
2) Challenge color
3) Challenge value
> 3
```

## 🔍 Notes & Future Work

- Only supports two players locally; no network or AI opponent
- Basic validation and error handling implemented
- Future improvements could include card shuffling, a GUI, or multiplayer over sockets

---

Feel free to fork, clone, or modify this game for your own experiments in C!
