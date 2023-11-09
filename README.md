# C-Minesweeper

C-Minesweeper is a text-based implementation of the classic Minesweeper game in C.

## Getting Started

1. **Compile the Game:**

   To compile the game, use a C compiler (GCC):

   ```shell
   gcc -o minesweeper minesweeper.c
   ```
2. **Run the Game:**
   ### Linux and macOS:
   ```shell
   ./minesweeper
   ```
    ### Windows:
   ```shell
   minesweeper.exe
   ```
4. **How to Play:**
- The game will prompt you to enter the following information interactively:

   1. Number of rows: Enter the desired number of rows for the game grid and press Enter.
   2. Number of columns: Enter the desired number of columns and press Enter.
   3. Number of mines: Specify the number of mines to be placed on the grid and press Enter.
   4. Enter action 'R' or 'r' to reveal a cell and 'F' or 'f' to flag a cell.
   5. Enter coordinates (row and column) to reveal or cells.
  
- Input Validation:
  
The game checks user input for both integers and characters, providing appropriate error messages.
The input buffer is cleared to ensure accurate input handling.

Feel free to explore the source code for more details about the game's specific features and controls.

## License
This project is under the MIT License. Feel free to modify and distribute it as you like.
