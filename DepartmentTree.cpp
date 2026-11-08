// DepartmentTree.cpp
// Purpose: Implements the department hierarchy using a general tree.
// Author: Bryce Harbison

#include "DepartmentTree.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
using namespace std;

DepartmentTree::DepartmentNode::DepartmentNode(const string& departmentName, DepartmentNode* parentNode) 
{
    name = departmentName;
    parent = parentNode;
}

DepartmentTree::DepartmentTree() 
{
    root = nullptr;
}

DepartmentTree::~DepartmentTree()
{
    deleteTree(root);
}

void DepartmentTree::deleteTree(DepartmentNode* node) 
{
    if(node == nullptr) 
    {
        return;
    }

    for(size_t i = 0; i < node->children.size(); i++) 
    {
        deleteTree(node->children[i]);
    }

    delete node;
}

DepartmentTree::DepartmentNode* DepartmentTree::findNode(const string& name) const 
{
    if(root == nullptr) 
    {
        return nullptr;
    }

    vector<DepartmentNode*> nodes;
    nodes.push_back(root);

    while (!nodes.empty()) 
    {
        DepartmentNode* current = nodes.back();
        nodes.pop_back();

        if(equalsIgnoreCase(current->name, name)) 
        {
            return current;
        }

        for(size_t i = 0; i < current->children.size(); i++) 
        {
            nodes.push_back(current->children[i]);
        }
    }

    return nullptr;
}

void DepartmentTree::loadFromFile(const string& dataDir, const string& fileName) 
{
    deleteTree(root);
    root = nullptr;
  
    int accepted = 0;
    int rejected = 0;
  
    ifstream in(joinPath(dataDir, fileName).c_str());

    if(!in) 
    {
        cout << "Error: could not open " << fileName << " - treating it as empty." << endl;
    } 
    else 
    {
        string line;
        int lineNumber = 0;

        while(getline(in, line)) 
        {
            lineNumber++;
            if(lineNumber == 1 && line.size() >= 3 && line.compare(0, 3, "\xEF\xBB\xBF") == 0) 
            {
                line = line.substr(3);
            }

            if(isBlankOrComment(line)) 
            {
                continue;
            }

            vector<string> fields = splitFields(line);
            string reason = "";

            if(fields.size() != 2) 
            {
                reason = "wrong number of fields (expected 2)";
            } 
            else 
            {
                string name = trim(fields[0]);
                string parentName = trim(fields[1]);

                if(name.empty() || parentName.empty()) 
                {
                    reason = "empty department name or parent name";
                } 
                else if(equalsIgnoreCase(name, "NONE")) 
                {
                    reason = "department name cannot be NONE";
                }
                else if(name.find('|') != string::npos) 
                {
                    reason = "department name must not contain '|'";
                } 
                else if(equalsIgnoreCase(name, parentName)) 
                {
                    reason = "department cannot be its own parent";
                } 
                else if(findNode(name) != nullptr) 
                {
                    reason = "duplicate department '" + name + "'";
                } 
                else if(equalsIgnoreCase(parentName, "NONE")) 
                {
                    if(root != nullptr) 
                    {
                        reason = "multiple root departments";
                    } 
                    else
                    {
                        root = new DepartmentNode(name);
                    }
                } 
                else 
                {
                    DepartmentNode* parent = findNode(parentName);
                    if(parent == nullptr) 
                    {
                        reason = "unknown parent department '" +
                                 parentName + "'";
                    } 
                    else
                    {
                        DepartmentNode* newNode = new DepartmentNode(name, parent);
                        parent->children.push_back(newNode);
                    }
                }
            }

            if(reason.empty()) 
            {
                accepted++;
            } 
            else 
            {
                cout << "Warning: " << fileName << " line " << lineNumber << ": " << reason << endl;
                rejected++;
            }
        }
    }

    cout << fileName << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
}

bool DepartmentTree::addDepartment(const string& name, const string& parentName) 
{
    string cleanName = trim(name);
    string cleanParent = trim(parentName);

    if(cleanName.empty()) 
    {
        cout << "Invalid input: department name cannot be empty." << endl;
        return false;
    }

    if(cleanName.find('|') != string::npos) 
    {
        cout << "Invalid input: department name cannot contain '|'." << endl;
        return false;
    }

    if(findNode(cleanName) != nullptr) 
    {
        cout << "Department already exists: " << cleanName << endl;
        return false;
    }

    if(root == nullptr) 
    {
        root = new DepartmentNode(cleanName);
        cout << "Root department added: " << cleanName << endl;
        return true;
    }

    if(cleanParent.empty()) 
    {
        cout << "Invalid input: please enter a parent department." << endl;
        return false;
    }

    DepartmentNode* parent = findNode(cleanParent);

    if(parent == nullptr) 
    {
        cout << "Parent department not found: " << cleanParent << endl;
        return false;
    }

    DepartmentNode* newNode = new DepartmentNode(cleanName, parent);

    parent->children.push_back(newNode);
    cout << "Department added successfully: " << cleanName << endl;

    return true;
}

bool DepartmentTree::isEmpty() const 
{
    return root == nullptr;
}

void DepartmentTree::searchDepartment(const string& name) const 
{
    string cleanName = trim(name);
    if(cleanName.empty()) 
    { 
      cout << "Invalid input: please enter a department name." << endl;
      return;
    }

    DepartmentNode* found = findNode(cleanName);

    if(found == nullptr) 
    {
        cout << "Department not found: " << cleanName << endl;
        return;
    }

    int depth = 0;
    DepartmentNode* current = found;

    while(current->parent != nullptr) 
    {
        depth++;
        current = current->parent;
    }

    cout << "Department: " << found->name << endl;

    if(found->parent == nullptr) 
    {
        cout << "Parent: None" << endl;
    } 
    else 
    {
        cout << "Parent: " << found->parent->name << endl;
    }

    cout << "Direct children: " << found->children.size() << endl;
    cout << "Depth: " << depth << endl;
}

void DepartmentTree::displayHierarchyHelper(DepartmentNode* node, int depth) const 
{
    if(node == nullptr) 
    {
        return;
    }

    for(int i = 0; i < depth; i++) 
    {
        cout << "    ";
    }

    cout << node->name << endl;

    for(size_t i = 0; i < node->children.size(); i++) 
    {
        displayHierarchyHelper(node->children[i], depth + 1);
    }
}

void DepartmentTree::displayHierarchy() const 
{
    if(root == nullptr) 
    {
        cout << "No departments are loaded." << endl;
        return;
    }

    displayHierarchyHelper(root, 0);
}

void DepartmentTree::preorderHelper(DepartmentNode* node) const 
{
    if(node == nullptr) 
    {
        return;
    }

    cout << node->name << endl;

    for(size_t i = 0; i < node->children.size(); i++) 
    {
        preorderHelper(node->children[i]);
    }
}

void DepartmentTree::preorderTraversal() const 
{
    if(root == nullptr) 
    {
        cout << "No departments are loaded." << endl;
        return;
    }

    cout << "Preorder traversal:" << endl;
    preorderHelper(root);
}
