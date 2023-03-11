//======== Copyright (c) 2022, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     MICHAL BALOGH <xbalog06@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author MICHAL BALOGH
 *
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

#include <string>


using namespace ::testing;
using namespace std;

class NonEmptyMap : public Test
{
protected:
    hash_map_t *map;

    void SetUp() override
    {
        map = hash_map_ctor();
        auto keys = {"Monkey", "Volant", "Macka", "Wooof", "fox"};
        int values[] = {45, 80, 411, 2003, 1};
        int i = 0;
        for (auto key : keys)
        {
            hash_map_put(map, key, values[i]);
            i++;
        }
    }
    void TearDown() override
    {
        hash_map_dtor(map);
    }
};

class EmptyMap : public Test
{
protected:
    hash_map_t *map;

    void SetUp() override
    {
        map = hash_map_ctor();
    }
    void TearDown() override
    {
        hash_map_dtor(map);
    }
};


//==============================================================================//

TEST(HashMapInitialization, hash_map_ctor)
{
    auto map = hash_map_ctor();
    EXPECT_TRUE(map != NULL);
    EXPECT_EQ(hash_map_capacity(map), 8);
    EXPECT_EQ(hash_map_size(map), 0);
    hash_map_dtor(map);
    EXPECT_EQ(map->allocated, 0);
    EXPECT_TRUE(map->first != NULL);
    EXPECT_TRUE(map->last != NULL);
    EXPECT_EQ(map->used, 0);
}

TEST_F(NonEmptyMap, HashMapInitialization)
{
    EXPECT_TRUE(map != NULL);
    EXPECT_EQ(hash_map_capacity(map), 8);
    EXPECT_EQ(hash_map_size(map), 5);
}

TEST_F(EmptyMap, HashMapInitialization)
{
    EXPECT_TRUE(map != NULL);
    EXPECT_EQ(hash_map_capacity(map), 8);
    EXPECT_EQ(hash_map_size(map), 0);
}

TEST_F(NonEmptyMap, hash_map_clear)
{
    auto size = hash_map_size(map);
    hash_map_clear(map);
    for (auto i = 0; i < map->allocated; ++i)
    {
        EXPECT_TRUE(map->index[i] == NULL);
    }
    EXPECT_EQ(map->used, 0);
    EXPECT_TRUE(map->first == NULL);
    EXPECT_TRUE(map->last == NULL);
}

TEST_F(EmptyMap, hash_map_clear)
{
    hash_map_clear(map);
    EXPECT_TRUE(map->index[0] == NULL);
    EXPECT_EQ(map->used, 0);
    EXPECT_TRUE(map->first == NULL);
    EXPECT_TRUE(map->last == NULL);
}

TEST_F(NonEmptyMap, hash_map_reserve_OK)
{
    auto result = hash_map_reserve(map, 16);
    EXPECT_EQ(result, OK);
    EXPECT_EQ(hash_map_capacity(map), 16);
}

TEST_F(EmptyMap, hash_map_reserve_OK)
{
    auto result = hash_map_reserve(map, 16);
    EXPECT_EQ(result, OK);
    EXPECT_EQ(hash_map_capacity(map), 16);
}

TEST_F(NonEmptyMap, hash_map_reserve_VALUE_ERROR)
{
    auto result = hash_map_reserve(map, 4);
    EXPECT_EQ(result, VALUE_ERROR);
    EXPECT_EQ(hash_map_capacity(map), 8);
}

TEST_F(NonEmptyMap, hash_map_size)
{
    EXPECT_EQ(hash_map_size(map), 5);
}

TEST_F(EmptyMap, hash_map_size)
{
    EXPECT_EQ(hash_map_size(map), 0);
}

TEST_F(NonEmptyMap, hash_map_size_add_key)
{
    EXPECT_EQ(hash_map_size(map), 5);
    EXPECT_EQ(hash_map_put(map, "Joke", 111), OK);
    EXPECT_EQ(hash_map_size(map), 6);
}

TEST_F(EmptyMap, hash_map_size_add_key)
{
    EXPECT_EQ(hash_map_size(map), 0);
    EXPECT_EQ(hash_map_put(map, "Joke", 111), OK);
    EXPECT_EQ(hash_map_size(map), 1);
    EXPECT_EQ(map->first->value, 111);
    EXPECT_EQ(map->last->value, 111);
}

TEST_F(NonEmptyMap, hash_map_size_remove_key)
{
    EXPECT_EQ(hash_map_size(map), 5);
    EXPECT_EQ(hash_map_remove(map, "fox"), OK);
    EXPECT_EQ(hash_map_size(map), 5);
    EXPECT_EQ(map->used, 5);
    EXPECT_TRUE(map->dummy != NULL);
}

TEST_F(EmptyMap, hash_map_size_remove_key)
{
    EXPECT_EQ(hash_map_size(map), 0);
    EXPECT_EQ(hash_map_remove(map, "fox"), KEY_ERROR);
    EXPECT_EQ(hash_map_size(map), 0);
}

