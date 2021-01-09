package cay.gurol.mehmet;

import com.sun.org.apache.xpath.internal.SourceTree;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.NoSuchElementException;
import java.util.Queue;

/**
 * Created by Mehmet Gurol Cay on 06.04.2017.
 */

/**
 * Class for a binary search tree that stores type E objects.
 * Class extended from BinaryTree
 *
 * @author Mehmet Gurol Cay
 */
public class BinarySearchTree<E> extends BinaryTree<E> {


    public BinarySearchTree() {
        super();
    }

    /**
     * This method adds to BinaryTree according to the binarySearchTree property.
     *
     * @param item Element to be added to the tree
     */
    public void createBinarySearchTree(E item) {
        if (item instanceof Integer) {
            if (this.root == null) {
                this.root = new Node<>(item);
                return;
            }
            createBinarySearchTree(item, root);
        } else {
            throw new IllegalArgumentException((String) item);
        }
    }

    /**
     * The element to be added adds to the tree according to the size smallness of the root.
     * It works recursively.
     *
     * @param item    Element to be added to the tree
     * @param current Current node
     */
    private void createBinarySearchTree(E item, Node<E> current) {
        if ((Integer) item <= (Integer) current.data) {
            if (current.left == null)
                current.left = new Node<>(item);
            else
                createBinarySearchTree(item, current.left);
        } else {
            if (current.right == null)
                current.right = new Node<>(item);
            else
                createBinarySearchTree(item, current.right);
        }
    }

    /**
     * Iterator method generates BinarySearchTreeIterator for binary search tree
     *
     * @return BinarySearchTreeIterator
     */
    public Iterator levelOrderIterator() {
        return new BinarySearchTreeIter();
    }

    /**
     * Inner class to implement the BinarySearchTreeIterator according level order traversal
     *
     * @param <E> Generic type
     */
    private class BinarySearchTreeIter<E> implements Iterator<E> {

        private Queue<E> queue;

        /**
         * No parameter constructor is initialize the queue and
         * fills the queue according level order traversal
         */
        public BinarySearchTreeIter() {
            queue = new LinkedList();

            addToQueue((Node<E>) root);
        }

        /**
         * This method adds a node to queue according to level order.
         *
         * @param current
         */
        private void addToQueue(Node current) {
            Queue tempQueue = new LinkedList<E>();

            if (current == null)
                return;
            tempQueue.offer(current);
            while (!tempQueue.isEmpty()) {
                if (((Node) tempQueue.peek()).left != null) {
                    tempQueue.offer(((Node) tempQueue.peek()).left);
                }
                if (((Node) tempQueue.peek()).right != null) {
                    tempQueue.offer(((Node) tempQueue.peek()).right);
                }
                queue.offer((E) tempQueue.poll());
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
            return !queue.isEmpty();
        }

        /**
         * Returns the next element in the iteration.
         *
         * @return the next element in the iteration
         * @throws NoSuchElementException if the iteration has no more elements
         */
        @Override
        public E next() {
            return queue.poll();
        }
    }

    /**
     * The binary tree is added in an insignificant way by reading sequentially from the file.
     *
     * @return BinarySearchTree
     */
    public static BinarySearchTree<Integer> readBinarSearchTree() {
        BinarySearchTree<Integer> newIntegerBinarySearchTree = new BinarySearchTree<>();

        try {
            BufferedReader bf = new BufferedReader(new FileReader("test.txt"));

            String line = bf.readLine();
            while (line != null) {
                String[] tokens = line.split(" ");

                for (int i = 0; i < tokens.length; i++) {
                    newIntegerBinarySearchTree.createBinarySearchTree(Integer.parseInt(tokens[i]));
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

        return newIntegerBinarySearchTree;
    }
}
