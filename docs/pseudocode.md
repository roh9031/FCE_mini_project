# Pseudocode

Pseudocode for every operation in `src/main.cpp`, in the order of the tasks.

**Conventions.** `node.iata` is the IATA code stored in a node. The root has depth 0, and the height of a tree is the depth of its deepest node (−1 for an empty tree), so a single node has height 0 and a chain of n nodes has height n − 1.

**Recursive and iterative versions.** Insert, search, min/max and delete are written both ways. The program uses the **iterative** versions in every task, because inserting the 6,072 codes in sorted order builds a chain 6,071 nodes deep, and recursion that deep can overflow the call stack. The recursive versions are checked against the iterative ones in the "Recursive vs iterative" part of `main`.

---

## Task 1: Load the data and build the tree

### splitline (commas inside quotes do not split a field)

```
SPLITLINE(line)
    fields ← empty list
    current ← empty string
    inquotes ← false
    for i ← 0 to length(line) − 1
        if line[i] is a double quote
            inquotes ← not inquotes            // the quote itself is not kept
        else if line[i] is a comma and not inquotes
            append current to fields           // a real separator
            current ← empty string
        else
            current ← current + line[i]        // includes commas inside quotes
    append current to fields                   // the last field has no comma after it
    return fields
```

### loadairports

```
LOADAIRPORTS(filename, airports)
    open filename; if it fails, return false
    for each line in the file
        f ← SPLITLINE(line)
        if size(f) < 8 or f[4] = "\N"
            skip the line                      // no IATA code
        append Airport(f[0] as int, f[1], f[2], f[3], f[4], f[5],
                       f[6] as double, f[7] as double) to airports
    return true
```

### Build (insert in file order)

```
BUILD(airports)
    inserted ← 0; duplicates ← 0; alwaysvalid ← true
    for i ← 0 to n − 1
        if INSERT(airports[i])
            inserted ← inserted + 1
        else
            duplicates ← duplicates + 1        // rule I4: report it
            print airports[i].iata
        if not VALIDATE()
            alwaysvalid ← false                // Task 4c: validate after every insert
    print inserted, duplicates, HEIGHT(), AVERAGEDEPTH()
```

### Insert, iterative (used by the program)

```
INSERT-ITERATIVE(root, airport)
    newnode ← new Node(airport)
    if root = NULL
        return newnode, inserted = true
    curr ← root
    loop
        if airport.iata < curr.iata
            if curr.left = NULL
                curr.left ← newnode; return root, inserted = true
            curr ← curr.left
        else if airport.iata > curr.iata
            if curr.right = NULL
                curr.right ← newnode; return root, inserted = true
            curr ← curr.right
        else                                   // duplicate (rule I4)
            delete newnode
            return root, inserted = false
```

### Insert, recursive

```
INSERT-RECURSIVE(node, airport)
    if node = NULL
        return new Node(airport), inserted = true
    if airport.iata < node.iata
        node.left ← INSERT-RECURSIVE(node.left, airport)
    else if airport.iata > node.iata
        node.right ← INSERT-RECURSIVE(node.right, airport)
    else
        inserted ← false                       // duplicate (rule I4)
    return node
```

### Height and average depth

```
HEIGHT(node)
    if node = NULL
        return −1
    return 1 + max(HEIGHT(node.left), HEIGHT(node.right))

SUM-AND-COUNT(node, depth, sum, count)
    if node = NULL
        return
    sum ← sum + depth
    count ← count + 1
    SUM-AND-COUNT(node.left, depth + 1, sum, count)
    SUM-AND-COUNT(node.right, depth + 1, sum, count)

AVERAGEDEPTH(root)
    sum ← 0; count ← 0
    SUM-AND-COUNT(root, 0, sum, count)
    return sum / count                         // as a double, not integer division
```

---

## Task 2: Search

### Search with path, iterative (used by `find`)

```
SEARCH-ITERATIVE(code)
    curr ← root; path ← empty list; comparisons ← 0
    while curr ≠ NULL
        append curr.iata to path
        comparisons ← comparisons + 1
        if code = curr.iata
            return curr, path, comparisons
        else if code < curr.iata
            curr ← curr.left
        else
            curr ← curr.right
    return NULL, path, comparisons             // not found
```

