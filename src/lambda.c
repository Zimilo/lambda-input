#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lambda_dict.h"
#include "lambda_trie.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("./lambda_input term\n");
		return EXIT_FAILURE;
	}

	struct lambda_dict *dict = lambda_dict_open("sample1", "./demo/sample.dict");

	struct lambda_completation_limit limit;
	limit.max_completation_size = 10;

	struct lambda_trie_tree_search_set *set = lambda_dict_complete(dict, argv[1], strlen(argv[1]), &limit);

	if (set) {
		int idx = 0;
		while (set != NULL) {
			printf("[%d] %s\n", idx, set->data);
			++idx;
			set = set->next;
		}
	} else {
		printf("Cannot find any suggestions\n");
	}
	return EXIT_SUCCESS;
}
