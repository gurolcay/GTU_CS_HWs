package cay.gurol.mehmet;

import java.util.ArrayList;
import java.util.Stack;

/**
 * Created by Mehmet Gurol Cay on 23.03.2017.
 */
public class StackA<E> extends ArrayList<E> implements StackInterface<E> {

    /**
     * Pushes an item onto the top of the stack and returns
     * the item pushed.
     * @param item  The Object to be insterted
     * @return The object inserted
     */
    @Override
    public E push(E item) {
        super.add(item);
        return item;
    }

    /**
     * Returns the object at he top of the stack and removes it
     * @return The obejct at the top of the stack
     * @throws java.util.EmptyStackException if stack is empty
     */
    @Override
    public E pop() {
        if(isEmpty()){
            return null;
        } else {
            return super.remove(size() - 1);
        }
    }

    /**
     * Returns true if the stack is empty; otherwise, returns false.
     * @return true if the stack is empty
     */
    public boolean isEmpty(){
        return super.isEmpty();
    }

}
