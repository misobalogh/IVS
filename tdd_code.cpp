//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     MICHAL BALOGH <xbalog06@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"

Graph::Graph()
{
    pHead = nullptr;
}

Graph::~Graph()
{
    Node *tmpNode = pHead;
    while (pHead != nullptr)
    {
        pHead = pHead->next;
        delete tmpNode;
        tmpNode = pHead;
    }
    pHead = nullptr;
}

std::vector<Node *> Graph::nodes()
{
    std::vector<Node *> nodes;

    if (pHead == nullptr)
    {
        return nodes;
    }

    Node *temp = pHead;
    while (temp != nullptr)
    {
        nodes.push_back(temp);
        temp = temp->next;
    }

    return nodes;
}

std::vector<Edge> Graph::edges() const
{
    std::vector<Edge> edges;

    Node *temp = pHead;
    while (temp != nullptr)
    {
        for (size_t neighboursId : temp->neighbours)
        {
            Edge newEdge(temp->id, neighboursId);
            bool found = false;
            for (const auto &edge : edges)
            {
                if (edge == newEdge)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                edges.push_back(newEdge);
            }
        }
        temp = temp->next;
    }

    return edges;
}

Node *Graph::addNode(size_t nodeId)
{

    Node *temp = pHead;
    while (temp != nullptr)
    {
        if (temp->id == nodeId)
        {
            return nullptr;
        }
        temp = temp->next;
    }

    Node *newNode = new Node(nodeId);
    if (pHead == nullptr)
    {
        pHead = newNode;
    }
    else
    {
        temp = pHead;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    return newNode;
}

bool Graph::addEdge(const Edge &edge)
{
    if (containsEdge(edge))
    {
        return false;
    }

    if (edge.a == edge.b)
    {
        return false;
    }

    Node *temp = pHead;
    while (temp != nullptr)
    {
        if (temp->id == edge.a)
        {
            temp->neighbours.push_back(edge.b);
            break;
        }
        temp = temp->next;
    }

    if (temp == nullptr)
    {

        Node *newNode = addNode(edge.a);
        newNode->neighbours.push_back(edge.b);
    }

    temp = pHead;
    while (temp != nullptr)
    {
        if (temp->id == edge.b)
        {
            temp->neighbours.push_back(edge.a);
            break;
        }
        temp = temp->next;
    }

    if (temp == nullptr)
    {

        Node *newNode = addNode(edge.b);
        newNode->neighbours.push_back(edge.a);
    }

    return true;
}

void Graph::addMultipleEdges(const std::vector<Edge> &edges)
{
    for (const auto &edge : edges)
    {
        addEdge(edge);
    }
}

Node *Graph::getNode(size_t nodeId)
{
    for (const auto &node : nodes())
    {
        if (node->id == nodeId)
        {
            return node;
        }
    }
    return nullptr;
}

bool Graph::containsEdge(const Edge &edge) const
{
    for (const auto &edgeToCheck : edges())
    {
        if (edge == edgeToCheck)
        {
            return true;
        }
    }
    return false;
}

void Graph::removeNode(size_t nodeId)
{
    if (nodeCount() == 0)
    {
        throw std::out_of_range("Graphs is empty.");
    }

    if (pHead->id == nodeId)
    {
        Node *clearNeigbhours = getNode(nodeId);
        for (auto const &neighbhour : clearNeigbhours->neighbours)
        {
            Edge edgeToRemove(clearNeigbhours->id, neighbhour);
            removeEdge(edgeToRemove);
        }
        Node *nodeToRemove = pHead;
        pHead = pHead->next;
        return;
    }

    Node *temp = pHead;

    while (temp->next != nullptr && temp->next->id != nodeId)
    {
        temp = temp->next;
    }

    if (temp->next == nullptr)
    {
        throw std::out_of_range("Graph does not contain the node.");
    }

    Node *clearNeigbhours = getNode(nodeId);
    for (auto const &neighbhour : clearNeigbhours->neighbours)
    {
        Edge edgeToRemove(clearNeigbhours->id, neighbhour);
        removeEdge(edgeToRemove);
    }
    Node *nodeToRemove = temp->next;
    temp->next = nodeToRemove->next;
}

void Graph::removeEdge(const Edge &edge)
{
    if (edgeCount() == 0)
    {
        throw std::out_of_range("Graph is empty.");
    }

    if (!containsEdge(edge))
    {
        throw std::out_of_range("Edge not found");
    }

    Node *nodeA = getNode(edge.a);
    Node *nodeB = getNode(edge.b);

    nodeA->neighbours.erase(std::remove(nodeA->neighbours.begin(), nodeA->neighbours.end(), edge.b), nodeA->neighbours.end());
    nodeB->neighbours.erase(std::remove(nodeB->neighbours.begin(), nodeB->neighbours.end(), edge.a), nodeB->neighbours.end());
}

size_t Graph::nodeCount() const
{

    if (pHead == nullptr)
    {
        return 0;
    }
    size_t count = 1;
    Node *temp = pHead;
    while (temp->next != nullptr)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

size_t Graph::edgeCount() const
{
    return edges().size();
}

size_t Graph::nodeDegree(size_t nodeId) const
{

    if (edges().empty())
        throw std::out_of_range("Graph is empty");

    Node *temp = pHead;
    while (temp->next != nullptr && temp->id != nodeId)
    {
        temp = temp->next;
    }
    if (temp->next == nullptr && temp->id != nodeId)
        throw std::out_of_range("Node not found");

    return temp->neighbours.size();
}

size_t Graph::graphDegree() const
{
    if (nodeCount() == 0)
        return 0;

    Node *temp = pHead;
    size_t maxGraphDegree = nodeDegree(temp->id);
    while (temp->next != nullptr)
    {
        if (temp->neighbours.size() > maxGraphDegree)
        {
            maxGraphDegree = nodeDegree(temp->id);
        }
        temp = temp->next;
    }
    return maxGraphDegree;
}

void Graph::coloring()
{
    size_t c = 1;
    Node *temp = pHead;
    if (nodeCount() == 0)
        return;

    while (temp->next != nullptr)
    {
        for (size_t neighboursId : temp->neighbours)
        {
            if (c == graphDegree() + 1)
                c = 1;
            if (getNode(neighboursId)->color == 0)
            {
                getNode(neighboursId)->color = c;
            }
            if (getNode(neighboursId)->color == temp->color)
            {
                getNode(neighboursId)->color = c++;
            }
        }
        temp = temp->next;
    }
    temp->color = c;
}

void Graph::clear()
{
    nodes().clear();
    edges().clear();
    this->~Graph();
}

/*** Konec souboru tdd_code.cpp ***/