### Search with path, recursive (used by `findrecursive`)

```
SEARCH-RECURSIVE(node, code, path, comparisons)
    if node = NULL
        return NULL                            // not found
    append node.iata to path
    comparisons ← comparisons + 1
    if code = node.iata
        return node
    if code < node.iata
        return SEARCH-RECURSIVE(node.left, code, path, comparisons)
    return SEARCH-RECURSIVE(node.right, code, path, comparisons)
```

### Minimum and maximum

```
MIN-ITERATIVE()                         MAX-ITERATIVE()
    curr ← root                             curr ← root
    while curr.left ≠ NULL                  while curr.right ≠ NULL
        curr ← curr.left                        curr ← curr.right
    return curr.iata                        return curr.iata

MIN-RECURSIVE(node)                     MAX-RECURSIVE(node)
    if node.left = NULL                     if node.right = NULL
        return node                             return node
    return MIN-RECURSIVE(node.left)         return MAX-RECURSIVE(node.right)
```

### Deepest node (for "a code deep in the tree")

```
DEEPEST()
    order ← LEVEL-ORDER(root)
    return the last code in order              // always on the deepest level
```

---

## Task 3: Delete

### Delete, iterative (used by `remove`)

```
DELETE-ITERATIVE(code)
    parent ← NULL; curr ← root
    while curr ≠ NULL and curr.iata ≠ code     // find the node and its parent
        parent ← curr
        if code < curr.iata
            curr ← curr.left
        else
            curr ← curr.right
    if curr = NULL
        return false                           // not found

    if curr.left = NULL and curr.right = NULL  // D1: leaf
        REPLACE-IN-PARENT(parent, curr, NULL)
        delete curr
    else if curr.left = NULL or curr.right = NULL   // D2: one child
        child ← the child that is not NULL
        REPLACE-IN-PARENT(parent, curr, child)
        delete curr
    else                                       // D3: two children
        succparent ← curr
        succ ← curr.right
        while succ.left ≠ NULL                 // inorder successor = min of right subtree
            succparent ← succ
            succ ← succ.left
        curr.data ← succ.data
        if succparent = curr
            succparent.right ← succ.right
        else
            succparent.left ← succ.right
        delete succ
    print the case and VALIDATE()
    return true

REPLACE-IN-PARENT(parent, curr, newchild)
    if parent = NULL
        root ← newchild                        // curr was the root
    else if parent.left = curr
        parent.left ← newchild
    else
        parent.right ← newchild
```

### Delete, recursive (used by `removerecursive`)

```
DELETE-RECURSIVE(node, code)                   // returns the new root of this subtree
    if node = NULL
        return NULL                            // not found
    if code < node.iata
        node.left ← DELETE-RECURSIVE(node.left, code)
        return node
    if code > node.iata
        node.right ← DELETE-RECURSIVE(node.right, code)
        return node
    if node.left = NULL and node.right = NULL  // D1: leaf
        delete node
        return NULL
    if node.right = NULL                       // D2: only a left child
        child ← node.left; delete node; return child
    if node.left = NULL                        // D2: only a right child
        child ← node.right; delete node; return child
    succ ← MIN-RECURSIVE(node.right)           // D3: two children
    node.data ← succ.data
    node.right ← DELETE-RECURSIVE(node.right, succ.iata)   // successor has no left child
    return node
```

The recursive version needs no parent pointer: each call returns the new root of its subtree, and the caller stores it in its own left or right pointer.

---

## Task 4: Traversals and validation

### Validate with the range (lo, hi) method

```
VALIDATE()
    return VALID-RANGE(root, NULL, NULL)

VALID-RANGE(node, low, high)                   // NULL bound means "no limit"
    if node = NULL
        return true
    if low ≠ NULL and node.iata ≤ low.iata
        return false
    if high ≠ NULL and node.iata ≥ high.iata
        return false
    return VALID-RANGE(node.left, low, node)   // left side: node is the new upper bound
       and VALID-RANGE(node.right, node, high) // right side: node is the new lower bound
```

