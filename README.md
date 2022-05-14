This is a simple program that generates MCs using Markov Chains.
A makefile is included to compile it using g++. The first command in the file, the all command is all you need. It will compile to the file "mcinterface".
Once it's compiled, run it in terminal with the following arguments supplied to it
- The number of MCs to generate
- One or more .txt files of MCs to train it on

It will output the number of MCs you requested, numbered, in "newmc.txt"

You will need to copy paste one or more batches of MCs into text documents and feed them to the program for it to work. I'd post some of them myself, but that would risk breaching the privacy of the writers.

The process of reading the file purposefully removes any characters other than letters, spaces, newlines, apostrophes and colons, so some messages, especially links, may appear a bit odd in the generated MCs.

Has not been extensively tested, nor tested on Macs, so no guarantees.

Apologies for the short README, this was a fun side project but I'm not dedicated enough to write full documentation for this at this time.

Happy MCing!
