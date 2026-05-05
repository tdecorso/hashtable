#include "hashtable.h"
#include "test.h"

int main(void) {

    TEST_SECTION("Hash table allocation") {
        
        int** table = NULL;
        HASHTABLE(table, 4);

        if (!table) exit(EXIT_FAILURE);

        CHECK_BOOL(
            HASH_SIZE(table) == 4,
            "Table size matches given value.",
            ""
        );

        HASHTABLE_FREE(table);
    };

    TEST_SECTION("Hash table insertion") {
        
        int** table = NULL;
        HASHTABLE(table, 32);

        if (!table) exit(EXIT_FAILURE);

        HASHTABLE_PUT(table, HASH_INT(1), 2);

        CHECK_BOOL(
            HASH_COUNT(table) == 1,
            "Count is updated after insertion.",
            ""
        );

        CHECK_BOOL(
            HASHTABLE_GET(table, HASH_INT(1)) == 2,
            "Inserted value matches.",
            ""
        );

        HASHTABLE_FREE(table);
    };

    TEST_SECTION("Hash table string key") {

        char*** table = NULL;
        HASHTABLE(table, 32);
        if (!table) exit(EXIT_FAILURE);
        HASHTABLE_PUT(table, HASH_STRING("John Smith"), "John Smith");
        CHECK_BOOL(
            strcmp(HASHTABLE_GET(table, HASH_STRING("John Smith")), "John Smith") == 0,
            "String key returns correct value.",
            ""
        );
        HASHTABLE_FREE(table);
    };

    TEST_SECTION("Hash table discard") {

        int** table = NULL;
        HASHTABLE(table, 32);
        if (!table) exit(EXIT_FAILURE);
        HASHTABLE_PUT(table, HASH_INT(1), 2);
        HASHTABLE_PUT(table, HASH_INT(1), 3);
        CHECK_BOOL(
            HASHTABLE_GET(table, HASH_INT(1)) == 3,
            "Latest value is on top after two insertions.",
            ""
        );
        HASHTABLE_DISCARD(table, HASH_INT(1));
        CHECK_BOOL(
            HASHTABLE_GET(table, HASH_INT(1)) == 2,
            "Previous value is restored after discard.",
            ""
        );
        HASHTABLE_FREE(table);
    };

    TEST_SECTION("Hash table collision") {

        int** table = NULL;
        HASHTABLE(table, 4);
        if (!table) exit(EXIT_FAILURE);
        // Force a collision: both keys map to the same bucket
        size_t h1 = HASH_INT(0);
        size_t h2 = HASH_INT(4); // 4 % 4 == 0 % 4 == 0
        HASHTABLE_PUT(table, h1, 10);
        HASHTABLE_PUT(table, h2, 20);
        CHECK_BOOL(
            HASH_COUNT(table) == 2,
            "Count is correct after collision.",
            ""
        );
        CHECK_BOOL(
            HASHTABLE_GET(table, h1) == 20,
            "Latest value is on top after collision.",
            ""
        );
        HASHTABLE_DISCARD(table, h1);
        CHECK_BOOL(
            HASHTABLE_GET(table, h1) == 10,
            "Previous value restored after discard on collision.",
            ""
        );
        HASHTABLE_FREE(table);
    };

    TEST_SECTION("Hash table rehash") {

        int** table = NULL;
        HASHTABLE(table, 4);
        if (!table) exit(EXIT_FAILURE);
        // Insert enough to trigger rehash (load > 0.75, so 4 * 0.75 = 3 entries)
        HASHTABLE_PUT(table, HASH_INT(0), 10);
        HASHTABLE_PUT(table, HASH_INT(1), 20);
        HASHTABLE_PUT(table, HASH_INT(2), 30);
        HASHTABLE_PUT(table, HASH_INT(3), 40);
        CHECK_BOOL(
            HASH_SIZE(table) == 8,
            "Table doubled in size after rehash.",
            ""
        );
        CHECK_BOOL(
            HASHTABLE_GET(table, HASH_INT(0)) == 10,
            "Value 0 survives rehash.",
            ""
        );
        CHECK_BOOL(
            HASHTABLE_GET(table, HASH_INT(1)) == 20,
            "Value 1 survives rehash.",
            ""
        );
        CHECK_BOOL(
            HASHTABLE_GET(table, HASH_INT(2)) == 30,
            "Value 2 survives rehash.",
            ""
        );
        CHECK_BOOL(
            HASHTABLE_GET(table, HASH_INT(3)) == 40,
            "Value 3 survives rehash.",
            ""
        );
        HASHTABLE_FREE(table);
    };
    TEST_SUMMARY();

    exit(EXIT_SUCCESS);
}
