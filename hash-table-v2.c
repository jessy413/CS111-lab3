#include "hash-table-base.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <pthread.h>

struct list_entry
{
	const char *key;
	uint32_t value;
	SLIST_ENTRY(list_entry)
	pointers;
};

SLIST_HEAD(list_head, list_entry);

struct hash_table_entry
{
	struct list_head list_head;
};

struct hash_table_v2
{
	struct hash_table_entry entries[HASH_TABLE_CAPACITY];

	pthread_mutex_t m[HASH_TABLE_CAPACITY];
};

struct hash_table_v2 *hash_table_v2_create()
{
	struct hash_table_v2 *hash_table = calloc(1, sizeof(struct hash_table_v2));
	assert(hash_table != NULL);
	for (size_t i = 0; i < HASH_TABLE_CAPACITY; ++i)
	{
		struct hash_table_entry *entry = &hash_table->entries[i];
		SLIST_INIT(&entry->list_head);

		pthread_mutex_init(m[i], NULL);
	}
	return hash_table;
}

static struct hash_table_entry *get_hash_table_entry(struct hash_table_v2 *hash_table,
													 const char *key)
{
	assert(key != NULL);
	uint32_t index = bernstein_hash(key) % HASH_TABLE_CAPACITY;
	struct hash_table_entry *entry = &hash_table->entries[index];
	return entry;
}

static struct list_entry *get_list_entry(struct hash_table_v2 *hash_table,
										 const char *key,
										 struct list_head *list_head)
{
	assert(key != NULL);

	struct list_entry *entry = NULL;

	SLIST_FOREACH(entry, list_head, pointers)
	{
		if (strcmp(entry->key, key) == 0)
		{
			return entry;
		}
	}

	return NULL;
}

bool hash_table_v2_contains(struct hash_table_v2 *hash_table,
							const char *key)
{
	struct hash_table_entry *hash_table_entry = get_hash_table_entry(hash_table, key);
	struct list_head *list_head = &hash_table_entry->list_head;
	struct list_entry *list_entry = get_list_entry(hash_table, key, list_head);
	return list_entry != NULL;
}

void hash_table_v2_add_entry(struct hash_table_v2 *hash_table,
							 const char *key,
							 uint32_t value)
{
	static pthread_mutex_t mutex5 = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
	static pthread_mutex_t mutex4 = PTHREAD_MUTEX_INITIALIZER;

	uint32_t index = bernstein_hash(key) % HASH_TABLE_CAPACITY;
	pthread_mutex_lock(hash_table[index]);

	struct hash_table_entry *hash_table_entry = get_hash_table_entry(hash_table, key);
	struct list_head *list_head = &hash_table_entry->list_head;
	// pthread_mutex_lock(&mutex1);
	struct list_entry *list_entry = get_list_entry(hash_table, key, list_head);
	// pthread_mutex_unlock(&mutex1);

	/* Update the value if it already exists */
	if (list_entry != NULL)
	{
		// int lock = pthread_mutex_lock(&mutex2);
		// if (lock != 0)
		// exit(lock);
		list_entry->value = value;
		// int unlock = pthread_mutex_unlock(&mutex2);
		// if (unlock != 0)
		// exit(unlock);
		return;
	}
	// calloc doesn't need a lock since it's atomic
	list_entry = calloc(1, sizeof(struct list_entry));

	// pthread_mutex_lock(&mutex4);
	list_entry->key = key;
	list_entry->value = value;
	// pthread_mutex_unlock(&mutex4);

	// pthread_mutex_lock(&mutex3);
	SLIST_INSERT_HEAD(list_head, list_entry, pointers);
	// int unlock = pthread_mutex_unlock(&mutex3);
	// if (unlock != 0)
	// 	exit(unlock);

	pthread_mutex_lock(hash_table[index]);
}

uint32_t hash_table_v2_get_value(struct hash_table_v2 *hash_table,
								 const char *key)
{
	struct hash_table_entry *hash_table_entry = get_hash_table_entry(hash_table, key);
	struct list_head *list_head = &hash_table_entry->list_head;
	struct list_entry *list_entry = get_list_entry(hash_table, key, list_head);
	assert(list_entry != NULL);
	return list_entry->value;
}

void hash_table_v2_destroy(struct hash_table_v2 *hash_table)
{
	for (size_t i = 0; i < HASH_TABLE_CAPACITY; ++i)
	{
		struct hash_table_entry *entry = &hash_table->entries[i];
		struct list_head *list_head = &entry->list_head;
		struct list_entry *list_entry = NULL;
		while (!SLIST_EMPTY(list_head))
		{
			list_entry = SLIST_FIRST(list_head);
			SLIST_REMOVE_HEAD(list_head, pointers);
			free(list_entry);
		}
	}
	free(hash_table);
}
