#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings


// Given a target plaintext word, use it to try to find
// a matching hash in the hashFile.
// Get this function working first!
char * tryWord(char * plaintext, char * hashFilename)
{
    // Hash the plaintext
    char *hash = md5(plaintext, strlen(plaintext));    // Create MD5 hash.
    if (!hash) {
        printf("Failed to create MD5 hash for %s\n", plaintext);
        return NULL;
    }
    //printf("%s is hash of %s\n", hash, plaintext);    // display hash for troubleshooting

    // Open the hash file
    FILE *hashFile = fopen(hashFilename, "r");
    if (!hashFile) {
        perror("Error opening file");
        printf("Could not open %s for reading\n", hashFilename);
        free(hash);  // Free the allocated memory for hash
        exit(1);
    }

    // Loop through the hash file, one line at a time.
    char line[HASH_LEN];
    while (fgets(line, sizeof(line), hashFile)) {
        // Remove newline character if present
        char *newline = strchr(line, '\n');
        if (newline) {
            *newline = '\0';
        }

        // Compare the hash with the current line in the file
        if (strcmp(hash, line) == 0) {
            fclose(hashFile);
            return hash;  // Return the matching hash
            free(hash);  // Free the allocated memory for hash
        
        }
    }

    fclose(hashFile);
    free(hash);  // Free the allocated memory for hash
    return NULL;  // No match found
}


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }


    // Open the dictionary file for reading.
    // output file will contain the md5 hash of the input file
    FILE *dictFile = fopen(argv[2], "r");
    if (!dictFile) {
        printf("Could not open %s for writing\n", argv[2]);
        exit(1);
    }

    // For each dictionary word, pass it to tryWord, which
    // will attempt to match it against the hashes in the hash_file.
    int cracked = 0;
    char line[PASS_LEN];
    while (fgets(line, PASS_LEN, dictFile)) {
        // find the newline character and truncate the line
        char *newline = strchr(line, '\n');
        if (newline) {
            *newline = '\0';
        } 
        // if no newline, then the line is the maximum length
        if (!newline) {
            line[strlen(line)] = '\0';
        }

        char *found = tryWord(line, argv[1]);
        if (found) {
        
            printf("%s %s\n", found, line);
            
            cracked++;
        }
    }



    // If we got a match, display the hash and the word. For example:
    //   5d41402abc4b2a76b9719d911017c592 hello
    
    // Close the dictionary file.
    fclose(dictFile);

    // Display the number of hashes that were cracked.
    printf("Cracked %d hashes!\n", cracked);
    
    // Free up any malloc'd memory?

    
}

