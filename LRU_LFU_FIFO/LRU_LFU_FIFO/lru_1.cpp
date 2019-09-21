// @author zhaohongbin
typedef struct queue_s queue_t;

struct queue_s {
	queue_t     *prev;
	queue_t     *next;
};

#define queue_init(q)       \
    (q)->prev = q;          \
    (q)->next = q

#define queue_head(q)       \
    (q)->next

#define queue_last(q)       \
    (q)->prev

#define queue_insert_head(q, v) \
    (v)->prev = q;              \
    (v)->next = (q)->next;      \
    (q)->next->prev = v;        \
    (q)->next = v

#define queue_remove(q)             \
    (q)->prev->next = (q)->next;    \
    (q)->next->prev = (q)->prev

typedef struct hash_pair_s hash_pair_t;
typedef struct cache_node_s cache_node_t;
typedef struct hash_s hash_t;

struct cache_node_s {
	queue_t     queue;
	int         key;
	int         val;
};

struct hash_pair_s {
	cache_node_t    node;
	hash_pair_t     *next;
};

struct hash_s {
	int    size;
	hash_pair_t     **values;
	hash_pair_t     *alloc;
	hash_pair_t     **free_pairs;
	int             free_pairs_n;
};

void hash_free(hash_t *hash) {
	if (hash == NULL) {
		return;
	}

	if (hash->values != NULL) {
		free(hash->values);
	}

	if (hash->alloc != NULL) {
		free(hash->alloc);
	}

	return;
}

hash_pair_t *hash_get_pair(hash_t *hash) {
	hash_pair_t     *pair;

	if (hash->free_pairs_n <= 0) {
		pair = calloc(1, sizeof(hash_pair_t));
		return pair;
	}

	hash->free_pairs_n--;
	pair = hash->free_pairs[hash->free_pairs_n];

	return pair;
}

void hash_free_pair(hash_t *hash, hash_pair_t *pair) {
	if (pair == NULL) {
		return;
	}

	if (hash->free_pairs_n >= hash->size) {
		free(pair);
		return;
	}

	hash->free_pairs[hash->free_pairs_n] = pair;
	hash->free_pairs_n++;

	return;
}

hash_t *hash_create(int size) {
	hash_t          *hash;
	hash_pair_t     *pairs;
	int             i;

	hash = calloc(1, sizeof(hash_t));
	if (hash == NULL) {
		return NULL;
	}

	hash->values = calloc(size * 2, sizeof(hash_pair_t *));
	if (hash->values == NULL) {
		free(hash);
		return NULL;
	}
	hash->free_pairs = hash->values + size;

	pairs = calloc(size, sizeof(hash_pair_t));
	if (pairs == NULL) {
		free(hash->values);
		free(hash);
		return NULL;
	}
	for (i = 0; i < size; i++) {
		hash->free_pairs[i] = &pairs[i];
	}

	hash->alloc = pairs;
	hash->free_pairs_n = size;
	hash->size = size;

	return hash;
}

cache_node_t *hash_insert(hash_t *hash, int key, int value) {
	int             k;
	hash_pair_t     *pair;

	k = abs(key) % hash->size;
	pair = hash->values[k];

	while (pair != NULL) {
		if (pair->node.key == key) {
			pair->node.val = value;
			return &pair->node;
		}

		pair = pair->next;
	}

	pair = hash_get_pair(hash);
	if (pair == NULL) {
		return;
	}

	pair->node.key = key;
	pair->node.val = value;
	pair->next = hash->values[k];
	hash->values[k] = pair;

	return &pair->node;
}

cache_node_t *hash_find(hash_t *hash, int key) {
	int             k;
	hash_pair_t     *pair;

	k = abs(key) % hash->size;
	pair = hash->values[k];

	while (pair != NULL) {
		if (pair->node.key == key) {
			return &pair->node;
		}

		pair = pair->next;
	}

	return NULL;
}


void hash_remove(hash_t *hash, cache_node_t *node) {
	int             k;
	hash_pair_t     *pair, *pre;

	k = abs(node->key) % hash->size;
	pair = hash->values[k];
	pre = NULL;

	while (pair != NULL) {
		if (&pair->node == node) {
			break;
		}

		pre = pair;
		pair = pair->next;
	}

	if (pair == NULL) {
		return;
	}

	if (pre == NULL) {
		hash->values[k] = pair->next;
	}
	else {
		pre->next = pair->next;
	}

	hash_free_pair(hash, pair);
	return;
}

typedef struct {
	queue_t     queue;
	int         size;
	int         index;
	hash_t      *hash;
} LRUCache;

LRUCache* lRUCacheCreate(int capacity) {
	LRUCache    *cache;

	cache = calloc(1, sizeof(LRUCache));
	if (cache == NULL) {
		return NULL;
	}

	cache->hash = hash_create(capacity);
	if (cache->hash == NULL) {
		free(cache);
		return NULL;
	}

	cache->size = capacity;
	cache->index = 0;
	queue_init(&cache->queue);

	return cache;
}

int lRUCacheGet(LRUCache* obj, int key) {
	cache_node_t    *node;

	node = hash_find(obj->hash, key);
	if (node == NULL) {
		return -1;
	}

	queue_remove(&node->queue);
	queue_insert_head(&obj->queue, &node->queue);

	return node->val;
}

void lRUCachePut(LRUCache* obj, int key, int value) {
	cache_node_t    *node, *node2;
	queue_t         *queue;

	node = hash_find(obj->hash, key);

	if (node == NULL) {
		if (obj->index >= obj->size) {
			queue = queue_last(&obj->queue);
			queue_remove(queue);
			node = (cache_node_t *)queue;
			hash_remove(obj->hash, node);
		}
		else {
			obj->index++;
		}

		node = hash_insert(obj->hash, key, value);
	}
	else {
		node->val = value;
		queue_remove(&node->queue);
	}

	queue_insert_head(&obj->queue, &node->queue);

	return;
}

void lRUCacheFree(LRUCache* obj) {
	if (obj == NULL) {
		return;
	}

	if (obj->hash != NULL) {
		hash_free(obj->hash);
	}

	free(obj);
}