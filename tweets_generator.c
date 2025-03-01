#include "markov_chain.h"
#define FILE_PATH_ERROR "Error: incorrect file path\n"
#define NUM_ARGS_ERROR "Usage: invalid number of arguments\n"
#define DELIMITERS " \n\t\r"

/**
 * Fill the MarkovChain database with words from a file.
 * @param fp File pointer to the input file.
 * @param words_to_read Number of words to read from the file (0 means read all).
 * @param markov_chain The MarkovChain to populate.
 * @return 0 on success, 1 on failure.
 */
int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
    char line[1000];
    int word_counter = 0;
    MarkovNode *prev_node = NULL;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char *word = strtok(line, DELIMITERS);

        while (word != NULL)
        {
            if (words_to_read != 0 && word_counter >= words_to_read)
            {
                return 0; // Stop reading if the desired number of words is reached
            }

            Node *new_node = add_to_database(markov_chain, word);
            if (new_node == NULL)
            {
                return 1; // Return on memory allocation failure
            }

            if (prev_node != NULL)
            {
                if (add_node_to_frequency_list(prev_node, new_node->data) != 0)
                {
                    return 1; // Return on memory allocation failure
                }
            }

            // Reset `prev_node` if the current word ends with a period
            if (new_node->data->data[strlen(new_node->data->data) - 1] == '.')
            {
                prev_node = NULL;
            }
            else
            {
                prev_node = new_node->data;
            }

            word_counter++;
            word = strtok(NULL, DELIMITERS); // Continue to the next word
        }
    }
    return 0;
}

/**
 * Main function to read input, build a MarkovChain, and generate tweets.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return EXIT_SUCCESS or EXIT_FAILURE.
 */
int main(int argc, char *argv[])
{
    // Check for the correct number of arguments
    if (argc != 4 && argc != 5)
    {
        fprintf(stderr, "%s\n", NUM_ARGS_ERROR);
        exit(EXIT_FAILURE);
    }

    unsigned int seed = (unsigned int) strtol(argv[1], NULL, 10); // Random seed
    int num_tweets = (int) strtol(argv[2], NULL, 10); // Number of tweets to generate
    char *input_path = argv[3]; // Input file path
    int words_to_read = 0;
    if (argc == 5)
    {
        words_to_read = (int) strtol(argv[4], NULL, 10); // Optional limit on words to read
    }

    srand(seed);

    FILE *fp = fopen(input_path, "r");
    if (fp == NULL)
    {
        fprintf(stderr,"%s\n",FILE_PATH_ERROR);
        exit(EXIT_FAILURE);
    }

    MarkovChain *chain = malloc(sizeof(MarkovChain));
    if (chain == NULL)
    {
        fprintf(stderr,ALLOCATION_ERROR_MASSAGE);
        fclose(fp);
        exit( EXIT_FAILURE);
    }

    chain->database = malloc(sizeof(LinkedList));
    if (chain->database == NULL)
    {
        fprintf(stderr,ALLOCATION_ERROR_MASSAGE);
        free(chain);
        fclose(fp);
        exit( EXIT_FAILURE);
    }
    chain->database->first = NULL;
    chain->database->last = NULL;
    chain->database->size = 0;

    // Call the function to fill the database with words from the file
    if (fill_database(fp, words_to_read, chain) != 0)
    {
        fprintf(stderr,ALLOCATION_ERROR_MASSAGE);
        free_database(&chain);
        fclose(fp);
        exit( EXIT_FAILURE);
    }

    // Generate tweets
    for (int i = 1; i <= num_tweets; i++)
    {
        printf("Tweet %d: ", i);
        MarkovNode *first = get_first_random_node(chain);
        if (first != NULL)
        {
            generate_tweet(first, 20); // Generate a tweet with a maximum length of 20 words
        }
    }

    // Free resources at the end
    free_database(&chain);
    fclose(fp);
    return EXIT_SUCCESS;
}
