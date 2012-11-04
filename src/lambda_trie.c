#include "lambda_trie.h"

struct lambda_trie_tree_node *lambda_trie_tree_create() {
	struct lambda_trie_tree_node *node = lambda_trie_tree_node_create(' ', LAMBDA_TRIE_TREE_NODE_NORMAL);
	return node;
}

struct lambda_trie_tree_node *lambda_trie_tree_node_create(char                     c, 
							   enum lambda_trie_tree_node_type type) {
	struct lambda_trie_tree_node *node = (struct lambda_trie_tree_node *)malloc(sizeof(*node));
	int i;
	if (NULL == node) 
		return NULL;
	node->c = c;
	node->type = type;
	for (i = 0; i < LAMBDA_TRIE_TREE_MAX_CHILDS_COUNT; ++i) {
		node->childs[i] = NULL;
	}
	return node;
}

struct lambda_trie_tree_node *lambda_trie_tree_insert(struct lambda_trie_tree_node *tree,
						      const char                   *str,
						      size_t                       len) {
	if (NULL == tree || str == NULL || 0 == len) 
		return NULL;
	struct lambda_trie_tree_node *t = tree;
	int i, idx;
	char lc;
	for (i = 0; i < len; ++i) {
		lc = tolower(str[i]);
		idx = lc - 'a';
		if (NULL == t->childs[idx]) {
			t->childs[idx] = lambda_trie_tree_node_create(lc, LAMBDA_TRIE_TREE_NODE_NORMAL);
		}
		t = t->childs[idx];
	}
	t->type = LAMBDA_TRIE_TREE_NODE_WORD;
	return t;
}

struct lambda_trie_tree_node *lambda_trie_tree_find(struct lambda_trie_tree_node *tree,
						    const char                   *str, 
						    size_t                        len) {
	return __lambda_trie_tree_find(tree, str, len, NULL);
}

struct lambda_trie_tree_node *__lambda_trie_tree_find(struct lambda_trie_tree_node *tree,
						      const char                   *str, 
						      size_t                        len,
						      char                         *tmp_buf) {
	if (NULL == tree)
		return NULL;
	struct lambda_trie_tree_node *t = tree;
	int i, idx;
	char lc;
	for (i = 0; i < len; ++i) {
		lc = tolower(str[i]);
		idx = lc - 'a';
		if (NULL == t->childs[idx])
			return NULL;
		t = t->childs[idx];
		if (tmp_buf != NULL)
			tmp_buf[i] = lc;
	}
	return t;
}

struct lambda_trie_tree_search_set *lambda_trie_tree_search_set_add(struct lambda_trie_tree_search_set **set,
								    const char                          *str,
								    size_t                               len) {
	if (NULL == set) return NULL;
	struct lambda_trie_tree_search_set *new_set = (struct lambda_trie_tree_search_set *)malloc(sizeof(*new_set) + len + 1);
	if (NULL == new_set) return NULL;
	memcpy(new_set->data, str, len);
	new_set->data[len] = 0;
	new_set->next = NULL;
	new_set->tail = NULL;
	if (NULL == *set)
		*set = new_set;
	else
		(*set)->tail->next = new_set;
	(*set)->tail = new_set;
	return *set;
}

struct lambda_trie_tree_search_set *__lambda_trie_tree_build_search_set(struct lambda_trie_tree_search_set **set,
									struct lambda_trie_tree_node        *node,
									size_t                              *count,
									size_t                               max_count,
									char                                *tmp_buf,
									int                                 *level) {
	tmp_buf[*level] = 0;
	int i;
	*(tmp_buf + *level - 1) = node->c;
	if (node->type == LAMBDA_TRIE_TREE_NODE_WORD) {
		lambda_trie_tree_search_set_add(set, tmp_buf, strlen(tmp_buf));
		++(*count);
	}
	for (i = 0; i < LAMBDA_TRIE_TREE_MAX_CHILDS_COUNT; ++i) {
		if (*count >= max_count) return *set;
		if (node->childs[i] != NULL) {
			++(*level);
			__lambda_trie_tree_build_search_set(set, node->childs[i], count, max_count, tmp_buf, level);
			--(*level);
		}
	}
	return *set;
}

struct lambda_trie_tree_search_set *lambda_trie_tree_search(struct lambda_trie_tree_node *tree,
							    const char                   *str,
							    size_t                        len,
							    size_t                        max_return) {
	char tmp_buf[128] = {0};
	struct lambda_trie_tree_node *node = __lambda_trie_tree_find(tree, str, len, tmp_buf);
	size_t saved = 0;
	int level = strlen(tmp_buf);
	struct lambda_trie_tree_search_set *set = NULL;
	if (NULL == node)
		return NULL;
	set = __lambda_trie_tree_build_search_set(&set, node, &saved, max_return, tmp_buf, &level);
	return set;
}

void lambda_trie_tree_print(struct lambda_trie_tree_node *tree) {
	if (NULL == tree) {
		printf("###### Empty Trie Tree ######\n");
		return;
	}
}

int lambda_trie_tree_destroy(struct lambda_trie_tree_node *tree) {
	if (NULL == tree) return 1;
}

#if 0

int main(int argc, char **argv) {
	struct lambda_trie_tree_node *tree = lambda_trie_tree_create();
	lambda_trie_tree_insert(tree, "za", strlen("za"));
	lambda_trie_tree_insert(tree, "zippo", strlen("zippo"));
	lambda_trie_tree_insert(tree, "zero", strlen("zero"));
	struct lambda_trie_tree_search_set *set = lambda_trie_tree_search(tree, "zi", strlen("zi"), 10);
	int idx = 0;
	while (set != NULL) {
		printf("[%d] %s\n", idx, set->data);
		++idx;
		set = set->next;
	}
	return EXIT_SUCCESS;
}

#endif

