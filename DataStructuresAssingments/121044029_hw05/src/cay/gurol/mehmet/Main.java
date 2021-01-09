package cay.gurol.mehmet;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Iterator;

public class Main {

    public static void main(String[] args) {
	    BinaryTree<Integer> myBinaryTree = BinaryTree.readBinaryTree();
        Iterator myIter;

//        myBinaryTree.addBinaryTree(4);
//        myBinaryTree.addBinaryTree(2);
//        myBinaryTree.addBinaryTree(5);
//        myBinaryTree.addBinaryTree(7);
//        myBinaryTree.addBinaryTree(3);
//        myBinaryTree.addBinaryTree(1);
//        System.out.println("toString\n" + myBinaryTree.toString());

        myIter = myBinaryTree.iterator();
        System.out.println("Testing first part of first questions according pre-order traversal: ");
        while (myIter.hasNext()){
            System.out.print(" " + myIter.next());
        }

        BinarySearchTree<Integer> integerBinarySearchTree = BinarySearchTree.readBinarSearchTree();
//
//        integerBinarySearchTree.createBinarySearchTree(20);
//        integerBinarySearchTree.createBinarySearchTree(10);
//        integerBinarySearchTree.createBinarySearchTree(2);
//        integerBinarySearchTree.createBinarySearchTree(13);
//        integerBinarySearchTree.createBinarySearchTree(7);
//        integerBinarySearchTree.createBinarySearchTree(1);
//        integerBinarySearchTree.createBinarySearchTree(0);
//
        Iterator bnsIter = integerBinarySearchTree.levelOrderIterator();
        System.out.println("\nTesting second part of first questions according level order traversal: ");
        while (bnsIter.hasNext()){
            System.out.print(" " + bnsIter.next());
        }
    }
}
