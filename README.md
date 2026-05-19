# Project Overview: Huffman compression in C
* Reference: [Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding#Compression)

# Implementation Roadmap
- [x] Implement frequency table of encoded characters
- [x] Implement linked list and common operations
- [x] Implement Queue based on linked list
- [x] Generate Huffman Tree
- [x] Visualize Tree using graphviz
- [ ] Decompression
- [ ] Extend compression and decompression to directories and handle file permissions

# Steps

* While there is more than one node in the queues:
    * Dequeue the two nodes with the lowest weight by examining the fronts of both queues.
    * Create a new internal node, with the two just-removed nodes as children (either node can be either child) and the sum of their weights as the new weight.
    * Enqueue the new node into the rear of the second queue.

## Example
    * 1 iter: sum 18
        * initial q: 3 -> 2 -> 2 -> 1 -> 1-> 1-> 1-> |1 -> 1|
        * combine_q: empty
    * 2 iter:
        * initial q: 3 -> 2 -> 2 -> 1 -> 1-> |1 -> 1|
        * combine_q: 2
    *               / \
    *               1 1
    * 3 iter:
        * initial q: 3 -> 2 -> 2 -> |1 -> 1|
        * combine_q: 2 -> 2
    *               / \  / \
    *               1 1  1 1
    * 4 iter:
        * initial q: 3 -> 2 -> 2
        * combine_q: 2 -> 2 -> 2
    *               / \  / \  / \
    *               1 1  1 1  1 1
    * 5 iter:
        * initial q: 3 -> 2
        * combine_q: 4 -> 2 -> 2
                    / \
                   2   2
    *             / \ / \
    *             1 1 1 1
    * 6 iter:
        * initial q: 3
        * combine_q: 4 -> 4 -> 2
                         / \
                        2   2
    *                  / \ / \
    *                  1 1 1 1
    * 7 iter:
        * initial q: empty
        * combine_q: 5 -> 4 -> 4
                         / \
                        2   2
    *                  / \ / \
    *                  1 1 1 1
    * 8 iter:
        * initial q: empty
        * combine_q: 8 -> 5
                         / \
                        2   2
    *                  / \ / \
    *                  1 1 1 1
