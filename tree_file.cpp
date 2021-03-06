#include <iostream>
using namespace std;

struct element
{
    int number;
    int height;
    element* left;
    element* right;
};

class Container
{
public:
    // Âèðòóàëüíûå ìåòîäû, äîëæíû áûòü ðåàëèçîâàíû âàøèì êîíòåéíåðîì
    virtual void insert(int value) = 0;
    virtual bool exists(int value) = 0;
    virtual void remove(int value) = 0;

    // È ýòîò òîæå, õîòÿ ê íåìó ïîòîì åù¸ âåðí¸ìñÿ
    virtual void print() = 0;

    // Âèðòóàëüíûé äåñòðóêòîð (ïîêà ïðîñòî ïîâåðüòå, ÷òî îí íóæåí)
    virtual ~Container() { };
};

class tree : public Container
{
public:
    void insert(int n);
    bool exists(int value);
    void remove(int n);
    void print();

protected:

    struct element* take_root();

    struct element* create_new_element(int new_number);

    //The block instruments for the following methods
    int height(element* el);
    int balance_factor(element* el);
    void check(element* el);

    //Rotates
    struct element* rightrotate(element* el_R);
    struct element* leftrotate(element* el_L);
    struct element* balance(element* el);

    //Curve insert,
    //because initially insert worked by returning a pointer
    //to the element being considered at the time,
    //which is not required in the task
    struct element* paste1(element* el, int n);

    //Instruments
    struct element* min_elem(element* el);
    struct element* check_min(element* el);

    //Curve remove for the same reason
    struct element* delete_element(element* el, int n);

    //--||--
    void print1(element* el);

    //--||--
    struct element* search1(element* el, int n);

    ~tree();
    
    struct element* root;
};

struct element* tree::take_root()
{
    return root;
}

struct element* tree::create_new_element(int new_number)
{
    struct element* elem = new struct element;
    elem->number = new_number;
    elem->left = NULL;
    elem->right = NULL;
    elem->height = 1;
    return elem;
}

int tree::height(element* el)
{
    int h;
    h = el ? el->height : 0;
    return h;
}

int tree::balance_factor(element* el)
{
    int delta;
    delta = height(el->right) - height(el->left);
    return delta;
}

void tree::check(element* el)
{
    int hl = height(el->left);
    int hr = height(el->right);
    el->height = (hl > hr ? hl : hr) + 1;
}

struct element* tree::rightrotate(element* el_R)
{
    element* el_L = el_R->left;
    el_R->left = el_L->right;
    el_L->right = el_R;
    check(el_R);
    check(el_L);
    return el_L;
}

struct element* tree::leftrotate(element* el_L)
{
    element* el_R = el_L->right;
    el_L->right = el_R->left;
    el_R->left = el_L;
    check(el_L);
    check(el_R);
    return el_R;
}

struct element* tree::balance(element* el)
{
    check(el);
    if (balance_factor(el) == 2)
    {
        if (balance_factor(el->right) < 0)
        {
            el->right = rightrotate(el->right);
        }
        struct element* el0 = leftrotate(el);
        return el0;
    }
    if (balance_factor(el) == -2)
    {
        if (balance_factor(el->left) > 0)
        {
            el->left = leftrotate(el->left);
        }
        struct element* el2 = rightrotate(el);
        return el2;
    }
    return el;
}

struct element* tree::paste1(element* el, int n)
{
    if (!el)
    {
        struct element* el0 = create_new_element(n);
        cout << "create new number = " << el0->number << endl;
        root = el0;
        return el0;
    }
    if (n < el->number)
    {
        el->left = paste1(el->left, n);
    }
    else
    {
        el->right = paste1(el->right, n);
    }
    struct element* el2 = balance(el);
    root = el2;
    return el2;
}

void tree::insert(int n)
{
    struct element* el = paste1(root, n);
};

struct element* tree::min_elem(element* el)
{
    struct element* el0;
    if (el->left)
    {
        el0 = min_elem(el->left);
    }
    else
    {
        el0 = el;
    }
    return el0;
}

struct element* tree::check_min(element* el)
{
    if (el->left == 0)
    {
        return el->right;
    }
    el->left = check_min(el->left);
    struct element* el0 = balance(el);
    return el0;
}

struct element* tree::delete_element(element* el, int n)
{
    if (!el) return NULL;
    if (n < el->number)
    {
        el->left = delete_element(el->left, n);
    }
    else if (n > el->number)
    {
        el->right = delete_element(el->right, n);
    }
    else
    {
        struct element* el_L = el->left;
        struct element* el_R = el->right;
        cout << "delete number = " << el->number << endl;
        delete el;
        if (!el_R)
        {
            return el_L;
        }
        struct element* min = min_elem(el_R);
        min->right = check_min(el_R);
        min->left = el_L;
        struct element* el0 = balance(min);
        root = el0;
        return el0;
    }
    struct element* el0 = balance(el);
    root = el0;
    return el0;
}

void tree::remove(int n)
{
    struct element* el = delete_element(root, n);
};

void tree::print1(struct element* el)
{
    if (!el->left)
    {
        cout << "Number " << el->number << endl;
    }
    else
    {
        print1(el->left);
        cout << "Number " << el->number << endl;
    }
    if (el->right)
    {
        print1(el->right);
    }
}

void tree::print()
{
    print1(root);
}

struct element* tree::search1(element* el, int n)
{
    struct element* el0;
    if (!el) return NULL;
    if (n < el->number)
    {
        el0 = search1(el->left, n);
    }
    else if (n > el->number)
    {
        el0 = search1(el->right, n);
    }
    else
    {
        el0 = el;
    }
    return el0;
}

bool tree::exists(int n)
{
    return (search1(root, n)) ? true : false;
};

tree::~tree()
{
    while (root)
    {
        root = delete_element(root, root->number);
    }
}

int main()
{
    Container* c = new tree();

    for (int i = 1; i < 10; i++)
        c->insert(i * i);

    cout << "Container after creation:" << endl;
    c->print();

    if (c->exists(25))
        cout << "Search for value 25: found" << endl;

    if (!c->exists(111))
        cout << "Search for value 111: not found" << endl;

    c->remove(25);
    cout << "Container after deletion of the element:" << endl;
    c->print();

    delete c;
    return 0;
}
