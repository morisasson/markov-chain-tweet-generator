# markov-chain-tweet-generator
Markov Chain Tweet Generator

This project implements a Markov Chain-based text generator in C, designed to create realistic-looking tweets based on an input dataset. The program reads tweets from a text file, constructs a Markov Chain model, and generates new sentences probabilistically.

Features:
- Parses input text and constructs a Markov Chain model
- Supports generating multiple random tweets
- Allows limiting the number of words read from the input
- Efficient memory management using linked lists
- Uses probability-based transitions to generate coherent sentences

Files Overview:
- tweets_generator.c  -> Main program for generating tweets
- markov_chain.c      -> Implementation of the Markov Chain logic
- markov_chain.h      -> Header file for Markov Chain definitions
- linked_list.c       -> Linked list implementation
- linked_list.h       -> Header file for linked list operations
- justdoit_tweets.txt -> Sample dataset (tweets)
- README.txt          -> This file (project documentation)

How to Compile and Run:

Prerequisites:
- A C compiler (e.g., gcc)
- Linux/macOS terminal or MinGW (for Windows users)

Compilation:
gcc -Wall -Wextra -std=c11 tweets_generator.c markov_chain.c linked_list.c -o tweets_generator

Running the Program:
./tweets_generator <seed> <num_tweets> <input_file> [<words_to_read>]

Arguments:
- <seed> – Random seed for reproducibility
- <num_tweets> – Number of tweets to generate
- <input_file> – Path to the input text file (e.g., justdoit_tweets.txt)
- [<words_to_read>] (optional) – Limits the number of words read from the file

Example Usage:
./tweets_generator 42 5 justdoit_tweets.txt 100

This will generate 5 tweets using the first 100 words from justdoit_tweets.txt.

How It Works:
1. Data Parsing: Reads words from an input file and stores them in a linked list-based database.
2. Markov Chain Construction: Tracks word transitions and their probabilities.
3. Tweet Generation: Starts with a random word and follows the Markov Chain to build a sentence.

Example Output:
Tweet 1: Just do it. #Nike #Motivation
Tweet 2: Believe in something. Even if it means sacrificing everything.
Tweet 3: Great way to meet new people at the gym! #JustDoIt

Potential Improvements:
- Enhance memory efficiency by using hash tables instead of linked lists
- Support additional input formats (e.g., JSON, CSV)
- Add user-configurable sentence constraints (e.g., min/max word count)

License:
This project is released under the MIT License.

---
Happy coding! 🚀

