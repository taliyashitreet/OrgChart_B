#include "OrgChart.hpp"

namespace ariel
{
    OrgChart::Node::~Node()
    {
        for (Node *child : this->children)
        {
            delete child;
        }
    }
    // copy constructor
    OrgChart::Node::Node(const Node &other)
        : name(other.name), next(other.next), next_rev(other.next_rev), next_pre(other.next_pre)
    {
        std::vector<Node *> tmp;
        for (Node *child : other.children)
        {
            tmp.push_back(child);
        }
        this->children = tmp;
    }

    // copy assignment operator - deep copy & assignment
    OrgChart::Node &OrgChart::Node::operator=(const Node &other)
    {
        if (&other != this)
        {
            this->name = other.name;
            std::vector<Node *> tmp;
            for (Node *child : other.children)
            {
                tmp.push_back(child);
            }
            this->children = tmp;
            this->next = other.next;
            this->next_rev = other.next_rev;
            this->next_pre = other.next_pre;
        }
        return *this;
    }
    // move constructor - copy constructor & delete other
    OrgChart::Node::Node(Node &&other) noexcept
        : name(std::move(other.name)),
          children(std::move(other.children)),
          next(other.next),
          next_rev(other.next_rev),
          next_pre(other.next_pre) {}
    // move assignment operator - copy & assign & delete other
    OrgChart::Node &OrgChart::Node::operator=(Node &&other) noexcept
    {
        this->name = other.name;
        std::vector<Node *> v;
        for (Node *child : other.children)
        {
            v.push_back(child);
        }
        this->children = v;
        this->next = other.next;
        this->next_rev = other.next_rev;
        this->next_pre = other.next_pre;
        std::move(other.name);
        return *this;
    }

    OrgChart::Iterator::Iterator()
    {
        this->curr = nullptr;
    }
     OrgChart::Iterator::Iterator(Node *root):curr(root){
            this->type="";
     }
    OrgChart::Iterator::Iterator(Type type_iter, Node *root) :curr(root)
    {
        if (type_iter == 1)
        {
            this->type = "level";
            build_level(root);
        }
        else if (type_iter == 2)
        {
            this->type = "reverse";
            build_Reverse(root);
        }
        else if (type_iter == 3)
        {
            this->type = "preorder";
            build_PreOrder(root);
        }
    }

    OrgChart::Iterator::Iterator(Iterator &&other) noexcept
    { // rvalue
        this->curr = other.curr;
        other.curr = nullptr;
    }
    //
    OrgChart::Iterator::Iterator(const Iterator &other)
    { // lvalue
        if (this != &other)
        {
            this->curr = other.curr;
        }
    }
    // Copy Assignment Operators- create a new object from passed object
    // by copying each and every item into a new memory location.
    OrgChart::Iterator &OrgChart::Iterator::operator=(const Iterator &other)
    { // lvalue
        if (this != &other)
        {
            this->curr = other.curr;
        }
        return *this;
    }
    //  move assignment operator - delete the current object and
    // create a new object by using as much memory from passed object.
    OrgChart::Iterator &OrgChart::Iterator::operator=(Iterator &&other) noexcept
    { // rvalue
        this->curr = other.curr;
        other.curr = nullptr;
        return *this;
    }
    string *OrgChart::Iterator::operator->() const
    {
        return &(curr->name); // return the name of the current node
    }
    string &OrgChart::Iterator::operator*() const
    {
        return (curr->name); // return the pointer itself
    }
    OrgChart::Iterator OrgChart::Iterator::operator++(int)
    {
        Iterator tmp = *this;
        (*this)++;
        return tmp;
    }
    OrgChart::Iterator &OrgChart::Iterator::operator++()
    {

        if (type == "level")
        {
            curr = curr->next;
        }
        else if (type == "reverse")
        {
            curr = curr->next_rev;
        }
        else
        {
            curr = curr->next_pre;
        }
        return *this;
    }
    bool OrgChart::Iterator::operator==(const OrgChart::Iterator &other)
    {
        return this->curr == other.curr;
    }

    bool OrgChart::Iterator::operator!=(const OrgChart::Iterator &other)
    {
        return this->curr != other.curr;
    }

    void OrgChart::Iterator::build_level(Node *root)
    {
        if (root == nullptr)
        {
            throw out_of_range("This OrgChart Not initialized");
        }
        queue<Node *> q;
        q.push(root);
        Node *tmp = nullptr;
        while (!q.empty())
        {
            tmp = q.front();
            cout << tmp->name << endl;
            q.pop();
            for (Node *child : tmp->children)
            {
                q.push(child);
            }
            tmp->next = q.front();
        }
        if (tmp != nullptr)
        {
            tmp->next = nullptr;
        }
    }
    void OrgChart::Iterator::build_PreOrder(Node *root)
    {
        if (root == nullptr)
        {
            throw out_of_range("This OrgChart Not initialized");
        }
        stack<Node *> stac;
        stac.push(root);
        Node *tmp = root;
        while (!stac.empty())
        {
            tmp = stac.top();
            stac.pop();
            for (int i = (int)tmp->children.size() - 1; i >= 0; i--)
            {
                stac.push(tmp->children.at((size_t)i));
            }
            if (!stac.empty())
            {
                tmp->next_pre = stac.top();
            }
        }

        if (tmp != nullptr)
        {
            tmp->next_pre = nullptr;
        }
    }
    void OrgChart::Iterator::build_Reverse(Node *root)
    {
        if (root == nullptr)
        {
            throw runtime_error("This OrgChart Not initialized");
        }
        stack<Node *> S;
        queue<Node *> Q;
        
        // 1) Instead of printing a node, we push the node to stack
        // 2) Right subtree is visited before left subtree
        Node *tmp = nullptr;
        Q.push(root);
        while (!Q.empty())
        {
            tmp = Q.front();
            Q.pop();
            S.push(tmp);

            // Enqueue right child
            for (int i = (int)tmp->children.size() - 1; i >= 0; i--)
            {
                Q.push(tmp->children.at((size_t)i));
            }
            // pop stack one by one
            while (!S.empty() && !Q.empty())
            {
                tmp = Q.front();
                S.top()->next_rev = tmp;
                S.pop();
            }
        }
      
        if(tmp != nullptr){
            tmp->next_rev =nullptr;
        }
        fix_order(root);

    }
    void OrgChart::Iterator::fix_order(Node *root)
    {
        Node *current = root;
        Node *next_ = current->next_rev;
        current->next_rev = nullptr;
        Node *prev = current;
        current = next_;
        // cout <<current->name<< endl;
        // cout <<current->next_rev->name<< endl;
        while (current != nullptr)
        {
            // cout <<current->next_rev->name<< endl;
            next_ = current->next_rev;
            current->next_rev = prev;
            prev = current;
            current = next_;
        }
        // cout<< prev->name << endl;
        //  cout<<current->name << endl;
        this->curr = prev;
        // cout << "root: " << curr->name <<endl;
        // cout << "root next" << curr->next_rev->name << endl;
    }

