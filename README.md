
# Adventures in Conwey's Game of Life.

I developed this game in 2008, if I am not mistaken, in college when I was
studying C and data structures.

While dynamically allocated data structures are not ideal for performance,
or even necessary in the case of this project, I wanted to implement my
own both for the sake of grades and self-satisfaction. 

The GLib influence is clear in the design of the data structures, which
gives them a quasi object-oriented paradigm.

This game depends on ncurses and should compile and should run on most unix-like
systems.

## How to start the game:

To start the game, input this on the terminal:

```
./life
```

This will generate a random pattern. To load a pattern, input the name
of the file containing it as an argument:

```
./life glider_gun
```

### Cells are predictable.

Game time is counted in generations. Each generation the cells on
the board are put to test. The result is either the death or the
birth of cells.

All cells have the eight adjacent cells as its neighbors.
They behave in the following manner:

- A live cell dies by loneliness if it has less than two neighbors.
- A cell dies by overcrowd if it has more than three neighbors.
- A cell keeps living if its neighbors are two or three.
- A dead cell comes to life if it has exactly three cells as neighbors.

## What does it all mean on the screen?

Red dots (.) are dead cells. Blue ampersands (&) are live cells.
The at sign (@) is you. The white square is the exit.


## What do I do?

You must escape the level by navigating on the board. The exit is a
white square, move to that position and you win.

You navigate by moving to one of four adjacent squares either by using
the arrow keys or the good old vim style 'hjkl'.

You must not move to a square where a cell is to be born. That is, if
there are threes cells adjacent to the square you are willing to move to,
a new cell will be born there and you will die. Predict their behavior
to always move to safe places.

## Ok. Any thing else I need to know?

Yes, you can give up the game by pressing 'q'.
Any key other than the movement keys and the 'q' key passes the turn.
There is a time limit too; if you don't make it in time, it is game over. 

And that's all, have fun.



## How do I do a pattern file?

Create a text file in the following format:

```
10
15
50
. . . . . . . . . .
. @ . . . . . & . .
. . . . . . . & . .
. . . . . . . & . .
. . . . . & . . . .
. . . . & & . . . .
. . . & . . . . . .
. . . . . . . . . .
. . . . . . . . X .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
. . . . . . . . . .
```

The first number is the width of the board.

The second number is the height of the board.

The third number is the number of generations the player has to win the
game.

The board size must be coherent to the values described above it.

The symbols are the same you will see in the game, except by the 'X',
which is the EXit. Only one exit and player character are allowed. 
Ampersands represent the first generation of live cells.

New lines must be Unix new lines, so set your editor accordingly.
There is no problem adding spaces between each element.

