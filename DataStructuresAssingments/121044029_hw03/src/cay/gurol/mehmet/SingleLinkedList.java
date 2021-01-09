package cay.gurol.mehmet;

import java.util.*;

/**
 * Single Linled List
 * <p>
 * Ders kitabından alıntı yapılmıştır.
 */

public class SingleLinkedList<E> {

    private Node<E> head = null;
    private int size = 0;

    private Node<E> removedNode = null;

    /**
     *
     * @return This method is returned single linked list size
     */
    public int getSize() {
        return size;
    }

    /**
     *
     * @param index
     * @return
     */
    public E get(int index) {
        Node<E> temp = head;
        int i = 0;
        while(temp != null){
            if(i == index)
                return temp.data;
            temp = temp.next;
            ++i;
        }
        return null;
    }

    /**
     * Default single linked list toString() method
     *
     * @return  This method return string which is contains linked list element
     */
    @Override
    public String toString() {
        String str = new String();
        Node<E> nextItem = head;

        while (nextItem != null) {
            str += nextItem.data + ",";
            nextItem = nextItem.next;
        }

        return str;
    }

    /**
     * Single Linked List iterator
     * @return
     */
    public Iterator<E> iterator() {
        return new SingleLinkedListIterator(0);
    }

    /**
     * addFirst method take an object and add to head of the single linked list
     * if head is null method create a new node, if head isn't null method add to head
     * @param obj   this object is added to single linked list
     */
    public void addFirst(E obj) {
        if (head == null) {
            head = new Node<E>(obj);
        } else {
            Node<E> temp = head;
            head = new Node<E>(obj);
            head.next = temp;
        }
        ++size;
    }

    /**
     *addLast method take an object and add to tail of the single linked list
     * if head is null method create new node, if head isn't null method add to tail
     * @param obj this object is added to single linked list
     */
    public void addLast(E obj) {
        Node<E> temp = head;
        if(head == null){
            addFirst(obj);
        }else{
            while (temp.next != null) {
                temp = temp.next;
            }
            temp.next = new Node<E>(obj);
            ++size;
        }

    }

    /**
     * This method puts the single linked list elements in reverse order
     * @return resersed single linked list string
     */
    public String reverseToString() {
        String reversedString = "";

        for (int i = getSize() - 1; 0 <= i; i--) {
            reversedString += get(i).toString() + ",";
        }
        return reversedString;
    }

    /**
     * Put deleted node elements into string
     * @return deleted node elements
     */
    public String printRemovedNode() {
        Node<E> temp = null;
        String result = "";

        while (removedNode != null) {
            temp = removedNode;
            removedNode = removedNode.next;
            result += temp.data + ",";
        }
        return result;
    }

    /**
     * It searches the given object in a single linked list and deletes it when it finds it.
     * The deleted node is added to the deleted nodes at the same time.
     * @param obj It will be deleted from the single linked list.
     * @return Deleted item.
     */
    public Node<E> remove (E obj) {
        Node<E> temp;
        if (head.data.equals(obj)){
            temp = head;
            head = head.next;
            --size;
            addRemovedNode(temp);
            return temp;
        } else {
            Node<E> tail = head;
            while(tail.next != null){
                if (tail.next.data.equals(obj)){
                    temp = tail.next;
                    tail.next = tail.next.next;
                    --size;
                    addRemovedNode(temp);
                    return temp;
                }
                tail = tail.next;
            }
        }
        return null;
    }

    /**
     * Method to add deleted elements to removed node
     * @param deletedNode Element to add.
     */
    private void addRemovedNode(Node<E> deletedNode) {
        if (removedNode == null) {
            removedNode = deletedNode;
            removedNode.next = null;
        } else {
            Node<E> temp = removedNode;
            while(temp.next != null)
                temp = temp.next;

            temp.next = deletedNode;
            deletedNode.next = null;
        }
    }
    // Inner Classes

    /**
     * A Node is the building block for a single-linked list.
     */
    private static class Node<E> {
        private E data;
        private Node<E> next;


        /**
         * Construct a node with the given data value.
         *
         * @param dataItem The data value
         */
        private Node(E dataItem) {
            data = dataItem;
            next = null;
        }
    } //end class Node

    /**
     * A single linked list iterator allows you to access the next elements
     * The nextItem, nextItemReturned and nextItemPrev variables are sufficient for this.
     * It also keeps an index to reach the indexed element.
     */
    private class SingleLinkedListIterator implements Iterator<E> {


        private Node<E> nextItem;
        private Node<E> lastItemReturned;
        private Node<E> nextItemPrev;
        private int index;


        public SingleLinkedListIterator(){
            nextItem = head;
            lastItemReturned = null;
            nextItemPrev = null;
            index = 0;
        }

        /**
         * Construct a SingleLinkedListIterator that will reference the ith item.
         *
         * @param i The index of the item to be referenced
         */
        public SingleLinkedListIterator(int i) {

            if (i < 0 || i > size) {
                throw new IndexOutOfBoundsException(
                        "Invalid index " + i);
            }
            lastItemReturned = null;
            // Special case of last item.

            // Start at the beginning
            nextItem = head;
            nextItemPrev = null;
            for (index = 0; index < i; index++) {
                nextItemPrev = nextItem;
                nextItem = nextItem.next;
            }
        }

        /**
         * Indicate whether movement forward is defined.
         *
         * @return true if call to next will not throw an exception
         */
        public boolean hasNext() {
            return nextItem != null;
        }

        /**
         * Move the iterator forward and return the next item.
         *
         * @return The next item in the list
         * @throws NoSuchElementException if there is no such object
         */
        public E next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            lastItemReturned = nextItem;
            nextItemPrev = nextItem;
            nextItem = nextItem.next;
            index++;
            return lastItemReturned.data;
        }


        /**
         * Return the index of the next item to be returned by next
         *
         * @return the index of the next item to be returned by next
         */
        public int nextIndex() {
            return index;
        }

        public void remove() {
            lastItemReturned = null;

            if (nextItem == head) {
                addRemovedNode(head);
                head = head.next;
            } else {
                addRemovedNode(nextItem);
                nextItemPrev.next = nextItem.next;
            }
            --size;
        }

        /**
         * @param obj
         */
        public void add(E obj) {

            if (head == null) {
                head = new Node<E>(obj);
            } else if (nextItem == head) { // Insert at head.
                Node<E> newNode = new Node<E>(obj);
                newNode.next = head;
                head = newNode;
            } else if (nextItem == null) {  // add tail
                Node<E> newNode = new Node<E>(obj);
                nextItemPrev.next = newNode;
            } else { // add middle
                Node<E> newNode = new Node<E>(obj);
                newNode.next = nextItem;
                nextItemPrev.next = newNode;
            }
            size++;
            index++;
            lastItemReturned = null;
        }
    } //end class SingleLinkedListIterator
}
