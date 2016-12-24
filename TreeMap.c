//
// Created by Richard Chien on 8/27/16.
//

#include "TreeMap.h"

#include <search.h>

typedef struct _BaseEntry {
    char *key;
} BaseEntry;

typedef struct _BaseMap {
    void *root;

    void (*release_value_callback)(void *entry);
} BaseMap;

int entry_comparator(const void *a, const void *b) {
    return a == b ? 0 : strcmp(((const BaseEntry *) a)->key, ((const BaseEntry *) b)->key);
}

void __TreeMap_insert_entry(void *_self, void *_entry) {
    BaseMap *self = _self;
    BaseEntry *entry = _entry;

    BaseEntry **rv = tsearch(entry, &self->root, entry_comparator);
    if (*rv != entry) {
        // The pointers "*rv" and "entry" are different,
        // which means "*rv" is the previous entry with same key as "entry"
        // So here we release the previous entry "*rv" and give it the new entry.
        if (self->release_value_callback) {
            self->release_value_callback(*rv);
        }
        free((*rv)->key);
        free(*rv);
        *rv = entry;
    }
}

void *__TreeMap_find_entry(void *_self, const char *key) {
    BaseMap *self = _self;
    BaseEntry entry;
    entry.key = strdup(key);
    void **result = tfind(&entry, &self->root, entry_comparator);
    free(entry.key);
    return result ? *result : NULL;
}

void __TreeMap_remove(void *_self, const char *key) {
    BaseMap *self = _self;
    BaseEntry entry;
    entry.key = strdup(key);
    BaseEntry **rv = tfind(&entry, &self->root, entry_comparator);
    free(entry.key);
    if (rv) {
        // The entry does exist, so we release memory of it.
        if (self->release_value_callback) {
            self->release_value_callback(*rv);
        }
        free((*rv)->key);
        free(*rv);
        // Remove the entry from tree.
        tdelete(*rv, &self->root, entry_comparator);
    }
}

static __thread int entry_count = 0;

void count_entry_traverse_callback(const void *entry) {
    entry_count++;
}

static __thread const void **entry_array = NULL;
static __thread int entry_array_cursor = 0;

void collect_entry_traverse_callback(const void *entry) {
    entry_array[entry_array_cursor++] = entry;
}

void __TreeMap_clear(void *_self) {
    BaseMap *self = _self;

    entry_count = 0;
    __TreeMap_traverse(self, count_entry_traverse_callback);

    entry_array = malloc(entry_count * sizeof(void *));
    entry_array_cursor = 0;
    __TreeMap_traverse(self, collect_entry_traverse_callback);

    for (int i = 0; i < entry_count; i++) {
        BaseEntry *entry = entry_array[i];

        // Release memory of the entry.
        if (self->release_value_callback) {
            self->release_value_callback(entry);
        }
        free(entry->key);
        free(entry);
        // Remove the entry from tree.
        tdelete(entry, &self->root, entry_comparator);
    }

    free(entry_array);
}

static __thread void (*traverse_callback)(const void *entry) = NULL;

void traverse_action(const void *nodep, VISIT which, int depth) {
    if (traverse_callback && (which == preorder || which == leaf)) {
        void *e = *(void **) nodep;
        traverse_callback(e);
    }
}

void __TreeMap_traverse(void *_self, void (*_traverse_callback)(const void *entry)) {
    BaseMap *self = _self;
    traverse_callback = _traverse_callback;
    twalk(self->root, traverse_action);
}

void __TreeMap_destroy(void **pself) {
    BaseMap *self = *pself;
    __TreeMap_clear(self);
    free(self->root);
    free(self);
    *pself = NULL;
}
