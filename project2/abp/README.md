# Sources

using the BSD Kernal crc32 function/file. This code is open source and free to use without restriction.
queue structure based off the implementaiton [here](http://embedjournal.com/implementing-circular-buffer-embedded-c/)

# Building

make

# Running

Syntax:

    ./project2 num-messages chance-of-loss chance-of-corruption chance-of-reordering average-time-between-msgs trace-level randomize 0 -1

The last 0 referse to unidirectional, and the -1 tells the simulator to use a seed with time(NULL) instead of the given number.
You can also use -1 for trace level which I think gives a nicer output.

Some examples:

    ./project2 20 0.01 0.01 0.01 100 1 1 0 -1
    ./project2 34 0.1 0.0 0.1 1000 1 1 0 -1

with pretty output
    ./project2 4 0.1 0.0 0.1 1000 -1 1 0 -1
    ./project2 12 0.5 0.0 0.02 1000 -1 1 0 -1

