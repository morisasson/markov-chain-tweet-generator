#include "markov_chain.h"

/**
 * Get a random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number)
{
    return rand() % max_number;
}

/**
 * Retrieve a node from the database by its data.
 * @param markov_chain The MarkovChain containing the database.
 * @param data_ptr The data to search for.
 * @return Pointer to the node if found, or NULL if not found.
 */
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
    if (markov_chain == NULL || data_ptr == NULL || markov_chain->database == NULL)
    {
        return NULL;
    }

    Node *current = markov_chain->database->first;

    while (current != NULL)
    {
        MarkovNode *current_markov = current->data;

        if (strcmp(current_markov->data, data_ptr) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

/**
 * Add a new node to the database or return an existing node if it already exists.
 * @param markov_chain The MarkovChain containing the database.
 * @param data_ptr The data to add.
 * @return Pointer to the node that was added or already exists, or NULL on failure.
 */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr)
{
    Node* existing_node = get_node_from_database(markov_chain, data_ptr);
    if (existing_node != NULL)
    {
        return existing_node;
    }

    // Create new MarkovNode
    MarkovNode *new_markov_node = malloc(sizeof(MarkovNode));
    if (new_markov_node == NULL)
    {
        printf(ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }

    // Allocate and copy the string
    char *word_copy = malloc(strlen(data_ptr) + 1);
    if (word_copy == NULL)
    {
        printf(ALLOCATION_ERROR_MASSAGE);
        free(new_markov_node);
        return NULL;
    }
    strcpy(word_copy, data_ptr);

    // Initialize the new node
    new_markov_node->data = word_copy;
    new_markov_node->frequency_list = NULL;

    // Add to database
    if (add(markov_chain->database, new_markov_node) != 0)
    {
        free(word_copy);
        free(new_markov_node);  // Ensure new MarkovNode is freed
        return NULL;
    }

    return markov_chain->database->last;
}

/**
 * Add or update a MarkovNode in the frequency list of another MarkovNode.
 * @param first_node The node whose frequency list is updated.
 * @param second_node The node to add/update in the frequency list.
 * @return 0 on success, 1 on failure.
 */
int add_node_to_frequency_list(MarkovNode *first_node, MarkovNode *second_node)
{
    if (first_node == NULL || second_node == NULL)
    {
        return 1;
    }

    if (first_node->frequency_list == NULL)
    {
        first_node->frequency_list = calloc(1, sizeof(MarkovNodeFrequency));
    }

    if (first_node->frequency_list == NULL)
    {
        printf(ALLOCATION_ERROR_MASSAGE);
        return 1;
    }

    int size = 0;
    while (first_node->frequency_list[size].markov_node != NULL)
    {
        if (first_node->frequency_list[size].markov_node == second_node)
        {
            first_node->frequency_list[size].frequency++;
            return 0;
        }
        size++;
    }

    MarkovNodeFrequency *new_array = realloc(first_node->frequency_list,
                                             (size + 2) * sizeof(MarkovNodeFrequency));
    if (new_array == NULL)
    {
        printf(ALLOCATION_ERROR_MASSAGE);
        return 1;
    }

    first_node->frequency_list = new_array;
    first_node->frequency_list[size].markov_node = second_node;
    first_node->frequency_list[size].frequency = 1;
    first_node->frequency_list[size + 1].markov_node = NULL;

    return 0;
}

/**
 * Get the first random MarkovNode from the database that is not a terminal node.
 * @param markov_chain The MarkovChain containing the database.
 * @return Pointer to the random MarkovNode, or NULL if not found.
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
    if (markov_chain == NULL || markov_chain->database == NULL ||
        markov_chain->database->first == NULL)
    {
        return NULL;
    }

    Node *current = markov_chain->database->first;
    MarkovNode *result = NULL;
    int non_terminal_words = 0;

    while (current != NULL)
    {
        non_terminal_words++;
        current = current->next;
    }

    if (non_terminal_words == 0)
    {
        return NULL;
    }
    do{
        int random_index = get_random_number(non_terminal_words);
        current = markov_chain->database->first;
        int counter = 0;

        while (counter != random_index )
        {
            counter++;
            current = current->next;
        }
        result = current->data;
    }while(!result || result->data[strlen(result->data) - 1] == '.');

    return result;
}

/**
 * Get the next random MarkovNode based on the frequency list of the current node.
 * @param cur_markov_node The current MarkovNode.
 * @return Pointer to the next MarkovNode, or NULL if not found.
 */
MarkovNode* get_next_random_node(MarkovNode *cur_markov_node)
{
    if (cur_markov_node == NULL || cur_markov_node->frequency_list == NULL)
    {
        return NULL;
    }

    int total_frequency = 0;
    int i = 0;
    while (cur_markov_node->frequency_list[i].markov_node != NULL)
    {
        total_frequency += cur_markov_node->frequency_list[i].frequency;
        i++;
    }

    int random_num = get_random_number(total_frequency);
    int current_sum = 0;
    i = 0;
    while (cur_markov_node->frequency_list[i].markov_node != NULL)
    {
        current_sum += cur_markov_node->frequency_list[i].frequency;
        if (random_num < current_sum)
        {
            return cur_markov_node->frequency_list[i].markov_node;
        }
        i++;
    }

    return NULL;
}

/**
 * Generate a random tweet starting from a given MarkovNode.
 * @param first_node The starting MarkovNode.
 * @param max_length Maximum number of words in the tweet.
 */
void generate_tweet(MarkovNode *first_node, int max_length)
{
    if (first_node == NULL || max_length <= 0)
    {
        return;
    }

    printf("%s", first_node->data);
    MarkovNode *current = first_node;
    int word_count = 1;

    while (word_count < max_length)
    {
        if (current->data[strlen(current->data) - 1] == '.')
        {
            break;
        }

        MarkovNode *next = get_next_random_node(current);
        if (next == NULL)
        {
            break;
        }

        printf(" %s", next->data);
        word_count++;
        current = next;
    }

    printf("\n");
}

/**
 * Free the database and its associated resources.
 * @param ptr_chain Pointer to the MarkovChain to free.
 */
void free_database(MarkovChain **ptr_chain)
{
    if (ptr_chain == NULL || *ptr_chain == NULL)
    {
        return;
    }

    MarkovChain *chain = *ptr_chain;
    Node *current = chain->database->first;

    while (current != NULL)
    {
        MarkovNode *markov_node = current->data;

        if (markov_node->data != NULL)
        {
            free(markov_node->data);  // Free the word.
        }

        if (markov_node->frequency_list != NULL)
        {
            free(markov_node->frequency_list);  // Free the frequency list.
        }

        free(markov_node);  // Free the MarkovNode.
        Node *next_node = current->next;  // Save the next node before freeing the current one.
        free(current);  // Free the Node.
        current = next_node;  // Move to the next node.
    }

    free(chain->database);  // Free the LinkedList.
    free(chain);  // Free the MarkovChain itself.
    *ptr_chain = NULL;  // Ensure the pointer does not point to any location.
}
