#ifndef __LAMBDA_TRIE_TREE_H__
#define __LAMBDA_TRIE_TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LAMBDA_TRIE_TREE_MAX_CHILDS_COUNT 26

enum lambda_trie_tree_node_type{
	LAMBDA_TRIE_TREE_NODE_WORD,
	LAMBDA_TRIE_TREE_NODE_NORMAL,
};

struct lambda_trie_tree_node{
	enum lambda_trie_tree_node_type type;
	char                            c;
	struct lambda_trie_tree_node   *childs[LAMBDA_TRIE_TREE_MAX_CHILDS_COUNT];
};

struct lambda_trie_tree_search_set{
	struct lambda_trie_tree_search_set *next;
	struct lambda_trie_tree_search_set *tail;
	char                                data[];
};

struct lambda_trie_tree_node *lambda_trie_tree_create();

struct lambda_trie_tree_node *lambda_trie_tree_node_create(char                            c, 
							   enum lambda_trie_tree_node_type type);

struct lambda_trie_tree_node *lambda_trie_tree_insert(struct lambda_trie_tree_node *tree,
						      const char                   *str,
						      size_t                        len);

struct lambda_trie_tree_node *lambda_trie_tree_find(struct lambda_trie_tree_node *tree,
						    const char                   *str, 
						    size_t                        len);

struct lambda_trie_tree_search_set *lambda_trie_tree_search(struct lambda_trie_tree_node *tree,
							    const char                   *str,
							    size_t                        len,
							    size_t                        max_return);

struct lambda_trie_tree_search_set *__lambda_trie_tree_build_search_set(struct lambda_trie_tree_search_set **set,
									struct lambda_trie_tree_node        *node,
									size_t                              *count,
									size_t                              max_count,
									char                               *tmp_buf,
									int                                *level);

struct lambda_trie_tree_node *__lambda_trie_tree_find(struct lambda_trie_tree_node *tree,
						      const char                   *str, 
						      size_t                        len,
						      char                         *tmp_buf);


struct lambda_trie_tree_search_set *lambda_trie_tree_search_set_add(struct lambda_trie_tree_search_set **set,
								    const char                          *str,
								    size_t                               len);

void lambda_trie_tree_print(struct lambda_trie_tree_node *tree);

int lambda_trie_tree_destroy(struct lambda_trie_tree_node *tree);

#endif /* !__TRIE_TREE_H__ */
