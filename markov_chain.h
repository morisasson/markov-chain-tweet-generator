#ifndef _MARKOV_CHAIN_H_
#define _MARKOV_CHAIN_H_

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For malloc()
#include <stdbool.h> // For bool type
#include <string.h> // For string operations like strcmp

#define ALLOCATION_ERROR_MASSAGE "Allocation failure: Failed to allocate"\
            " new memory\n"

/**
 * Struct representing a MarkovChain.
 * Contains a linked list as its database to store Markov nodes.
 */
typedef struct MarkovChain {
    LinkedList *database; // Pointer to the linked list database
} MarkovChain;

/**
 * Struct representing a MarkovNode.
 * Contains the data (word or phrase) and a frequency list that tracks
 * the occurrences of subsequent nodes.
 */
typedef struct MarkovNode {
    char *data; // The word or phrase stored in the node
    struct MarkovNodeFrequency* frequency_list; // List of subsequent nodes and their frequencies
    // Add any additional fields if needed
} MarkovNode;

/**
 * Struct representing the frequency of transitions between nodes.
 * Tracks how often a particular MarkovNode follows another.
 */
typedef struct MarkovNodeFrequency {
    struct MarkovNode* markov_node; // Pointer to the MarkovNode being tracked
    int frequency; // Number of times this transition occurs
    // Add any additional fields if needed
} MarkovNodeFrequency;

/**
 * Check if `data_ptr` exists in the database. If it exists, return the Node
 * wrapping it in the `markov_chain`, otherwise return NULL.
 *
 * @param markov_chain The chain to search in its database.
 * @param data_ptr The data to search for.
 * @return Pointer to the Node wrapping the given data, or NULL if the data
 * is not in the database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr);

/**
 * Check if `data_ptr` exists in the database. If it exists, return its Node.
 * Otherwise, create a new Node, add it to the end of the `markov_chain`'s
 * database, and return it.
 *
 * @param markov_chain The chain to search in its database.
 * @param data_ptr The data to add or retrieve.
 * @return Node wrapping the given `data_ptr` in the database, or NULL in case
 * of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr);

/**
 * Add the `second_node` to the frequency list of the `first_node`.
 * If the `second_node` already exists in the list, update its frequency.
 *
 * @param first_node The node whose frequency list is being updated.
 * @param second_node The node to add or update in the frequency list.
 * @return 0 if the process was successful, 1 in case of allocation failure.
 */
int add_node_to_frequency_list(MarkovNode *first_node, MarkovNode *second_node);

/**
 * Free the `markov_chain` and all of its contents from memory.
 *
 * @param ptr_chain Pointer to the MarkovChain to free.
 */
void free_database(MarkovChain **ptr_chain);

/**
 * Get a random MarkovNode from the `markov_chain`'s database.
 *
 * @param markov_chain The chain containing the database.
 * @return A random MarkovNode.
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain);

/**
 * Choose the next MarkovNode randomly based on its occurrence frequency.
 *
 * @param cur_markov_node The current MarkovNode.
 * @return The next random MarkovNode.
 */
MarkovNode* get_next_random_node(MarkovNode *cur_markov_node);

/**
 * Generate and print a random sentence using the MarkovChain.
 * The sentence must contain at least two words.
 *
 * @param first_node The starting MarkovNode for the sentence.
 * @param max_length The maximum length of the generated sentence.
 */
void generate_tweet(MarkovNode *first_node, int max_length);

/**
 * Print the contents of the MarkovChain database.
 *
 * @param chain The MarkovChain to print.
 */

#endif /* _MARKOV_CHAIN_H_ */