### Inorder list and sorted check (Task 4a)

```
PRINT-INORDER(node)
    if node = NULL
        return
    PRINT-INORDER(node.left)
    print node.iata, node.name, node.city, node.country
    PRINT-INORDER(node.right)

CHECK-INORDER(node, prev)                      // true if every code is larger than the one before
    if node = NULL
        return true
    if not CHECK-INORDER(node.left, prev)
        return false
    if prev ≠ NULL and prev.iata ≥ node.iata
        return false
    prev ← node
    return CHECK-INORDER(node.right, prev)
```

### Level order (with a queue)

```
LEVEL-ORDER(root)
    result ← empty list
    if root = NULL
        return result
    Q ← queue containing root
    while Q is not empty
        curr ← DEQUEUE(Q)
        append curr.iata to result
        if curr.left ≠ NULL:  ENQUEUE(Q, curr.left)
        if curr.right ≠ NULL: ENQUEUE(Q, curr.right)
    return result

PRINT-LEVELS(k)                                // top k levels, one line per level
    Q ← queue containing root
    for level ← 0 to k − 1, while Q is not empty
        levelsize ← size(Q)
        for i ← 0 to levelsize − 1
            curr ← DEQUEUE(Q); print curr.iata
            enqueue curr's children that are not NULL
        print a new line
```

### Save and load in preorder (Task 4b)

```
PREORDER(node, codes)
    if node = NULL
        return
    append node.iata to codes
    PREORDER(node.left, codes)
    PREORDER(node.right, codes)

SAVE(file)
    codes ← empty list
    PREORDER(root, codes)
    write each code to file, one per line

LOAD(file, sorted)                             // sorted = airports sorted by code
    CLEAR()
    for each code in file
        record ← BINARY-SEARCH(sorted, code)
        INSERT(record)
```

Inserting the codes in preorder rebuilds exactly the same shape: every node comes before everything in its subtrees, so it is inserted before its descendants and lands in its original position.

---

## Task 5: Range query

```
RANGE(from, to)
    result ← empty list; visited ← 0
    RANGE-HELPER(root, from, to, result, visited)
    return result, visited

RANGE-HELPER(node, from, to, result, visited)
    if node = NULL
        return
    visited ← visited + 1
    if node.iata > from                        // otherwise the whole left side is below from
        RANGE-HELPER(node.left, from, to, result, visited)
    if from ≤ node.iata ≤ to
        append node.iata to result
    if node.iata < to                          // otherwise the whole right side is above to
        RANGE-HELPER(node.right, from, to, result, visited)
```

This is an inorder traversal that skips subtrees outside the range, so the results come out in alphabetical order.

---

## Task 6: Balanced build and rebalance

```
BUILD-BALANCED(sorted, lo, hi)                 // sorted by code, no duplicates
    if lo > hi
        return NULL
    mid ← floor((lo + hi) / 2)
    node ← new Node(sorted[mid])               // the middle element is the root
    node.left ← BUILD-BALANCED(sorted, lo, mid − 1)
    node.right ← BUILD-BALANCED(sorted, mid + 1, hi)
    return node

REBALANCE()
    sorted ← empty list
    INORDER-INTO(root, sorted)                 // inorder gives the airports sorted by code
    CLEAR()
    root ← BUILD-BALANCED(sorted, 0, size(sorted) − 1)
```

Each level halves the range, so the height is ⌊log₂ n⌋ = 12 for n = 6,072. The recursion is only about 13 calls deep.

### Shape experiment

```
EXPERIMENT(airports)
    queries ← 10,000 random codes from airports (fixed seed)
    sorted ← airports sorted by code
    shuffled ← airports in random order

    for each build in [file order, sorted order, balanced, random shuffle]
        T ← empty tree
        if build = balanced
            T.root ← BUILD-BALANCED(sorted, 0, n − 1)
        else
            for i ← 0 to n − 1
                T.INSERT(list for this build[i])
        start ← clock()
        for i ← 0 to 9,999
            T.CONTAINS(queries[i])             // iterative search, no printing
        time ← clock() − start
        print HEIGHT(T), AVERAGEDEPTH(T), time
```