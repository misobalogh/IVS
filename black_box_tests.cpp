//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Michal Balogh <xbalog06@stud.fit.vutbr.cz>
// $Date:       $2023-02-21
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Michal Balogh
 *
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "red_black_tree.h"

using namespace ::testing;

class NonEmptyTree : public Test
{

protected:
    BinaryTree tree;
    std::vector<std::pair<bool, BinaryTree::Node_t *>> treeSetUp;

    void SetUp() override
    {
        std::vector<int> keys = {5, 14, 7, 8, 2, 12};
        tree.InsertNodes(keys, treeSetUp);
    }
};

class EmptyTree : public Test
{

protected:
    BinaryTree tree;
};

class TreeAxioms : public Test
{

protected:
    BinaryTree tree;
    std::vector<std::pair<bool, BinaryTree::Node_t *>> treeSetUp;

    void SetUp() override
    {
        std::vector<int> keys = {2, 1, 4, 5};
        tree.InsertNodes(keys, treeSetUp);
    }
};

//===============NonEmptyTree===============================================================//

TEST_F(NonEmptyTree, InsertNode)
{
    auto newNode = tree.InsertNode(10);
    EXPECT_TRUE(newNode.first);
    EXPECT_EQ(newNode.second->key, 10);
    auto newNode2 = tree.InsertNode(-10);
    EXPECT_TRUE(newNode2.first);
    EXPECT_EQ(newNode2.second->key, -10);
    auto newNode3 = tree.InsertNode(0);
    EXPECT_TRUE(newNode3.first);
    EXPECT_EQ(newNode3.second->key, 0);
}

TEST_F(NonEmptyTree, InsertNode_NodeExisting)
{
    auto newNode = tree.InsertNode(8);
    EXPECT_FALSE(newNode.first);
    EXPECT_EQ(newNode.second, tree.FindNode(8));
}

TEST_F(NonEmptyTree, DeleteNode)
{
    EXPECT_TRUE(tree.DeleteNode(14));
}

TEST_F(NonEmptyTree, DeleteNode_NodeNotInTree)
{
    EXPECT_FALSE(tree.DeleteNode(0));
}

TEST_F(NonEmptyTree, FindNode)
{
    auto nodeToFind = tree.FindNode(5);
    EXPECT_TRUE(nodeToFind != NULL);
}

TEST_F(NonEmptyTree, FindNode_DeleteNode_TryFind)
{
    auto nodeToFind = tree.FindNode(5);
    EXPECT_TRUE(nodeToFind != NULL);
    EXPECT_TRUE(tree.DeleteNode(5));
    auto nodeToFind2 = tree.FindNode(5);
    EXPECT_FALSE(nodeToFind2 != NULL);
}

TEST_F(NonEmptyTree, FindNode_NodeNotInTree)
{
    auto nodeToFind = tree.FindNode(0);
    EXPECT_TRUE(nodeToFind == NULL);
}

//===============EmptyTree===============================================================//

TEST_F(EmptyTree, InsertNode)
{
    auto newNode = tree.InsertNode(10);
    EXPECT_TRUE(newNode.first);
    EXPECT_EQ(newNode.second->key, 10);
}

TEST_F(EmptyTree, DeleteNode)
{
    auto nodeToDelete = tree.DeleteNode(10);
    EXPECT_FALSE(nodeToDelete);
}

TEST_F(EmptyTree, FindNode)
{
    auto nodeToFind = tree.FindNode(10);
    EXPECT_TRUE(nodeToFind == NULL);
}

//===============TreeAxioms===============================================================//

TEST_F(TreeAxioms, Axiom1)
{
    std::vector<Node_t *> leafnodes;
    tree.GetLeafNodes(leafnodes);
    for (auto node : leafnodes)
    {
        EXPECT_TRUE(node->color == BLACK);
    }
}

TEST_F(TreeAxioms, Axiom2)
{
    std::vector<Node_t *> nodes;
    tree.GetNonLeafNodes(nodes);
    for (auto node : nodes)
    {
        if (node->color == RED)
        {
            EXPECT_TRUE(node->pLeft->color == BLACK && node->pRight->color == BLACK);
        }
    }
}

TEST_F(TreeAxioms, Axiom3)
{

    int expectedCount = -1;
    int nodesCount;

    auto root = tree.GetRoot();
    EXPECT_TRUE(root != nullptr);

    std::vector<Node_t *> leafNodes;
    tree.GetLeafNodes(leafNodes);

    for (auto node : leafNodes)
    {
        nodesCount = 0;
        while (node->pParent != root)
        {
            if (node->color == BLACK)
            {
                nodesCount++;
            }
            node = node->pParent;
        }

        if (expectedCount == -1)
        {
            expectedCount = nodesCount;
        }
        else
        {
            EXPECT_EQ(nodesCount, expectedCount);
        }
    }
}

/*** Konec souboru black_box_tests.cpp ***/
