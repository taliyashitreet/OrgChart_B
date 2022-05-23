#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
using namespace std;
namespace ariel
{
    class OrgChart
    {
        struct Node
        {
            string name;
            vector<Node *> children;
            Node *next;
            Node *next_rev;
            Node *next_pre;
            // constructor
            Node(string name):name(move(name)),next(nullptr),next_pre(nullptr),next_rev(nullptr) {}
            Node(const Node &other);                // copy constructor
            Node &operator=(const Node &other);     // copy assignment operator
            Node(Node &&other) noexcept;            // move constructor
            Node &operator=(Node &&other) noexcept; // move assignment operator
            ~Node();// non_defult destructor
        };
      
        class Iterator
        {
        private:
            Node *curr;
            string type;
            static void build_level(Node *root);
            void build_Reverse(Node *root);
            static void build_PreOrder(Node *root);
            void fix_order(Node *root);

        public:
        enum Type{leve =1,reverse=2,preorder =3 };
        Iterator(); // constructor
        ~Iterator(); // distructor
        Iterator(Node *root);
            Iterator(Type type_iter,Node *root); // constructor
            Iterator(const Iterator& other); // copy constructor
            Iterator(Iterator&& other) noexcept; // move assignment constructor
            Iterator& operator=(const Iterator& other);// copy assignment operator
            Iterator& operator=(Iterator&& other) noexcept; // move assignment operator
            string &operator*() const;
            string *operator->() const;
            Iterator &operator++();
            Iterator operator++(int);
            bool operator==(const Iterator &other);
            bool operator!=(const Iterator &other);

        };
        private:
        Node* root;
        friend ostream &operator<<(ostream &out, const OrgChart &org);
        bool recursive_sub(Node &start, string const &father, string const &child); //find father and put his child
        public:
        OrgChart(); // constructor
        ~OrgChart(); //distructor
        OrgChart(OrgChart &other); // copy constructor
        OrgChart(OrgChart &&other)noexcept; //move constructor
        OrgChart &operator=(const OrgChart &org); // copy assignment operator
        OrgChart &operator=(OrgChart &&org) noexcept; // move assignment operator  
        OrgChart &add_root(string const &name); // add root
        OrgChart &add_sub(string const &father, string const &child); // add vhild to father
        //return iterator to of specific order
        Iterator begin_level_order();
        Iterator end_level_order();
        Iterator begin_reverse_order();
        Iterator reverse_order();
        Iterator begin_preorder();
        Iterator end_preorder();
        Iterator begin(); // same as level order
        Iterator end(); // same as level order
        

    };
}