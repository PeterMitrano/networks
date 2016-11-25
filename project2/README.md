# Sources

using the BSD Kernal crc32 function/file. This code is open source and free to use without restriction.
queue structure based off the implementaiton [here](http://embedjournal.com/implementing-circular-buffer-embedded-c/)

# Building

make

# Running

Syntax:

    ./project2 num-messages chance-of-loss chance-of-corruption chance-of-reordering average-time-between-msgs trace-level randomize 0 -1

The last 0 referse to unidirectional, and the -1 tells the simulator to use a seed with time(NULL) instead of the given number.

Some examples:

    ./project2 64 0.01 0.01 0.01 10 1 1 0 -1
    ./project2 64 0.1 0.0 0.1 100 1 1 0 -1
    ./project2 4 0.1 0.0 0.1 100 1 1 0 -1
    ./project2 128 0.5 0.0 0.02 100 1 1 0 -1

