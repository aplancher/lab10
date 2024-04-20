#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABET_SIZE 26

// Trie structure
struct Trie
{	
    struct Trie *children[ALPHABET_SIZE];
    int end;
    int count;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *current=pTrie;
    for(int i=0;word[i] != '\0';i++) {
        int idx= word[i] - 'a';
        if(!current->children[idx]) 
            current->children[idx]= createTrie();
        
        current= current->children[idx];
    }
    current->end = 1;
    current->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *current= pTrie;
    for(int i=0;word[i] != '\0';i++) {
        int idx= word[i]- 'a';
        if (!current->children[idx])
            return 0;

        current=current->children[idx];
    }
    if(current != NULL && current->end)
        return current->count;
    return 0;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if(pTrie){
        for(int i=0;i<ALPHABET_SIZE;i++) {
            deallocateTrie(pTrie->children[i]);
        }
        free(pTrie);
    }
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    if(node){
        node->end= 0;
        node->count= 0;
        for(int i= 0;i<ALPHABET_SIZE;i++){
            node->children[i]= NULL;
        }
    }
    return node;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file=fopen(filename, "r");
    if(file == NULL)
        exit(1);
    
    int numWords= 0;
    char buffer[256];
    while(fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;
        pInWords[numWords]= strdup(buffer);
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}