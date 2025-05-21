# Entertaining Spice Pretending (ESP) - A Two-Player Card Game

Welcome to **Entertaining Spice Pretending (ESP)**, a thrilling two-player card game inspired by the original card game Spicy. This project was implemented as part of a university assignment and demonstrates proficiency in memory management, file I/O, and linked data structures.

---

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Learning Goals](#learning-goals)
4. [How to Play](#how-to-play)
5. [Technical Details](#technical-details)
6. [Getting Started](#getting-started)
7. [Example Output](#example-output)
8. [Acknowledgements](#acknowledgements)

---

## Introduction

The ESP card game is designed for two players and is loosely based on the mechanics of Spicy. Each player alternates turns, playing cards face down and deciding whether to tell the truth or bluff about the card's value and spice. The opponent can either believe the claim or challenge it, adding a strategic layer of deception and deduction.

The game ends when all cards in the draw pile are played, and the player with the highest score wins.

---

## Features

- **Dynamic Memory Management**: Cards are stored on the heap, ensuring efficient use of memory.
- **Linked List Implementation**: Both player hands and the draw pile are represented as linked lists.
- **File I/O**: Game configuration is loaded from a file, and results are appended to the same file.
- **Robust Error Handling**: Handles invalid inputs, memory allocation failures, and file access issues gracefully.
- **Command-Based Gameplay**: Players interact using text commands, allowing for a clear and structured game flow.
- **Scoring System**: Tracks points earned through successful challenges and gameplay.

---

## Learning Goals

This project was designed to enhance the following technical skills:

- **Heap and Memory Management**: Efficiently allocate and deallocate memory to prevent leaks.
- **File I/O**: Read and write configuration and result data to external files.
- **Data Structures**: Implement linked lists to manage dynamic collections of cards.
- **Structs and Enums**: Use structured data types to represent cards and game states.
- **Dynamic Input Handling**: Manage user input of arbitrary length using `realloc`.

---

## How to Play

1. **Setup**: The game begins by distributing cards to both players and initializing a draw pile.
2. **Turn-Based Play**:
   - A player plays a card from their hand, either truthfully or as a bluff.
   - The opponent can either trust the play or issue a challenge.
3. **Challenges**: Points are awarded based on the validity of the challenge.
4. **Endgame**: The game concludes when the draw pile is empty, and the winner is determined based on total points.

For detailed rules and commands, refer to the Assignment Description.

---

## Technical Details

- **Programming Language**: C
- **Core Concepts**:
  - Dynamic memory allocation (`malloc`, `realloc`, `free`)
  - Linked list implementation
  - File handling (`fopen`, `fwrite`, `fclose`)
  - Error handling and program termination
- **Code Structure**:
  - `card_t`: Struct representing a card with `value` and `spice`.
  - Linked lists for hand cards and the draw pile.
  - Command-based input system with error validation.

---

## Getting Started

### Prerequisites

- GCC or any compatible C compiler
- A terminal or IDE with C support

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/bluffing-game.git
   ```
2. Navigate to the project directory:
   ```bash
   cd bluffing-game
   ```
3. Compile the program:
   ```bash
   clang -o a4-csf a4-csf.c
   ```

### Usage

Run the program with a configuration file:

```bash
./a4-csf configs/config_example.txt
```

---

## Example Output

```
Welcome to Entertaining Spice Pretending!
-------------------
ROUND START
-------------------
Player 1:
    latest played card:
    cards played this round: 0
    hand cards: 4_p 10_p 3_w 9_w 9_w 10_w
P1 > play 3_w 3_w
Player 2:
    latest played card: 3_w
    cards played this round: 1
    hand cards: 5_c 5_c 7_c 7_p 6_w 10_w
P2 > challenge value
Challenge successful: 3_w's value does not match the real card 10_w.
Player 1 gets 4 points.
```

---

## Acknowledgements

This project was developed as part of a university course on Computer Science Fundamentals (CSF). Special thanks to the course instructors and peers for their guidance and support.

