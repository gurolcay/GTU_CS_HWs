package cay.gurol.mehmet;

import java.util.Iterator;
import java.util.Queue;

/**
 * Created by Mehmet Gurol Cay on 22.03.2017.
 */
public class MyQueue<E> extends KWLinkedList {

    /**
     * Retrieves and removes the head of this queue,
     * or return null if this queue is empty.
     * @return the head of this queue, or null if this queue is empty
     */
    public E poll(){
        E item = peek();
        Iterator myIter = listIterator();

        if (myIter != null){
            item = (E) myIter.next();
            myIter.remove();
        }

        return item;
    }

    /**
     * Retrieves, but does not remove, the head of this queue,
     * or returns {@code null} if this queue is empty.
     * @return the head of this queue, or null if this queue is empty
     */
    public E peek(){
        E item = null;

        if (getFirst() != null){
            item = (E) getFirst();
        }

        return item;
    }

    /**
     * Inserts the specified element into this queue if it is possible to do
     * so immediately without violating capacity restrictions.
     * @param item the element to add
     * @return {@code true} if the element was added to this queue, else
     *         {@code false}
     */
    public boolean offer(E item){
        addLast(item);

        if (getLast().equals(item))
            return true;

        return false;
    }

    /**
     * Ensures that the content of the queue is displayed properly.
     * @return formatted string of this queue
     */
    @Override
    public String toString() {
        StringBuilder stBuilder = new StringBuilder();

        for (int i = 0; listIterator(i).hasNext(); ++i){
            stBuilder.append(listIterator(i).next() + " ");
        }

        return stBuilder.toString();
    }

    /**
     * Inverts the elements in this queue.
     */
    public void reverse() {
        int prevSize = size();

        for (int i = size() - 1; 0 <= i ; i--) {
            offer((E) get(i));
        }

        for (int i = 0; i < prevSize; ++i){
            poll();
        }
    }

    /**
     * Count the number of queue elements.
     * @return number of queue elements.
     */
    public int size(){
        int i;
        for (i = 0; listIterator(i).hasNext(); ++i);
        return i;
    }

    /**
     * It takes a queue object of any type and recursively reverses it.
     * @param queueItem The queue to be reversed.
     */
    public void reverse(Queue<E> queueItem){
        E data = null;

        if (!queueItem.isEmpty()){
            data = queueItem.poll();
            reverse(queueItem);
            queueItem.offer(data);
        }
    }
}
