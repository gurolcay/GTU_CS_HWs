package cay.gurol.mehmet;

import java.util.EmptyStackException;
import java.util.LinkedList;
import java.util.Queue;

/**
 * Created by Mehmet Gurol Cay on 23.03.2017.
 */
public class StackD<E>  implements StackInterface<E> {

    private Queue<E> queue;

    public StackD() {
        queue = new LinkedList<E>() ;
    }

    /**
     * Pushes an item onto the top of the stack and returns
     * the item pushed.
     * @param item  The Object to be insterted
     * @return The object inserted
     */
    @Override
    public E push(E item) {

        if(queue.add(item))
            return item;
        return null;
    }

    /**
     * Returns the object at he top of the stack and removes it
     * @return The obejct at the top of the stack
     * @throws java.util.EmptyStackException if stack is empty
     */
    @Override
    public E pop() {
        E returnedValue;
        reverse(queue);
        returnedValue = queue.poll();
        reverse(queue);

         if ( returnedValue != null )
             return returnedValue;
         else
             throw new EmptyStackException();

    }

    /**
     * Returns true if the stack is empty; otherwise, returns false.
     * @return true if the stack is empty
     */
    @Override
    public boolean isEmpty() {
        if (queue.isEmpty())
            return true;
        return false;
    }

    /**
     * Returns size of stack.
     * @return returns size of stack
     */
    @Override
    public int size() {
        return queue.size();
    }

    private void reverse(Queue<E> queueItem){
        E data = null;

        if (!isEmpty()){
            data = queueItem.poll();
            reverse(queueItem);
            queueItem.offer(data);
        }
    }
}
