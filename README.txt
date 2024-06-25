The program simulates a lexical anylizer from a compiler.
It will read the input from a text file given from terminal
with command './a.out [file name].txt' when running.
The code will not care about grammar or order but 
will care about # of characters in a sequence and which
characters are recognizable according to its reserved library
of words and symbols.
A lexeme table and token table will be produced as the code reads
the file to show what characters (or group of character) were recognized
and given a token. Spaces aren't read and if it's not recognized, error is outputted.