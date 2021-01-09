package cay.gurol.mehmet;

import java.util.Iterator;

/**
 * Created by Mehmet Gürol Çay on 11.03.2017.
 *
 *
 */
public class MyStringBuilder<E>   {

    private SingleLinkedList<E> singleLinkedList = null;

    public MyStringBuilder()
    {
        singleLinkedList = new SingleLinkedList<>();
    }

    /**
     * Adds the given object to the end of the single linked list
     * @param obj This element is added to the single linked list
     * @return myStringBuilder
     */
    public MyStringBuilder<E> append(E obj)
    {
        singleLinkedList.addLast(obj);
        return this;
    }

    /**
     * The toString method works by index
     * @return string
     */
    public String toStringIndex() {
        String str = new String();
        for (int i = 0; i < singleLinkedList.getSize(); i++) {
            str += singleLinkedList.get(i).toString() + ",";
        }
        return str;
    }

    /**
     * The toString method works by linked list toString method
     * @return string
     */
    public String toStringLinkedList(){
        return singleLinkedList.toString();
    }

    /**
     * The toString method works by single linked list iterator.
     * @return string
     */
    public String toStringIterator() {
        String str = new String();
        Iterator<E> newIt = singleLinkedList.iterator();

        while (newIt.hasNext()){
            str += newIt.next() + ",";
        }
        return str;
    }
}
