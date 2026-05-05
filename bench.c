#include "bench.h"
#include "hashtable.h"

#define STRING_TABLE_SETUP(table) do {                              \
    HASHTABLE(table, 32);                                           \
    HASHTABLE_PUT(table, HASH_STRING("John Smith"), "John Smith");  \
    HASHTABLE_PUT(table, HASH_STRING("Lisa Smith"), "Lisa Smith");  \
    HASHTABLE_PUT(table, HASH_STRING("Mark Twain"), "Mark Twain");  \
    HASHTABLE_PUT(table, HASH_STRING("Jane Doe"),   "Jane Doe");    \
} while (0)

int main(void) {

    BENCH("HASHTABLE_PUT int", 1000, 100, 10, {
        int** table = NULL;
        HASHTABLE(table, 32);
        for (int i = 0; i < 100; i++)
            HASHTABLE_PUT(table, HASH_INT(i), i);
        HASHTABLE_FREE(table);
    });

    BENCH("HASHTABLE_PUT int no rehash", 1000, 100, 10, {
        int** table = NULL;
        HASHTABLE(table, 256);
        for (int i = 0; i < 100; i++)
            HASHTABLE_PUT(table, HASH_INT(i), i);
        HASHTABLE_FREE(table);
    });

    BENCH("HASHTABLE_GET int", 1000, 100, 10, {
        int** table = NULL;
        HASHTABLE(table, 32);
        for (int i = 0; i < 100; i++)
            HASHTABLE_PUT(table, HASH_INT(i), i);
        for (int i = 0; i < 100; i++)
            (void)HASHTABLE_GET(table, HASH_INT(i));
        HASHTABLE_FREE(table);
    });

    BENCH("HASHTABLE_PUT string", 1000, 100, 10, {
        char*** table = NULL;
        STRING_TABLE_SETUP(table);
        HASHTABLE_FREE(table);
    });

    BENCH("HASHTABLE_GET string", 1000, 100, 10, {
        char*** table = NULL;
        STRING_TABLE_SETUP(table);
        (void)HASHTABLE_GET(table, HASH_STRING("John Smith"));
        (void)HASHTABLE_GET(table, HASH_STRING("Lisa Smith"));
        (void)HASHTABLE_GET(table, HASH_STRING("Mark Twain"));
        (void)HASHTABLE_GET(table, HASH_STRING("Jane Doe"));
        HASHTABLE_FREE(table);
    });

    BENCH("HASHTABLE_PUT string 100", 1000, 100, 10, {
        char*** table = NULL;
        HASHTABLE(table, 256);
        for (int i = 0; i < 100; i++)
            HASHTABLE_PUT(table, HASH_STRING("John Smith"), "John Smith");
        HASHTABLE_FREE(table);
    });

    BENCH("HASHTABLE_REHASH", 1000, 10, 5, {
        int** table = NULL;
        HASHTABLE(table, 4);
        for (int i = 0; i < 24; i++)
            HASHTABLE_PUT(table, HASH_INT(i), i);
        HASHTABLE_FREE(table);
    });

    BENCH("HASHTABLE_DISCARD", 1000, 100, 10, {
        int** table = NULL;
        HASHTABLE(table, 32);
        for (int i = 0; i < 10; i++)
            HASHTABLE_PUT(table, HASH_INT(0), i);
        for (int i = 0; i < 10; i++)
            HASHTABLE_DISCARD(table, HASH_INT(0));
        HASHTABLE_FREE(table);
    });

    exit(EXIT_SUCCESS);
}
