package cay.gurol.mehmet;

import java.util.ArrayList;

/**
 * Created by Mehmet Gurol Cay on 23.03.2017.
 */
public class StackB<E> implements StackInterface<E>  {

    private ArrayList<E> arrayList;


    public StackB() {
        arrayList = new ArrayList<E>();
    }

    /**
     * Pushes an item onto the top of the stack and returns
     * the item pushed.
     * @param item  The Object to be insterted
     * @return The object inserted
     */
    @Override
    public E push(E item) {
        if(arrayList.add(item))
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
        return arrayList.remove(size() - 1);
    }

    /**
     * Returns true if the stack is empty; otherwise, returns false.
     * @return true if the stack is empty
     */
    @Override
    public boolean isEmpty() {
        return arrayList.isEmpty();
    }

    /**
     * Returns size of stack.
     * @return returns size of stack
     */
    @Override
    public int size() {
        return arrayList.size();
    }
}
