# CTreeMap

[![License](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](LICENSE)

适用于 C/C++ 的 TreeMap，用 `search.h` 中的二叉搜索树相关函数实现，使用宏来实现「伪泛型」，生成不同类型的 TreeMap 和相关函数。

## 用法

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ctreemap/ctmap.h"

// 下面的宏调用会生成类型 StrMap （存储 char * 类型数据）以及相关函数
GENERATE_MAP_TYPE(StrMap, char *)

void release_value_callback(StrMap_Entry *e) {
    printf("release_value_callback, key: %s, value: %s\n", e->key, e->value);
    free(e->value);
}

void foreach_callback(const StrMap_Entry *e) {
    printf("foreach_callback, key: %s, value: %s\n", e->key, e->value);
}

int main() {
    StrMap *map = new_StrMap(release_value_callback); // 如果存储的数据类型不需要释放内存，这里 release_value_callback 可以为 NULL
    
    map->put(map, "key", strdup("hello"));
    map->put(map, "key", strdup("hello2"));
    printf("get, key: %s, value: %s\n", "key", map->get(map, "key"));
    map->remove(map, "key");
    printf("get, key: %s, value: %s\n", "key", map->get(map, "key"));
    map->put(map, "key", strdup("hello"));
    map->put(map, "key11", strdup("hello11"));
    map->put(map, "key21", strdup("hello21"));
    map->put(map, "key31", strdup("hello31"));
    map->put(map, "key41", strdup("hello41"));
    map->clear(map);

    map->put(map, "key1", strdup("hello1"));
    map->put(map, "key2", strdup("hello2"));
    map->put(map, "key3", strdup("hello3"));
    map->put(map, "key4", strdup("hello4"));
    map->foreach(map, foreach_callback);

    delete_StrMap(&map);
}
```
