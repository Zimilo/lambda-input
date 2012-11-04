#ifndef __LAMBDA_DICT_H__
#define __LAMBDA_DICT_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lambda_trie.h"

struct lambda_dict {
	const char                   *dict_name;     /* the name of the current dict */
	const char                   *dict_path;     /* the absolute path of the current dict */
	struct lambda_trie_tree_node *trie_tree;     /* the trie tree built by the current dict */
};

struct lambda_completation_limit {
	size_t max_completation_size;
};

struct lambda_dict *lambda_dict_open(const char *name, const char *path);

int lambda_dict_close(struct lambda_dict *dict);

struct lambda_trie_tree_search_set *lambda_dict_complete(struct lambda_dict               *dict, 
							 const char                       *str, 
							 size_t                            len, 
							 struct lambda_completation_limit *limits);

struct lambda_dict *__lambda_dict_load(const char *name, const char *path);

#endif /* !__LAMBDA_DICT_H */
