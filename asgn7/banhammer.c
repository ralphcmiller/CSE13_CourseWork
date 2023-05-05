#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>

#include "bv.h"
#include "ht.h"
#include "bf.h"
#include "node.h"
#include "bst.h"
#include "parser.h"
#include "messages.h"

#define OPTIONS "hst:f:"
int main(int argc, char **argv) {
    //initalize nessisary variables
    FILE *infile = stdin;
    int opt = 0;
    bool do_display = false;
    bool do_stats = false;
    uint32_t size_ht = 65536; // 2^16
    uint32_t size_bf = 1048576; // 2^20

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': do_display = true; break;
        case 's': do_stats = true; break;
        case 't': size_ht = abs(atoi(optarg)); break;
        case 'f': size_bf = abs(atoi(optarg)); break;
        default: fprintf(stderr, ""); do_display = true;
        }
    }
    // Exit program if size is invalid
    if (size_ht == 0) {
        fprintf(stderr, "Invalid hash table size.\n");
        exit(1);
    }
    if (size_bf == 0) {
        fprintf(stderr, "Invalid Bloom filter size.\n");
        exit(1);
    }
    if (do_display) { //display program help and usage, true if -h, incorrect input, or no input.
        printf("SYNOPSIS\n   A word filtering program for the GPRSC.\n");
        printf("   Filters out and reports bad words parsed from stdin.\n\n");
        printf("USEAGE\n   %s [-hs] [-t size] [-f size]\n\n", argv[0]);
        printf("OPTIONS\n");
        printf("   -h             Program help and usage.\n");
        printf("   -s             Print program statistics.\n");
        printf("   -t             Specify hash table size (default: 2^16).\n");
        printf("   -f             Specify Bloom filter size (default 2^20).\n");
    }

    // Open bad/newspeak.txt, catch errors and exit if error //

    FILE *new = fopen("newspeak.txt", "r"); // open badspeak file
    if (new == NULL) {
        // file does not exist
        exit(1);
    }
    FILE *bad = fopen("badspeak.txt", "r"); // open badspeak file
    if (bad == NULL) {
        fclose(new);
        // file does not exist
        exit(1);
    }

    // Init Hash Table and Bloom Filer //

    HashTable *ht = ht_create(size_ht);
    BloomFilter *bf = bf_create(size_bf);

    // Read in list of badspeak with fscanf add to bf and ht //
    char oldspeak[4096]; // 4kb buffer to hold oldspeak word
    char newspeak[4096]; // 4kb buffer to hold newspeak word

    // scan in a word, one by one until EOF is reached
    while (fscanf(bad, "%s", oldspeak) != EOF) {
        // add to bf
        bf_insert(bf, oldspeak);
        // add to ht
        ht_insert(ht, oldspeak, NULL);
    }

    // Read in list of bad and newspeak pairs, add to bf and ht //

    // scan in a word, one by one until EOF is reached
    while (fscanf(new, "%s %s", oldspeak, newspeak) != EOF) {
        // add to bf
        bf_insert(bf, oldspeak);
        // add to ht
        ht_insert(ht, oldspeak, newspeak);
    }

    // Read in words from std in using the parsing module //

    // Compile regex

    regex_t word_regex;
    regcomp(&word_regex, "[A-Za-z0-9_'-]+", REG_EXTENDED);
    char *word = NULL;

    // initialize vars for messages and bst for bad words
    bool thot = false;
    bool coun = false;

    Node *bad_words = NULL;
    Node *mixed_words = NULL;

    // Get words from std in and filter
    while ((word = next_word(infile, &word_regex)) != NULL) {

        // make input word lower case
        int i = 0;
        while (word[i]) {
            word[i] = tolower(word[i]);
            i++;
        }

        // see if word is in bf
        if (bf_probe(bf, word) == true) {

            //get node from ht
            Node *n = ht_lookup(ht, word);

            if (n != NULL) {

                // IF there is no newspeak translation, thoughtcrime commited
                if (n->newspeak == NULL && n->oldspeak != NULL) {
                    //insert these words into a bst for later
                    if (bad_words == NULL) {
                        bad_words = node_create(n->oldspeak, NULL);
                    }
                    bst_insert(bad_words, n->oldspeak, NULL);
                    thot = true;
                }
                // IF there is a newspeak translation, counseling required
                if (n->newspeak != NULL) {
                    //insert these words into a bst for later
                    if (mixed_words == NULL) {
                        mixed_words = node_create(n->oldspeak, n->newspeak);
                    }
                    bst_insert(mixed_words, n->oldspeak, n->newspeak);
                    coun = true;
                }
            }
            // Else, no crime commited, user is good.
        }
    }

    // Free regex memory
    clear_words();
    regfree(&word_regex);

    // Display messages if bad words were found //
    // and stats are not enabled                //

    if (do_stats != true) {
        if (thot == true && coun == true) {
            printf("%s", mixspeak_message);
            bst_print(bad_words);
            bst_print(mixed_words);
        }
        if (thot != true && coun == true) {
            printf("%s", goodspeak_message);
            bst_print(mixed_words);
        }
        if (thot == true && coun != true) {
            printf("%s", badspeak_message);
            bst_print(bad_words);
        }
    }

    // Print statistics if enabled //

    if (do_stats == true) {

        // compute branches / lookups
        double avg_branch = ((double) branches) / ((double) lookups);
        // compute ht_count / ht_size
        double ht_load = 100 * (((double) ht_count(ht)) / ((double) ht_size(ht)));
        // compute bf_count / bf_size
        double bf_load = 100 * (((double) bf_count(bf)) / ((double) bf_size(bf)));

        printf("Average BST size: %f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %f\n", ht_avg_bst_height(ht));
        printf("Average branches traversed: %f\n", (avg_branch));
        printf("Hash table load: %f%%\n", ht_load);
        printf("Bloom filter load: %f%%\n", bf_load);
    }

    //Clean up and close
    fclose(infile);
    fclose(bad);
    fclose(new);

    bst_delete(&bad_words);
    bst_delete(&mixed_words);

    ht_delete(&ht);
    bf_delete(&bf);
    return 0;
}
