package cay.gurol.mehmet;

/**
 * Created by Mehmet Gurol Cay on 23.03.2017.
 */
public interface StackInterface<E> {
    /**
     * Pushes an item onto the top of the stack and returns
     * the item pushed.
     * @param item  The Object to be insterted
     * @return The object inserted
     */
    public E push(E item);

    /**
     * Returns the object at he top of the stack and removes it
     * @return The obejct at the top of the stack
     * @throws java.util.EmptyStackException if stack is empty
     */
    public E pop();

    /**
     * Returns true if the stack is empty; otherwise, returns false.
     * @return true if the stack is empty
     */
    public boolean isEmpty();

    /**
     * Returns size of stack.
     * @return returns size of stack
     */
    public int size();

}