TEST_F(NonEmptyMap, hash_map_capacity)
{
    EXPECT_EQ(hash_map_capacity(map), 8);
    auto result = hash_map_reserve(map, 10);
    EXPECT_EQ(result, OK);
    EXPECT_EQ(hash_map_capacity(map), 10);
}

TEST_F(EmptyMap, hash_map_capacity)
{
    EXPECT_EQ(hash_map_capacity(map), 8);
    auto result = hash_map_reserve(map, 14);
    EXPECT_EQ(result, OK);
    EXPECT_EQ(hash_map_capacity(map), 14);
}

TEST_F(NonEmptyMap, hash_map_capacity_add_key)
{
    EXPECT_EQ(hash_map_capacity(map), 8);
    EXPECT_EQ(hash_map_put(map, "Gigantosaurus", 225), OK);
    EXPECT_EQ(hash_map_capacity(map), 16);
}

TEST_F(EmptyMap, hash_map_capacity_add_key)
{
    EXPECT_EQ(hash_map_capacity(map), 8);
    EXPECT_EQ(hash_map_put(map, "Gigantosaurus", 225), OK);
    EXPECT_EQ(hash_map_capacity(map), 8);
}

TEST_F(NonEmptyMap, hash_map_capacity_remove_key)
{
    EXPECT_EQ(hash_map_remove(map, "fox"), OK);
    EXPECT_EQ(hash_map_capacity(map), 8);
}

TEST_F(NonEmptyMap, hash_map_capacity_pop_key)
{
    int value;
    EXPECT_EQ(hash_map_pop(map, "fox", &value), OK);
    EXPECT_EQ(hash_map_capacity(map), 8);
}

TEST_F(NonEmptyMap, hash_map_capacity_pop_key_first)
{
    int value;
    EXPECT_EQ(hash_map_pop(map, "Monkey", &value), OK);
    EXPECT_EQ(map->first->value, 80);
    EXPECT_EQ(hash_map_capacity(map), 8);
}

TEST_F(EmptyMap, hash_map_capacity_pop_key)
{
    int value;
    EXPECT_EQ(hash_map_pop(map, "fox", &value), KEY_ERROR);
    EXPECT_EQ(hash_map_capacity(map), 8);
}

TEST_F(NonEmptyMap, hash_map_contains_succes)
{
    EXPECT_TRUE(hash_map_contains(map, "fox") != 0);
}

TEST_F(NonEmptyMap, hash_map_contains_failure)
{
    EXPECT_EQ(hash_map_contains(map, "Managarmr"), 0);
}

TEST_F(EmptyMap, hash_map_contains)
{
    EXPECT_EQ(hash_map_contains(map, "fox"), 0);
}

TEST_F(NonEmptyMap, hash_map_get_succes)
{
    int value;
    EXPECT_EQ(hash_map_get(map, "fox", &value), OK);
    EXPECT_EQ(value, 1);
}

TEST_F(NonEmptyMap, hash_map_get_failure)
{
    int value = 0;
    EXPECT_EQ(hash_map_get(map, "RYZA", &value), KEY_ERROR);
    EXPECT_EQ(value, 0);
}

TEST_F(NonEmptyMap, hash_map_get)
{
    int value;
    EXPECT_EQ(hash_map_get(map, "fox", &value), OK);
    EXPECT_EQ(value, 1);
    EXPECT_EQ(hash_map_get(map, "fox", &value), OK);
    EXPECT_EQ(value, 1);
    EXPECT_EQ(hash_map_remove(map, "fox"), OK);
    EXPECT_EQ(hash_map_get(map, "fox", &value), KEY_ERROR);
    EXPECT_EQ(value, 1);

}

TEST_F(EmptyMap, hash_map_get)
{
    int value = 0;
    EXPECT_EQ(hash_map_get(map, "RYZA", &value), KEY_ERROR);
    EXPECT_EQ(value, 0);
}


TEST_F(NonEmptyMap, hash_map_put)
{
    EXPECT_EQ(hash_map_put(map, "Managarmr", 46), OK);
    EXPECT_EQ(hash_map_size(map), 6);
    EXPECT_EQ(hash_map_capacity(map), 16);
    EXPECT_EQ(hash_map_put(map, "Managarmr", 46), KEY_ALREADY_EXISTS);
    EXPECT_EQ(hash_map_put(map, "Goodnight", 46), OK);
}

TEST_F(EmptyMap, hash_map_put)
{
    EXPECT_EQ(hash_map_put(map, "Managarmr", 23), OK);
    EXPECT_EQ(hash_map_size(map), 1);
    EXPECT_EQ(map->first->value, 23);
    EXPECT_EQ(map->last->value, 23);
    EXPECT_EQ(hash_map_capacity(map), 8);
    EXPECT_EQ(hash_map_put(map, "Managarmr", 23), KEY_ALREADY_EXISTS);
    EXPECT_EQ(hash_map_put(map, "Goodnight", 23), OK);
    EXPECT_EQ(hash_map_size(map), 2);
    EXPECT_EQ(map->last->value, 23);
}

/*** Konec souboru white_box_tests.cpp ***/
