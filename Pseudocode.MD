# Pseudocode

## Load / Build (Task 1)

```
splitline(line):
    fields <- [], current <- "", inquotes <- false
    for i = 0 to len(line)-1:
        if line[i] == '"': inquotes <- not inquotes
        else if line[i] == ',' and not inquotes: fields.append(current), current <- ""
        else: current <- current + line[i]
    fields.append(current)
    return fields

loadairports(filename, airports, skipped):
    for each line in file:
        fields <- splitline(line)
        if size(fields) < 8 or fields[4] == "\N": skipped++, continue
        airports.append(Airport(id, name, city, country, iata, icao, lat, lon))

buildtree(tree, list):
    duplicateCount <- 0
    for i = 0 to n-1:
        if not tree.insert(list[i]): duplicateCount++, print "Duplicate code"
```

## insertBSTiterative (rule I4: no duplicates)

```
insertBSTiterative(root, airport, inserted):
    newnode <- new Node(airport), inserted <- true
    if root == NULL: return newnode
    curr <- root
    loop:
        if airport.iata < curr.iata:
            if curr.left == NULL: curr.left <- newnode, return root
            curr <- curr.left
        else if airport.iata > curr.iata:
            if curr.right == NULL: curr.right <- newnode, return root
            curr <- curr.right
        else:                                   // duplicate
            inserted <- false, delete newnode, return root
```

## computeheight, sumAndCount, averagedepth

```
computeheight(node):
    if node == NULL: return -1
    return 1 + max(computeheight(node.left), computeheight(node.right))

sumAndCount(node, depth, sum, count):
    if node == NULL: return
    sum <- sum + depth, count++
    sumAndCount(node.left, depth+1, sum, count)
    sumAndCount(node.right, depth+1, sum, count)

averagedepth(root):
    sumAndCount(root, 0, sum, count)
    return (double) sum / count
```

## searchBSTiterative, findmin, findmax

```
searchBSTiterative(code, path, comparisons):
    curr <- root, path <- [], comparisons <- 0
    while curr != NULL:
        path.append(curr.iata), comparisons++
        if code == curr.iata: return curr
        if code < curr.iata: curr <- curr.left
        else: curr <- curr.right
    return NULL                        // not found

findmin: curr <- root; while curr.left != NULL: curr <- curr.left; return curr
findmax: curr <- root; while curr.right != NULL: curr <- curr.right; return curr
```

## deleteBST

```
deleteBST(code, deletecase):
    parent <- NULL, curr <- root
    while curr != NULL and curr.iata != code:
        parent <- curr
        curr <- (code < curr.iata) ? curr.left : curr.right
    if curr == NULL: return false

    if curr has no children:                    // D1
        replace curr in parent with NULL
    else if curr has exactly one child c:       // D2
        replace curr in parent with c
    else:                                       // D3
        succparent <- curr, succ <- curr.right
        while succ.left != NULL: succparent <- succ, succ <- succ.left
        curr.data <- succ.data
        if succparent == curr: succparent.right <- succ.right
        else: succparent.left <- succ.right
        delete succ
    return true

"replace curr in parent with x":
    if parent == NULL: root <- x
    else if parent.left == curr: parent.left <- x
    else: parent.right <- x
```

## validateBST (range method)

```
validateBST(node, lo, hi):           // "" means no limit
    if node == NULL: return true
    if lo != "" and node.iata <= lo: return false
    if hi != "" and node.iata >= hi: return false
    return validateBST(node.left, lo, node.iata) and validateBST(node.right, node.iata, hi)

validate(): return validateBST(root, "", "")
```

## inordertraversal, preordertraversal, savepreorder, load

```
inordertraversal(node, out):         // alphabetical
    if node == NULL: return
    inordertraversal(node.left, out); out.append(node); inordertraversal(node.right, out)

preordertraversal(node, out):
    if node == NULL: return
    out.append(node.iata); preordertraversal(node.left, out); preordertraversal(node.right, out)

savepreorder(file): write preorder codes, one per line
load(file):         clear tree; for each code in file: insert(record for code)
                    // inserting a preorder gives back exactly the same shape
```

## rangequery

```
rangequery(node, from, to, out, visited):
    if node == NULL: return
    visited++
    if node.iata > from:  rangequery(node.left, from, to, out, visited)    // else skip left
    if from <= node.iata <= to: out.append(node)
    if node.iata < to:    rangequery(node.right, from, to, out, visited)   // else skip right
```

## buildbalancedrecursive, rebalance

```
buildbalancedrecursive(sorted, lo, hi):
    if lo > hi: return NULL
    mid <- (lo + hi) / 2
    node <- new Node(sorted[mid])
    node.left  <- buildbalancedrecursive(sorted, lo, mid-1)
    node.right <- buildbalancedrecursive(sorted, mid+1, hi)
    return node

rebalance(): sorted <- inorder(); clear(); root <- buildbalancedrecursive(sorted, 0, n-1)
```
