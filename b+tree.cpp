#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
int MAX = 4;

class Node
{
    bool IS_LEAF;
    string *key;
    int size;
    Node **ptr;
    friend class BPTree;
public:
    Node();
};

class BPTree
{
    Node *root;
    void InsertInternal(string, Node *, Node *);
    void RemoveInternal(string, Node *, Node *);
    Node *findParent(Node *, Node *);

public:
    BPTree();
    void Search(string, string);
    void Insert(string);
    void PrintTree(Node *);
    int Remove(string);
    int height(Node *);
    void printCurrentLevel(Node *, int);
    void DumpStatistics();
    Node *getRoot();
    double total_nodes;
    double total_data;
    double total_index;
};

Node::Node()
{
    key = new string[MAX];
    ptr = new Node *[MAX + 1];
}

BPTree::BPTree()
{
    root = NULL;
    total_data=0;
    total_index=0;
    total_nodes=0;
}

// Search operation
void BPTree::Search(string x, string y)
{
    if (root == NULL)
    {
        cout << "Tree is empty\n";
    }
    else
    {
        Node *cursor = root;
        int nums = 0;
        while (cursor->IS_LEAF == false)
        {
            for (int i = 0; i < cursor->size; i++)
            {
                if (x < cursor->key[i])
                {
                    cursor = cursor->ptr[i];
                    break;
                }
                if (i == cursor->size - 1)
                {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }
        while (cursor != NULL)
        {
            for (int i = 0; i < cursor->size; i++)
            {
                if (cursor->key[i] >= x && cursor->key[i] <= y)
                {
                    cout << cursor->key[i] << "\n";
                    nums++;
                }
            }
            cursor = cursor->ptr[MAX - 1];
        }
        if (nums == 0)
            cout << "Not found\n";
    }
}

void BPTree::Insert(string x)
{
    if (root == NULL)
    {
        root = new Node;
        root->key[0] = x;
        root->IS_LEAF = true;
        root->size = 1;
        total_data++;
        total_nodes++;
        total_index++;
    }
    else
    {
        total_data++;
        Node *cursor = root;
        Node *parent;
        while (cursor->IS_LEAF == false)
        {
            parent = cursor;
            for (int i = 0; i < cursor->size; i++)
            {
                if (x < cursor->key[i])
                {
                    cursor = cursor->ptr[i];
                    break;
                }
                if (i == cursor->size - 1)
                {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }
        if (cursor->size < MAX)
        {
            // total_index++;
            int i = 0;
            while (x > cursor->key[i] && i < cursor->size)
                i++;
            for (int j = cursor->size; j > i; j--)
            {
                cursor->key[j] = cursor->key[j - 1];
            }
            cursor->key[i] = x;
            cursor->size++;
            cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = NULL;
            total_index++;
        }
        else
        {
            total_nodes++;
            Node *newLeaf = new Node;
            string virtualNode[MAX + 1];
            for (int i = 0; i < MAX; i++)
            {
                virtualNode[i] = cursor->key[i];
            }
            int i = 0, j;
            while (x > virtualNode[i] && i < MAX)
                i++;
            for (int j = MAX + 1; j > i; j--)
            {
                virtualNode[j] = virtualNode[j - 1];
            }
            virtualNode[i] = x;
            newLeaf->IS_LEAF = true;
            cursor->size = (MAX + 1) / 2;
            newLeaf->size = MAX + 1 - (MAX + 1) / 2;
            cursor->ptr[cursor->size] = newLeaf;
            newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
            cursor->ptr[MAX] = NULL;
            for (i = 0; i < cursor->size; i++)
            {
                cursor->key[i] = virtualNode[i];
            }
            for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++)
            {
                newLeaf->key[i] = virtualNode[j];
                total_index++;
            }
            if (cursor == root)
            {
                Node *newRoot = new Node;
                newRoot->key[0] = newLeaf->key[0];
                newRoot->ptr[0] = cursor;
                newRoot->ptr[1] = newLeaf;
                newRoot->IS_LEAF = false;
                newRoot->size = 1;
                root = newRoot;
                total_index++;
                total_nodes++;
            }
            else
            {
                InsertInternal(newLeaf->key[0], parent, newLeaf);
            }
        }
    }
}

void BPTree::InsertInternal(string x, Node *cursor, Node *child)
{
    if (cursor->size < MAX)
    {
        int i = 0;
        while (x > cursor->key[i] && i < cursor->size)
            i++;
        for (int j = cursor->size; j > i; j--)
        {
            cursor->key[j] = cursor->key[j - 1];
        }
        for (int j = cursor->size + 1; j > i + 1; j--)
        {
            cursor->ptr[j] = cursor->ptr[j - 1];
        }
        cursor->key[i] = x;
        cursor->size++;
        cursor->ptr[i + 1] = child;
        total_index++;
    }
    else
    {
        Node *newInternal = new Node;
        string virtualKey[MAX + 1];
        Node *virtualPtr[MAX + 2];
        for (int i = 0; i < MAX; i++)
        {
            virtualKey[i] = cursor->key[i];
        }
        for (int i = 0; i < MAX + 1; i++)
        {
            virtualPtr[i] = cursor->ptr[i];
        }
        int i = 0, j;
        while (x > virtualKey[i] && i < MAX)
            i++;
        for (int j = MAX + 1; j > i; j--)
        {
            virtualKey[j] = virtualKey[j - 1];
        }
        virtualKey[i] = x;
        for (int j = MAX + 2; j > i + 1; j--)
        {
            virtualPtr[j] = virtualPtr[j - 1];
        }
        virtualPtr[i + 1] = child;
        newInternal->IS_LEAF = false;
        cursor->size = (MAX + 1) / 2;
        newInternal->size = MAX - (MAX + 1) / 2;
        for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
        {
            newInternal->key[i] = virtualKey[j];
        }
        for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
        {
            newInternal->ptr[i] = virtualPtr[j];
        }
        if (cursor == root)
        {
            Node *newRoot = new Node;
            newRoot->key[0] = cursor->key[cursor->size];
            newRoot->ptr[0] = cursor;
            newRoot->ptr[1] = newInternal;
            newRoot->IS_LEAF = false;
            newRoot->size = 1;
            root = newRoot;
        }
        else
        {
            InsertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
        }
    }
}

Node *BPTree::findParent(Node *cursor, Node *child)
{
    Node *parent;
    if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF)
    {
        return NULL;
    }
    for (int i = 0; i < cursor->size + 1; i++)
    {
        if (cursor->ptr[i] == child)
        {
            parent = cursor;
            return parent;
        }
        else
        {
            parent = findParent(cursor->ptr[i], child);
            if (parent != NULL)
                return parent;
        }
    }
    return parent;
}

void BPTree::PrintTree(Node *root)
{
    int h = height(root);
    int i;
    for (i = 1; i <= h; i++)
    {
        for (int j = 0; j < h - i; j++)
            cout << "\t";
        printCurrentLevel(root, i);
        cout << "\n\n";
    }
}

int BPTree::Remove(string x)
{
    if (root == NULL)
    {
        cout << "Tree empty\n";
    }
    else
    {
        Node *cursor = root;
        Node *parent;
        int leftSibling, rightSibling;
        while (cursor->IS_LEAF == false)
        {
            for (int i = 0; i < cursor->size; i++)
            {
                parent = cursor;
                leftSibling = i - 1;
                rightSibling = i + 1;
                if (x < cursor->key[i])
                {
                    cursor = cursor->ptr[i];
                    break;
                }
                if (i == cursor->size - 1)
                {
                    leftSibling = i;
                    rightSibling = i + 2;
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }
        bool found = false;
        int pos;
        for (pos = 0; pos < cursor->size; pos++)
        {
            if (cursor->key[pos] == x)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            cout << "Not found\n";
            return 0;
        }
        for (int i = pos; i < cursor->size; i++)
        {
            cursor->key[i] = cursor->key[i + 1];
        }
        cursor->size--;
        if (cursor == root)
        {
            for (int i = 0; i < MAX + 1; i++)
            {
                cursor->ptr[i] = NULL;
            }
            if (cursor->size == 0)
            {
                cout << "Tree died\n";
                delete[] cursor->key;
                delete[] cursor->ptr;
                delete cursor;
                root = NULL;
            }
            return 1;
        }
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
        cursor->ptr[cursor->size + 1] = NULL;
        if (cursor->size >= (MAX + 1) / 2)
        {
            return 1;
        }
        if (leftSibling >= 0)
        {
            Node *leftNode = parent->ptr[leftSibling];
            if (leftNode->size >= (MAX + 1) / 2 + 1)
            {
                for (int i = cursor->size; i > 0; i--)
                {
                    cursor->key[i] = cursor->key[i - 1];
                }
                cursor->size++;
                cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
                cursor->ptr[cursor->size - 1] = NULL;
                cursor->key[0] = leftNode->key[leftNode->size - 1];
                leftNode->size--;
                leftNode->ptr[leftNode->size] = cursor;
                leftNode->ptr[leftNode->size + 1] = NULL;
                parent->key[leftSibling] = cursor->key[0];
                return 1;
            }
        }
        if (rightSibling <= parent->size)
        {
            Node *rightNode = parent->ptr[rightSibling];
            if (rightNode->size >= (MAX + 1) / 2 + 1)
            {
                cursor->size++;
                cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
                cursor->ptr[cursor->size - 1] = NULL;
                cursor->key[cursor->size - 1] = rightNode->key[0];
                rightNode->size--;
                rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
                rightNode->ptr[rightNode->size + 1] = NULL;
                for (int i = 0; i < rightNode->size; i++)
                {
                    rightNode->key[i] = rightNode->key[i + 1];
                }
                parent->key[rightSibling - 1] = rightNode->key[0];
                return 1;
            }
        }
        if (leftSibling >= 0)
        {
            Node *leftNode = parent->ptr[leftSibling];
            for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++)
            {
                leftNode->key[i] = cursor->key[j];
            }
            leftNode->ptr[leftNode->size] = NULL;
            leftNode->size += cursor->size;
            leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];
            RemoveInternal(parent->key[leftSibling], parent, cursor);
            delete[] cursor->key;
            delete[] cursor->ptr;
            delete cursor;
        }
        else if (rightSibling <= parent->size)
        {
            Node *rightNode = parent->ptr[rightSibling];
            for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++)
            {
                cursor->key[i] = rightNode->key[j];
            }
            cursor->ptr[cursor->size] = NULL;
            cursor->size += rightNode->size;
            cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
            cout << "Merging two leaf nodes\n";
            RemoveInternal(parent->key[rightSibling - 1], parent, rightNode);
            delete[] rightNode->key;
            delete[] rightNode->ptr;
            delete rightNode;
            total_nodes--;
        }
    }
    return 1;
}
void BPTree::RemoveInternal(string x, Node *cursor, Node *child)
{
    if (cursor == root)
    {
        if (cursor->size == 1)
        {
            if (cursor->ptr[1] == child)
            {
                delete[] child->key;
                delete[] child->ptr;
                delete child;
                root = cursor->ptr[0];
                delete[] cursor->key;
                delete[] cursor->ptr;
                delete cursor;
                cout << "Changed root node\n";
                return;
            }
            else if (cursor->ptr[0] == child)
            {
                delete[] child->key;
                delete[] child->ptr;
                delete child;
                root = cursor->ptr[1];
                delete[] cursor->key;
                delete[] cursor->ptr;
                delete cursor;
                cout << "Changed root node\n";
                return;
            }
        }
    }
    int pos;
    for (pos = 0; pos < cursor->size; pos++)
    {
        if (cursor->key[pos] == x)
        {
            break;
        }
    }
    for (int i = pos; i < cursor->size; i++)
    {
        cursor->key[i] = cursor->key[i + 1];
    }
    for (pos = 0; pos < cursor->size + 1; pos++)
    {
        if (cursor->ptr[pos] == child)
        {
            break;
        }
    }
    for (int i = pos; i < cursor->size + 1; i++)
    {
        cursor->ptr[i] = cursor->ptr[i + 1];
    }
    cursor->size--;
    if (cursor->size >= (MAX + 1) / 2 - 1)
    {
        return;
    }
    if (cursor == root)
        return;
    Node *parent = findParent(root, cursor);
    int leftSibling, rightSibling;
    for (pos = 0; pos < parent->size + 1; pos++)
    {
        if (parent->ptr[pos] == cursor)
        {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }
    if (leftSibling >= 0)
    {
        Node *leftNode = parent->ptr[leftSibling];
        if (leftNode->size >= (MAX + 1) / 2)
        {
            for (int i = cursor->size; i > 0; i--)
            {
                cursor->key[i] = cursor->key[i - 1];
            }
            cursor->key[0] = parent->key[leftSibling];
            parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
            for (int i = cursor->size + 1; i > 0; i--)
            {
                cursor->ptr[i] = cursor->ptr[i - 1];
            }
            cursor->ptr[0] = leftNode->ptr[leftNode->size];
            cursor->size++;
            leftNode->size--;
            return;
        }
    }
    if (rightSibling <= parent->size)
    {
        Node *rightNode = parent->ptr[rightSibling];
        if (rightNode->size >= (MAX + 1) / 2)
        {
            cursor->key[cursor->size] = parent->key[pos];
            parent->key[pos] = rightNode->key[0];
            for (int i = 0; i < rightNode->size - 1; i++)
            {
                rightNode->key[i] = rightNode->key[i + 1];
            }
            cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
            for (int i = 0; i < rightNode->size; ++i)
            {
                rightNode->ptr[i] = rightNode->ptr[i + 1];
            }
            cursor->size++;
            rightNode->size--;
            return;
        }
    }
    if (leftSibling >= 0)
    {
        Node *leftNode = parent->ptr[leftSibling];
        leftNode->key[leftNode->size] = parent->key[leftSibling];
        for (int i = leftNode->size + 1, j = 0; j < cursor->size; j++)
        {
            leftNode->key[i] = cursor->key[j];
        }
        for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; j++)
        {
            leftNode->ptr[i] = cursor->ptr[j];
            cursor->ptr[j] = NULL;
        }
        leftNode->size += cursor->size + 1;
        cursor->size = 0;
        RemoveInternal(parent->key[leftSibling], parent, cursor);
    }
    else if (rightSibling <= parent->size)
    {
        Node *rightNode = parent->ptr[rightSibling];
        cursor->key[cursor->size] = parent->key[rightSibling - 1];
        for (int i = cursor->size + 1, j = 0; j < rightNode->size; j++)
        {
            cursor->key[i] = rightNode->key[j];
        }
        for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; j++)
        {
            cursor->ptr[i] = rightNode->ptr[j];
            rightNode->ptr[j] = NULL;
        }
        cursor->size += rightNode->size + 1;
        rightNode->size = 0;
        RemoveInternal(parent->key[rightSibling - 1], parent, rightNode);
    }
}