    OrgChart::Iterator::~Iterator()
    {
        this->curr = nullptr;
        move(this->type);
    }
    OrgChart::~OrgChart()
    {
        delete this->root;
    }
    OrgChart::OrgChart()
    {
        this->root = nullptr;
    }
    // copy constructor - just copy
    OrgChart::OrgChart(OrgChart &other)
    {
        this->root = other.root;
    }
    // move constructor - copy constructor & delete other
    OrgChart::OrgChart(OrgChart &&other) noexcept
    {
        this->root = other.root;
        other.root = nullptr;
    }
    OrgChart &OrgChart::operator=(const OrgChart &org)
    {
        if (&org != this)
        {
            this->root = org.root;
        }

        return *this;
    }
    OrgChart &OrgChart::operator=(OrgChart &&org) noexcept
    {
        this->root = org.root;
        org.root = nullptr;
        return *this;
    }
    OrgChart &OrgChart::add_root(string const &name)
    {
        if (this->root != nullptr)
        {
            this->root->name = name;
        }
        else
        {
            this->root = new Node(name);
        }
        return *this;
    }
    OrgChart &OrgChart::add_sub(string const &father, string const &child)
    {
        if (this->root == nullptr)
        {
            throw out_of_range("OrgChart is null");
        }

        if (!recursive_sub(*(this->root), father, child))
        {
            throw runtime_error("didnt found this father");
        }

        return *this;
    }
    bool OrgChart::recursive_sub(Node &start, string const &father, string const &child)
    {
        if (start.name == father)
        {
            Node *enter = new Node(child);
            start.children.push_back(enter);
            return true;
        }
        for (unsigned int i = 0; i < start.children.size(); i++)
        {
            if (recursive_sub(*(start.children.at(i)), father, child))
            {
                return true;
            }
        }

        return false;
    }

    OrgChart::Iterator OrgChart::begin_level_order()
    {
        if (this->root == nullptr)
        {
            throw invalid_argument("orgChart is empty");
        }
        return OrgChart::Iterator(Iterator::Type(1), this->root);
    }
    OrgChart::Iterator OrgChart::end_level_order()
    {
        if (this->root == nullptr)
        {
            throw invalid_argument("orgChart is empty");
        }
        return OrgChart::Iterator(nullptr);
    }
    OrgChart::Iterator OrgChart::begin_reverse_order()
    {
        if (this->root == nullptr)
        {
            throw invalid_argument("orgChart is empty");
        }
        return OrgChart::Iterator(Iterator::Type(2), this->root);
    }
    OrgChart::Iterator OrgChart::reverse_order()
    {
        if (this->root == nullptr)
        {
            throw invalid_argument("orgChart is empty");
        }
        return OrgChart::Iterator(nullptr);
    }
    OrgChart::Iterator OrgChart::begin_preorder()
    {
        if (this->root == nullptr)
        {
            throw invalid_argument("orgChart is empty");
        }
        return OrgChart::Iterator(Iterator::Type(3), this->root);
    }
    OrgChart::Iterator OrgChart::end_preorder()
    {
        if (this->root == nullptr)
        {
            throw invalid_argument("orgChart is empty");
        }
        return OrgChart::Iterator(nullptr);
    }
    OrgChart::Iterator OrgChart::begin()
    {
        if (this->root == nullptr)
        {
            throw invalid_argument("orgChart is empty");
        }
        return begin_level_order();
    }
    OrgChart::Iterator OrgChart::end()
    {
        if (this->root == nullptr)
        {
            throw invalid_argument("orgChart is empty");
        }
        return end_level_order();
    }
    std::ostream &operator<<(std::ostream &out, const OrgChart &org)
    {
        deque<OrgChart::Node *> q;
        if (org.root == nullptr)
        {
            throw std::out_of_range("OrgChart is empty");
        }
        q.push_back(org.root);
        while (!q.empty())
        {
            size_t len = q.size();
            for (size_t i = 0; i < len; i++)
            {
                OrgChart::Node *tmp = q.front();
                out << tmp->name << "  ";
                if (!tmp->children.empty())
                {
                    for (OrgChart::Node *child : tmp->children)
                    {
                        q.push_back(child);
                    }
                }
                q.pop_front();
            }
            out << "\n";
        }

        return out;
    }

}
