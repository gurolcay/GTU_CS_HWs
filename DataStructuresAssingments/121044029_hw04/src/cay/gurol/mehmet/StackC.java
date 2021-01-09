package cay.gurol.mehmet;

/**
 * Created by Mehmet Gurol Cay on 23.03.2017.
 */
public class StackC<E> implements StackInterface<E> {

    private Node<E> topOfStackRef;

    private static class Node <E>{
        private E data;
        private Node next;

        private Node(E dataItem){
            data = dataItem;
            next = null;
        }

        private Node(E dataItem, Node<E> nodeRef){
            data = dataItem;
            next = nodeRef;
        }
    }

    /**
     * Pushes an item onto the top of the stack and returns
     * the item pushed.
     * @param item  The Object to be insterted
     * @return The object inserted
     */
    @Override
    public E push(E item) {
        topOfStackRef = new Node<E>(item,topOfStackRef);
        return item;
    }

    /**
     * Returns the object at he top of the stack and removes it
     * @return The obejct at the top of the stack
     * @throws java.util.EmptyStackException if stack is empty
     */
    @Override
    public E pop() {
        E returnedValue = null;
        if (!isEmpty()){
            returnedValue = topOfStackRef.data;
            topOfStackRef = topOfStackRef.next;
        }
        return returnedValue;
    }

    /**
     * Returns true if the stack is empty; otherwise, returns false.
     * @return true if the stack is empty
     */
    @Override
    public boolean isEmpty() {
        if (topOfStackRef == null)
            return true;
        else
            return false;
    }

    /**
     * Returns size of stack.
     * @return returns size of stack
     */
    @Override
    public int size() {
        Node<E> temp;
        int size = 0;

        if (!isEmpty()){
            ++size;
        }
        temp = topOfStackRef;

        while(temp.next != null){
            temp = temp.next;
            ++size;
        }

        return size;
    }
}
