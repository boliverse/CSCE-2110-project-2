// DepartmentTree.h
// Purpose: Manages the department hierarchy using a general tree.
// Author: Bryce Harbison

#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H
#include <string>
#include <vector>

class DepartmentTree 
{
private:
    struct DepartmentNode 
    {
        std::string name;
        DepartmentNode* parent;
        std::vector<DepartmentNode*> children;
        DepartmentNode(const std::string& departmentName, DepartmentNode* parentNode = nullptr);
    };

    DepartmentNode* root;
    DepartmentNode* findNode(const std::string& name) const;
    void deleteTree(DepartmentNode* node);
    void displayHierarchyHelper(DepartmentNode* node, int depth) const;
    void preorderHelper(DepartmentNode* node) const;

public:
    DepartmentTree();
    ~DepartmentTree();
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;
    void loadFromFile(const std::string& dataDir, const std::string& fileName);
    bool addDepartment(const std::string& name, const std::string& parentName);
    bool isEmpty() const;
    void searchDepartment(const std::string& name) const;
    void displayHierarchy() const;
    void preorderTraversal() const;
};

#endif
