package cay.gurol.mehmet;

import java.io.*;
import java.util.Random;
import java.util.concurrent.TimeUnit;

public class Main {

    public static void main(String[] args) {
        System.out.println("Testing Q1)");
        testFirstPart();
        System.out.println("End of test.");
        System.out.println("Testing Q2)");
        testSecondPart();
        System.out.println("End of test.");
        System.out.println("Testing Q4)");
        testFourtPart();
        System.out.println("End of test.");

    }

    /**
     * Test the fourth part
     * Generate random 100 number and add to single linked list and remove 50 random number from single linked list
     * and again add 100 random number to single linked list.
     */
    public static void testFourtPart(){
        SingleLinkedList<Integer> singleLinkedList = new SingleLinkedList<>();
        Random randomGenerator = new Random();

        int randomInt = randomGenerator.nextInt(10);
        singleLinkedList.addFirst(randomInt);

        for(int i = 0; i < 100; i++){
            randomInt = randomGenerator.nextInt(10);
            singleLinkedList.addLast(randomInt);
        }

        for (int i = 0; i < 50; i++) {
            randomInt = randomGenerator.nextInt(10);
            singleLinkedList.remove(randomInt);
        }

        for(int i = 0; i < 100; i++){
            randomInt = randomGenerator.nextInt(50);
            singleLinkedList.addLast(randomInt);
        }
        System.out.println("Single linkend list elements:\n" +singleLinkedList.toString());
        System.out.println("Removed single likend list elements:\n" + singleLinkedList.printRemovedNode());

    }

    /**
     * Test the second part
     * Singly linked list adds two elements and inverts them with the reversed ToString method
     */
    public static void testSecondPart() {
        SingleLinkedList<String> singleLinkedList = new SingleLinkedList<>();
        singleLinkedList.addFirst("gurol");
        singleLinkedList.addLast("cay");
        System.out.println("Normal string: " + singleLinkedList.toString());
        System.out.println("Reversed string: " + singleLinkedList.reverseToString());
    }

    /**
     * Test the first part medhod. This method generate random 100000 integer value and save to the numbers.txt file
     * Reads it again from the file and saves them in result1.txt, result2.txt and result3.txt.
     * Use different toString methods for recording.
     */
    public static void testFirstPart() {
        Random randomGenerator = new Random();
        Writer writer = null;
        try {
            writer = new BufferedWriter(new OutputStreamWriter( new FileOutputStream("numbers.txt"), "utf-8"));
            for (int i = 0; i < 100000; i++) {
                int randomInt = randomGenerator.nextInt(100000);
                writer.write(randomInt + "\n");
            }

        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try{
                writer.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        String line = null;


        MyStringBuilder<String> myStringBuilder = new MyStringBuilder<>();

        try {
            BufferedReader reader = new BufferedReader(new FileReader("numbers.txt"));
            int number = 0;
            while ((line = reader.readLine()) != null) {
                if(line != null)
                    myStringBuilder.append(line);
            }

            reader.close();
//            Writer writer;
            long current;
            long current2;
            double seconds;

            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream("result1.txt"), "utf-8"));
            current = System.nanoTime();
            writer.write(myStringBuilder.toStringIndex());
            current2 = System.nanoTime();
            seconds = (double) (current2 - current) / 1000000000.0;
            System.out.println("toStringIndex: " + seconds + "seconds");
            writer.close();

            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream("result2.txt"), "utf-8"));
            current = System.nanoTime();
            writer.write(myStringBuilder.toStringIterator());
            current2 = System.nanoTime();
            seconds = (double) (current2 - current) / 1000000000.0;
            System.out.println("toStringIterator: " + seconds + "seconds");
            writer.close();

            writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream("result3.txt"), "utf-8"));
            current = System.nanoTime();
            writer.write(myStringBuilder.toStringLinkedList());
            current2 = System.nanoTime();
            seconds = (double) (current2 - current) / 1000000000.0;
            System.out.println("toStringLinkedList: " + seconds + "seconds");
            writer.close();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

}
