
# Container

In this project, I implemented the following containers:

- vector
- map (Red-Black tree)
- stack
- set (Red-Black tree)

And:

- iterators_traits
- reverse_iterator
- enable_if
- is_integral
- equal and/or lexicographical_compare
- std::pair
- std::make_pair

## Iterators

I started creating a struct **Iterators** and 5 tags.
In this structure we can find:

- Category, which will be a tag
- T, the type of data dereferenced
- Distance, a value used to calculate distance between Iterators
- Pointer
- Reference

A tag can be:

- input_iterator_tag
- output_iterator_tag
- forward_iterator_tag
- bidirectional_iterator_tag
- random_access_iterator_tag
According to the tag, there will be inheritance (forward inherit from input, for example)

I, then, defined the iterator_traits, fundamental values used by the STD algos.

The operators are not hard, actually we need only one or two of them and we can exploit them to define the other (ex: if we defined **>**, **<=** will be just his negation).

### Reverse Iterator

Something we need to do for the Reverse Iterator is reverse the iterator sense. By defining all the operators, we simply swap how **+** and **-** works.

#### Bidirectional Iterator

We need to define the + and - operators in all their forms, with the correct iteration (so, not reversed like the reverse!), other than basic operators like * and **->**

#### Random Access Iterator

I just redefined the operators, to avoid any possible conflict. It's not too different from the bidirectional (just for the sake of **this** implementation, they are different in theory!)

## Vector

This implementation of the class Vector is based on C98, so I just look what attribute it had at that time. The four key values of the Vector will be:

- _alloc, the std::Allocator
- _start, the first element of the Vector
- _end, the last element of the Vector
- _end_capacity, the last allocated elements (which, often, will be different that_end to avoid resize, improving performance)

### Constructors

The fill constructor is just a constructor that allocate **n** space and, then, fill them all with a value **val**.

The range constructor works similiar to the previous one, but with one more step: to fill with an ordered range we start from the beginning, we insert **value** and then we increment it.

The copy constructor will just copy all the elements of a vector x in our vector, using two pointers (one for each vector).

The '=' operator will just clear the entire vector and then insert the new one.

Important: to destroy a vector, we need to clear and deallocate memory!

#### Size

**Size()** will return the current elements *(_end - _start)* while max_size the theorical maximum size of *_alloc*.

**Resize()** have two different prototypes but they works in the same way: if we are shrinking the Vector, we destroy everything after size or, if smaller, we reserve and then construct the remaining spaces (empty for the basic resize, initializated to *val* for the *resize and fill*).

**Capacity()** will return the possible capacity of the Vector, **empty()** if the Vector is empty.

**Reserve()**, for a valid size, will construct a new Vector, copy the previous elements and then destroy the old ones, deallocating the spaces.

**Assign()** will replaces the content with *n* copies of *values* or with elements in range [*first*, *last*). The only difference in the way they works is that, for the range one, we need to manually calculate the *n* size.

**Push_Back()** calculate the first available space, and construct an element initializated to *val* there.

**Pop_Back()** will simply destroy the last allocated element.

**Insert()** arguably the most difficult implementation, for me. In the easy case, where there is already enough capacity, we just shift by *n* the previous elements and then fill the now free space with *value*. For the other case instead I store the previous attributes in *tmp* variables, then construct a new Vector while, at the same time, destroying the old elements and deallocate what need to be deallocated.

**Erase()** is a function that delete the element at *pos* or inside a range *first, last*. It's important to shift all the elements to preserve the integrity.

**Swap()** will just swap two Vector. Just like a normal swap, I used *tmp*s variables.

**Operators** just like Iterators, it's possible to create just one or two of them and use them for every operators. To do that, I used *equal* and *lexicographically_compare()*.

## Utilities

Before moving to the others Container, I made all the requested utilities, already used by Vector himself.

**Equal()** returns *true* if two vectors are equals, or if the two vectors are equal and satisfy the *Binary Predicate*

