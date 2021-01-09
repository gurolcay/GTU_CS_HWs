package cay.gurol.mehmet;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

/**
 * Created by Mehmet Gurol Cay on 02.04.2017.
 */

/**
 * Class for a binary tree that stores type E objects.
 *
 * @author Koffman and Wolfgang edit by Mehmet Gurol Cay
 */
public class BinaryTree<E> implements Iterable<E> {

    protected class Node<E> {
        protected E data;
        protected Node<E> left;
        protected Node<E> right;

        /**
         * Construct a node with given data and no children.
         *
         * @param data The data to store in this node
         */
        public Node(E data) {
            this.data = data;
            left = null;
            right = null;
        }

        /**
         * Return a string representation of the node.
         *
         * @return A string representation of the data fields
         */
        public String toString() {
            return data.toString();
        }

    }

    /**
     * Root is root of binary tree
     */
    protected Node<E> root;

    /**
     * No parameter constructor is initialize the root as null.
     */
    public BinaryTree() {
        root = null;
    }

    /**
     * One parameter constructor is initialize the this.root by given root parameter.
     *
     * @param root this parameter passing to this.root
     */
    protected BinaryTree(Node<E> root) {
        this.root = root;
    }

    /**
     *
     * Constructs a new binary tree with data in its root,leftTree
     * as its left subtree and rightTree as its right subtree.
     * @param data Element to be added
     * @param leftTree left tree
     * @param rightTree right tree
     */
    public BinaryTree(E data, BinaryTree<E> leftTree, BinaryTree<E> rightTree) {
        root = new Node<>(data);

        if (leftTree != null)
            root.left = leftTree.root;
        else
            root.left = null;

        if (rightTree != null)
            root.right = rightTree.root;
        else
            root.right = null;
    }

    /**
     * Return the left subtree.
     *
     * @return The left subtree or null if either the root or
     * the left subtree is null
     */
    public BinaryTree<E> getLeftSubTree() {
        if (root != null && root.left != null)
            return new BinaryTree<E>(root.left);
        else
            return null;
    }

    /**
     * Return the right sub-tree
     *
     * @return the right sub-tree or
     * null if either the root or the
     * right subtree is null.
     */
    public BinaryTree<E> getRightSubTree() {
        if (root != null && root.right != null)
            return new BinaryTree<E>(root.right);
        else
            return null;
    }

    /**
     * Return the data field of the root
     *
     * @return the data field of the root
     * or null if the root is null
     */
    public E getData() {
        if (root != null)
            return root.data;
        else
            return null;
    }

    /**
     * Determine whether this tree is a leaf.
     *
     * @return true if the root has no children
     */
    public boolean isLeaf() {
        return (root.left == null && root.right == null);
    }

    /**
     * Return a string representation of the binary tree.
     *
     * @return A string representation of the binary tree fields
     */
    public String toString() {
        StringBuilder sb = new StringBuilder();
        preOrderTraverse(root, 1, sb);
        return sb.toString();

    }

    /**
     * Perform a preorder traversal.
     *
     * @param node  The local root
     * @param depth The depth
     * @param sb    The string buffer to save the output
     */
    private void preOrderTraverse(Node<E> node, int depth, StringBuilder sb) {
        for (int i = 0; i < depth; ++i)
            sb.append(" ");
        if (node == null)
            sb.append("null\n");
        else {
            sb.append(node.toString());
            sb.append("\n");
            preOrderTraverse(node.left, depth + 1, sb);
            preOrderTraverse(node.right, depth + 1, sb);
        }
    }

    /**
     * The binary tree is added in an insignificant way by reading sequentially from the file.
     *
     * @return Binary Tree
     */
    public static BinaryTree<Integer> readBinaryTree() {
        BinaryTree<Integer> newBinaryTree = new BinaryTree<Integer>();

        try {
            BufferedReader bf = new BufferedReader(new FileReader("test.txt"));
            String line = bf.readLine();
            while (line != null) {
                String[] tokens = line.split(" ");
                for (int i = 0; i < tokens.length; i++) {
                    newBinaryTree.addBinaryTree(Integer.parseInt(tokens[i]));
                }
                line = bf.readLine();
            }
            bf.close();
        } catch (FileNotFoundException e) {
            System.out.println("test.txt file does not exist!");
        } catch (IOException e) {
            System.out.println("IO Exception!");
        } catch (NumberFormatException e) {
            System.out.println("File was corrupted! Please check that the content of test.txt file is integer.");
        }

        return newBinaryTree;
    }


    /**
     * This method adds any item to binary tree
     *
     * @param item Element to add to the tree.
     * @return return value is true.
     */
    public boolean addBinaryTree(E item) {
        Node<E> newNode = new Node<>(item);
        Node<E> rootNodeRef = root;

        if (root == null) {
            root = newNode;
            return true;
        } else {
            while (true) {
                if (rootNodeRef.left == null) {
                    rootNodeRef.left = newNode;
                    return true;
                } else {
                    if (rootNodeRef.right == null) {
                        rootNodeRef.right = newNode;
                        return true;
                    } else {
                        rootNodeRef = rootNodeRef.left;
                    }
                }
            }
        }
    }

    /**
     * Method to read a binary tree.
     * pre: The input consists of a preorder traversal
     * of the binary tree. The line "null" indicates a null tree.
     *
     * @param bR The input file
     * @return The binary tree
     * @throws IOException If there is an input error
     */

    /**
     * Iterator method generates BinaryTreeIterator for binary tree
     *
     * @return BinaryTreeIterator
     */
    @Override
    public Iterator<E> iterator() {
        return new BinaryTreeIterator();
    }

    /**
     * Inner class to implement the BinaryTreeIterator according pre-order
     *
     * @param <E> Generic type
     */
    private class BinaryTreeIterator<E> implements Iterator<E> {
        Queue<Node<E>> list;

        /**
         * No parameter constructor is initialize list and fills the list according
         * to pre-order traversal
         */
        public BinaryTreeIterator() {
            list = new LinkedList<>();

            this.addToList(root);
        }

        /**
         * This private method is helper method to adds a node to list according pre-order
         *
         * @param current
         */
        private void addToList(Node current) {
            if (current == null) {
                return;
            } else {
                list.offer(current);
                addToList(current.left);
                addToList(current.right);
            }
        }

        /**
         * Returns {@code true} if the iteration has more elements.
         * (In other words, returns {@code true} if {@link #next} would
         * return an element rather than throwing an exception.)
         *
         * @return {@code true} if the iteration has more elements
         */
        @Override
        public boolean hasNext() {
            return !list.isEmpty();
        }

        /**
         * Returns the next element in the iteration.
         *
         * @return the next element in the iteration
         * @throws NoSuchElementException if the iteration has no more elements
         */
        @Override
        public E next() {
            return (E) list.poll();
        }
    }

}