void BPTree::printCurrentLevel(Node *cursor, int level)
{
    if (cursor == NULL)
        return;
    if (level == 1)
    {
        cout << "| ";
        for (int i = 0; i < cursor->size; i++)
        {
            cout << cursor->key[i] << " | ";
        }
        if (cursor->IS_LEAF)
            cout << " -> ";
        else
            cout << "\t\t";
    }
    else if (level > 1)
    {
        for (int i = 0; i < cursor->size + 1; i++)
        {
            printCurrentLevel(cursor->ptr[i], level - 1);
        }
    }
}

int BPTree::height(Node *cursor)
{
    if (cursor == NULL)
        return 0;
    else
    {
        int l = 0;
        if (cursor->IS_LEAF != true)
        {
            for (int i = 0; i < cursor->size + 1; i++)
            {
                l = max(l, height(cursor->ptr[i]));
            }
        }
        return (l + 1);
    }
}

void BPTree::DumpStatistics()
{
    cout << endl << "Statistics of the B+-tree:" << endl;
    cout << "Total number of nodes: " << total_nodes << endl;
    cout << "Total number of data entries: " << total_data << endl;
    cout << "Total number of index entries: " << total_index << endl;
    cout << "Average fill factor: " << (total_index / (total_nodes * MAX)) * 100 << "%" << endl;
    cout << "Height of tree: " << height(root) << endl;
}