**Lexicographically Compare()** checks if the second Vector is *less*, or *comp* than the first one.

**Enable_if {}** Attributes a *T* value and a *Bool* which will be *True* if T is integral

**Pair** A simple structure that take two elements of *template* type. The operators are, like usual, easy to do after the first one.

## Stack

With a *Vector* well-made and polished, the implementation of *Stack* was easy.

I create a *Class Stack* which inherit from Vector; all the peculiar function of *Stack*, then, just call the corrispondent of *Vector* (*top* return *back*, *push* returns *push_back* and so on).

### Operators

Being non members, to make the operators work I used the keyword *friend*. That way, the operators can access the values even if they are not members.

## Red Black Tree

To facilitate the implementation of map as a red black tree, I choose to create an hpp to use for both **Set** and **Map**.

### Node

I started with a struct *_tree_node*, that contains informations about the node itself: his color, his parent, his two child and his value.

**is_left_node()** return if the node is left, by return himself equal to the left node of his parent

**child_is_red()** return if the child exist and is red

#### Identification Functions

**Uncle()** gives a pointer to the uncle, sibling of the parent

**Sibling()** gives a pointer to the sibling

#### Functions

**Move_Down()** move a node down, and put a new one in his old location.

**Swap()** A normal swap with some more steps because, well, it's a tree node.

### Iterator

I used a syntax as similiar as possible as the the other iterator. The *++* and *--* works by using *Increment()* and *Decrement()*, defined in the Tree itself.

## Tree Class

The tree itself contains attributes head (a node, parent of root, that has left and right equal to the two extreme), total node, compare key and node alloc; all quite explicit in their uses. To make easier working with it, I typedef various terms to keep a syntax similiar to the others container and to avoid calling all classes & subclasses origin. Important functions are all the end and begin (foundamental for the implementations of Map and Set), *True Key* with all the overloads (to return precisely the *key_type*, always), *True End* (the *Head* node) and *True Root* (the parent of Head, also the 'start'of the Tree).

### Tree Functions

**Increment()** To increment a Node, we check if the node->right does not points to itself. If it does, return the node itself. If there's something at his right (so different than 0) I move the iterator there, then to the left untill possible.

**Decrement()** Simiarly to increment, if the node->right points to itself I return the node itself. If the color is red and the grandparent of node is equal to himself (dummy node), move to right. Now, like increment, if there's something to the left the iterator moves there, then to right untill possible. If not, move to the parent and then to the left untill possible.

**Most Left()** That function works in a simple way. Starting from root, we go the left node until we reached a leaf.

**Most Right()** Works like the previous one, but moves at right.

**Insert Tree Node()** The insertion takes a node root and a new node. In an empty tree, it will just return the new_node. In an already established tree it will place, using recursion, the node at the right place.

**Rotations** Rotation are a tricky way to preserve the red-black tree properties when Inserting or deleting would invalidate them. When rotating left, for example, a node X, his right child become his parent. This node's left child become his right child. Without a visual support that sounds conceptually a lot harder that it actually is, while it's just a bunch of lines of code.

**No Red Red Nodes** One of the biggest challenge in this project. There's actually a possibility to have two red node linked. That invalidates the properties of a Red Black Tree, so it's something that need a fix. The fix is obviously different if the nodes are left or right between them, differing in the nodes we work with (left or right) and the eventual rotation needed (right or left). If parent, child and uncle are all red it's possible to just recolor (red grand parent, so his two children black and the node red). If uncle is black it's actually requested a rotation, left if node is right child of parent or right if it's left.

**No Black Black** Like the *No Red Red* there's actually a lot of different cases to manage. If there's no sibling, I fix the black black in the parent node. If there's a sibling and he's red, it's possible to recolors node and rotate (left if node is right, right if node is left). If the sibling is black and the child is red there are four possible cases: left left, right left, right left and right right. According to case, rotation are required. Explaining by text is actually a lot more difficult than it really is, so I advise to look for a visual tutorial (youtube, text and images ...) or directly the code.

