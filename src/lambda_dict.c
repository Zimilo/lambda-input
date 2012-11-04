#include "lambda_dict.h"
#include "lambda_trie.h"
#include <fcntl.h>
#include <sys/types.h>

struct lambda_dict *lambda_dict_open(const char *name, const char *path) {
	return __lambda_dict_load(name, path);
}

int lambda_dict_close(struct lambda_dict *dict) {
	lambda_trie_tree_destroy(dict->trie_tree);
	free(dict);
	return 0;
}

struct lambda_trie_tree_search_set *lambda_dict_complete(struct lambda_dict               *dict, 
							 const char                       *str, 
							 size_t                            len, 
							 struct lambda_completation_limit *limits) {
	struct lambda_trie_tree_search_set *set = lambda_trie_tree_search(dict->trie_tree, str, len, limits->max_completation_size);
	return set;
}

struct lambda_dict *__lambda_dict_load(const char *name, const char *path) {
	FILE *fp = fopen(path, "rb");
	if (!fp)
		return NULL;

	struct lambda_trie_tree_node *trie_tree = lambda_trie_tree_create();
	if (!trie_tree)
		return NULL;

	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	char *l;
	char buf[1024] = {0};
	while ((linelen = getline(&line, &linecap, fp)) > 0) {
		l = strchr(line, '\t');
		if (!l) 
			continue;
		snprintf(buf, l - line, "%s", line);
		printf("Loaded... |%s|\n", buf);
		lambda_trie_tree_insert(trie_tree, buf, strlen(buf));
	}
	fclose(fp);
	if (line)
		free(line);

	struct lambda_dict *dict = (struct lambda_dict *)malloc(sizeof(*dict));
	if (!dict)
		return NULL;
	dict->dict_name = name;
	dict->dict_path = path;
	dict->trie_tree = trie_tree;
	
	return dict;
}


#if 0

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("./a.out term\n");
		return EXIT_FAILURE;
	}
	struct lambda_dict *dict = lambda_dict_open("sample1", "./../demo/sample.dict");

	struct lambda_completation_limit limit;
	limit.max_completation_size = 10;

	struct lambda_trie_tree_search_set *set = lambda_dict_complete(dict, argv[1], strlen(argv[1]), &limit);

	int idx = 0;
	while (set != NULL) {
		printf("[%d] %s\n", idx, set->data);
		++idx;
		set = set->next;
	}

	return EXIT_SUCCESS;
}

#endif