// Get the root
Node *BPTree::getRoot()
{
    return root;
}

int main(int argc, char *argv[])
{
    if (strcmp(argv[1], "-help") == 0)
        cout << "Usage: btree [fname]\nfname: the name of the data file storing the search key values" << endl;
    else
    {
        cout << "Building an initial B+-tree... Launching the B+-tree test program... \nWaiting for your commands:" << endl;
        BPTree node;
        string str;
        ifstream MyReadFile("data.txt");
        while (getline(MyReadFile, str))
            if (str.length() <= 10)
                node.Insert(str);
        MyReadFile.close();

        string input;
        while (true)
        {
            getline(cin, input);
            istringstream ss(input);
            string operation, val, end;
            ss >> operation;
            ss >> val;
            ss >> end;
            if (operation == "insert")
            {
                node.Insert(val);
                cout << "The key " << val << " has been inserted in the B+-tree!" << endl;
            }
            else if (operation == "delete")
            {
                int res = node.Remove(val);
                if (res)
                    cout << "The key " << val << " has been deleted in the B+-tree." << endl;
            }
            else if (operation == "search")
            {
                node.Search(val, end);
            }
            else if (operation == "print")
                node.PrintTree(node.getRoot());
            else if (operation == "stats")
                node.DumpStatistics();
            else if (operation == "quit")
                break;
        };
        cout << "The program is terminated.";
    }
}