**Reinitialize** Set head to red, left and right to head, parent and total node to 0. Basically, reinitialize the entire tree.

**Node Move** An ausiliary to swap. Swap a tree.

**Clear** Erase the true root and reinitialize the tree. Used for Map::Clear.

**No left child** Return a node with no left child. Useful for Node Replacing.

**Replace Node** Find the node that will replace a deleted one. In a two children situation, it's the no left child of the right one. For a leaf it's *NULL* and, otherwise (so 1 child), is just node->left or node->right.

**Node Deleter** I created an *helper* variable, that will replace the deleted *Node*. If both are black, I store that information in a boolean. If there's no helper, Node is a leaf: if the both black boolean is true, I fix the black black with the apposite funcion, else if it does have sibling it must be red. I invalidate then the pointer to him (so his parent left or right) and use *Node Putter*, then return. If the node has s child, I change the actual pointer from the *Node* to the *Helper* and then destroy the delete one. If both black, there may be needed a *No_Black_Black*.

**Node Getter** Increment the *Total Node* by 1, then return the node allocated.

**Node Constructor** Try to create a *Node*, construct it and, if catch an exception, destroy it.

**Node Creator** Calls *Node Getter* and *Node Constructor*, return the node.

**Node Destroyer** The node destroyer. Reduce by 1 the total node, declare a value allocator and then destroy and deallocate both the node and node->value_type.

**Node Cloner** Create a node, initialize color and value to the argument one, then return it.

**Node Copier** Only used for copy constructor and equal operator; it refresh the *True Root* and, recursevely, all the nodes.

**_Erase** If the node given as argument is valid, erase *x->* right, destroy x and move the argument to *x->left*. This erase will erase basically all the tree and is used by Copy Constructor and Destroyer.

**Bounds** Lower bound return the first element not less (default) than the given. Upper bound return the first element greater (default) than the given one.

**Insert** Insert a node with the value given. It creates a Node, insert it, fix colors if needed and then refresh the *True Root*. It returns the node itself. The Boolean indicates if the insertion took place.

**Erase** Search for a key and, if found, use *Node Deleter*. If there's a size, refresh true root, left and right. Otherwise, heads links point on itself and true root on *NULL*.

**Find** Find search for a Node with the given value. To do that, it utilise the lower bound, then return true if it's equal to the value it was looking for.

**Equal Range** Returns a range containing all elements with the given key in the container. The range is defined by two iterators, one pointing to the first element that is not less than key and another pointing to the first element greater than key.

## Map

I declared a Value Compare class, hat inherit from std::binary_function. That std class is a base class that take two arguments and a bool in his template.

### Constructor

For constructor, I recreated the two empty constructor and the range constructor, which utilises insert. It's important to use *enable_if* in the template, so I can call the range contructor only if we are storing integrals.

### Basic members

The functions related to iterators and size are basically identical to the corrispondent one in the previous Container.

### Access member

**At()** Using *Lower Bound*, return the element we are looking for or an error, if he's not in the map.

**Operator[]** The operator return the value of the elements we are looking for. If it doesn't exist, its created.

**Insert()** The first insert a single value, the second a range (only if I'm working with Integrals!)

**Lookups** All the lookups functions are just based on their corrispondent one in the Red Black Tree.

#### Comparators

The only problem with Comparators is that they are not member, so they cannot access private or protected data in the Class.

I used *Friend* to make two operator, = and <, able to access the class Map. By defining them, I extract the other four (!= it's just *not**= , > is equal to < with the two elements swapped and so on).

### Set

The implementation is, almost, an exact copy of **Map**. The biggest difference, talking about this implementation, is that the Template takes just a *Key* instead of *Key* and *Value*. That causes some differences in templates. Another difference is that *Set* does not have the element access *At* and the operator *[]*. Do not forget that Set elements are always Constant.
