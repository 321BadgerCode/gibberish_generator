/**
 * @file main.c
 * @brief A simple Markov chain text generator.
 * @author Badger Code
 * @date 2025-4-18
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define constants
#define NPREF 2
#define MAX_WORD 100
#define HASHSIZE 4093
#define NONWORD "\n"

/**
 * @class Suffix
 * @brief Structure to hold suffixes for a given prefix.
 * @param word Pointer to the word.
 * @param next Pointer to the next suffix in the linked list.
 */
typedef struct Suffix {
	char *word;
	struct Suffix *next;
} Suffix;

/**
 * @class State
 * @brief Structure to hold the state of the Markov chain.
 * @param prefix Array of prefixes.
 * @param suffixes Pointer to the linked list of suffixes.
 * @param next Pointer to the next state in the hash table.
 */
typedef struct State {
	char *prefix[NPREF];
	Suffix *suffixes;
	struct State *next;
} State;

State *statetab[HASHSIZE];

/**
 * @brief Hash function to compute the hash value for a given prefix.
 * @param s Array of strings representing the prefix.
 * @return Hash value.
 */
unsigned int hash(char *s[]) {
	unsigned int h = 0;
	for (int i = 0; i < NPREF; i++) {
		for (char *p = s[i]; *p != '\0'; p++)
			h = h * 31 + (unsigned char)*p;
	}
	return h % HASHSIZE;
}

/**
 * @brief Duplicate a string safely.
 * @param s Pointer to the string to duplicate.
 * @return Pointer to the duplicated string.
 */
char *strdup_safe(const char *s) {
	char *p = malloc(strlen(s) + 1);
	if (p == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}
	strcpy(p, s);
	return p;
}

/**
 * @brief Lookup a prefix in the hash table.
 * @param prefix Array of strings representing the prefix.
 * @param create Flag to indicate whether to create a new state if not found.
 * @return Pointer to the state corresponding to the prefix.
 */
State *lookup(char *prefix[], int create) {
	unsigned int h = hash(prefix);
	State *sp = statetab[h];
	for (; sp != NULL; sp = sp->next) {
		int i;
		for (i = 0; i < NPREF; i++) {
			if (strcmp(prefix[i], sp->prefix[i]) != 0)
				break;
		}
		if (i == NPREF)
			return sp;
	}
	if (create) {
		sp = malloc(sizeof(State));
		if (sp == NULL) {
			fprintf(stderr, "malloc failed\n");
			exit(1);
		}
		for (int i = 0; i < NPREF; i++)
			sp->prefix[i] = strdup_safe(prefix[i]);
		sp->suffixes = NULL;
		sp->next = statetab[h];
		statetab[h] = sp;
	}
	return sp;
}

/**
 * @brief Add a suffix to the state.
 * @param sp Pointer to the state.
 * @param suffix Pointer to the suffix to add.
 */
void add_suffix(State *sp, char *suffix) {
	Suffix *suf = malloc(sizeof(Suffix));
	if (suf == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}
	suf->word = strdup_safe(suffix);
	suf->next = sp->suffixes;
	sp->suffixes = suf;
}

/**
 * @brief Add a word to the Markov chain.
 * @param prefix Array of strings representing the prefix.
 * @param suffix Pointer to the suffix to add.
 */
void add(char *prefix[], char *suffix) {
	State *sp = lookup(prefix, 1);
	add_suffix(sp, suffix);
	memmove(prefix, prefix + 1, (NPREF - 1) * sizeof(prefix[0]));
	prefix[NPREF - 1] = suffix;
}

/**
 * @brief Build the Markov chain from the input file.
 * @param f Pointer to the input file.
 */
void build(FILE *f) {
	char buf[MAX_WORD];
	char *prefix[NPREF];

	for (int i = 0; i < NPREF; i++)
		prefix[i] = (char *)NONWORD;

	while (fscanf(f, "%99s", buf) != EOF) {
		add(prefix, strdup_safe(buf));
	}

	add(prefix, NONWORD);
}

/**
 * @brief Generate text using the Markov chain.
 * @param nwords Number of words to generate.
 */
void generate(int nwords) {
	char *prefix[NPREF];
	for (int i = 0; i < NPREF; i++)
		prefix[i] = (char *)NONWORD;

	for (int i = 0; i < nwords; i++) {
		State *sp = lookup(prefix, 0);
		if (!sp)
			break;

		int n = 0;
		for (Suffix *suf = sp->suffixes; suf != NULL; suf = suf->next)
			n++;

		if (n == 0)
			break;

		int r = rand() % n;
		Suffix *suf = sp->suffixes;
		for (int j = 0; j < r; j++)
			suf = suf->next;

		if (strcmp(suf->word, NONWORD) == 0)
			break;

		printf("%s ", suf->word);

		prefix[0] = prefix[1];
		prefix[1] = suf->word;
	}

	printf("\n");
}

/**
 * @brief Main function to read input file and generate text.
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return Exit status: 0 on success, non-zero on failure.
 */
int main(int argc, char *argv[]) {
	int nwords = 100;
	FILE *f;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s file [nwords]\n", argv[0]);
		return 1;
	}

	if ((f = fopen(argv[1], "r")) == NULL) {
		perror(argv[1]);
		return 1;
	}

	if (argc >= 3)
		nwords = atoi(argv[2]);

	srand((unsigned int)time(NULL));
	build(f);
	fclose(f);
	generate(nwords);
	return 0;
}