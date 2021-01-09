/**
 * Created by Mehmet Gurol Cay on 22.03.2017.
 */

package cay.gurol.mehmet;

import java.io.*;
import java.util.LinkedList;
import java.util.Queue;

public class Main {
    private static String COMA_DELIMETER = ",";
    private static String NEW_LINE_DELIMETER = "\n";
    private static String INPUT_FILE_NAME = "test.csv";
    private static String TEST_1_RESULT_FILE_NAME = "testResult_1.csv";
    private static String TEST_2_RESULT_FILE_NAME = "testResult_2.csv";


    Queue<String> myquQueue;

    LinkedList<String> myLinkedList;
    public static void main(String[] args) {
        testQ1();
        testQ2();
    }

    /**
     * Test first query. It reads the test.csv file for StackA, StackB, StackC,
     * and StackD and saves their results in the testResult_1.csv file.
     */
    public static void testQ1(){

        testQ1StackA();
        testQ1StackB();
        testQ1StackC();
        testQ1StackD();
    }

    /**
     * Test StackA
     */
    private static void testQ1StackA(){
        StackA<Integer> stackAInt = new StackA<>();
        StackA<Double> stackADouble = new StackA<>();
        StackA<Character> stackAChar = new StackA<>();
        StackA<String> stackAStr = new StackA<>();

        String line;

        try {
            BufferedReader bf = new BufferedReader(new FileReader(INPUT_FILE_NAME));

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackAInt.push(Integer.parseInt(tokens[i]));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackADouble.push(Double.parseDouble(tokens[i]));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackAChar.push(tokens[i].charAt(0));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackAStr.push(tokens[i]);
                }
            }
            bf.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            FileWriter writer = new FileWriter(TEST_1_RESULT_FILE_NAME);
            int size = stackAInt.size();
            writer.append(Integer.toString(stackAInt.size()));
            writer.append(COMA_DELIMETER);

            for (int i = 0; i < size; i++) {
                writer.append(Integer.toString(stackAInt.pop()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackADouble.size();
            writer.append(Integer.toString(stackADouble.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(Double.toString(stackADouble.pop()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackAChar.size();
            writer.append(Integer.toString(stackAChar.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(stackAChar.pop());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackAStr.size();
            writer.append(Integer.toString(stackAStr.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(stackAStr.pop());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Test StackB
     */
    private static void testQ1StackB(){
        StackB<Integer> stackBInt = new StackB<>();
        StackB<Double> stackBDouble = new StackB<>();
        StackB<Character> stackBChar = new StackB<>();
        StackB<String> stackBStr = new StackB<>();

        String line;

        try {
            BufferedReader bf = new BufferedReader(new FileReader(INPUT_FILE_NAME));

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackBInt.push(Integer.parseInt(tokens[i]));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackBDouble.push(Double.parseDouble(tokens[i]));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackBChar.push(tokens[i].charAt(0));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackBStr.push(tokens[i]);
                }
            }
            bf.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }


        try {
            FileWriter writer = new FileWriter(TEST_1_RESULT_FILE_NAME,true);
            int size = stackBInt.size();
            writer.append(Integer.toString(stackBInt.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(Integer.toString(stackBInt.pop()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackBDouble.size();
            writer.append(Integer.toString(stackBDouble.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(Double.toString(stackBDouble.pop()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackBChar.size();
            writer.append(Integer.toString(stackBChar.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(stackBChar.pop());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackBStr.size();
            writer.append(Integer.toString(stackBStr.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(stackBStr.pop());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    /**
     * Test StackC
     */
    private static void testQ1StackC(){
        StackC<Integer> stackCInt = new StackC<>();
        StackC<Double> stackCDouble = new StackC<>();
        StackC<Character> stackCChar = new StackC<>();
        StackC<String> stackCStr = new StackC<>();

        String line;

        try {
            BufferedReader bf = new BufferedReader(new FileReader(INPUT_FILE_NAME));

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackCInt.push(Integer.parseInt(tokens[i]));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackCDouble.push(Double.parseDouble(tokens[i]));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackCChar.push(tokens[i].charAt(0));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackCStr.push(tokens[i]);
                }
            }
            bf.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            FileWriter writer = new FileWriter(TEST_1_RESULT_FILE_NAME,true);
            int size = stackCInt.size();
            writer.append(Integer.toString(stackCInt.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(Integer.toString(stackCInt.pop()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackCDouble.size();
            writer.append(Integer.toString(stackCDouble.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(Double.toString(stackCDouble.pop()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackCChar.size();
            writer.append(Integer.toString(stackCChar.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(stackCChar.pop());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackCStr.size();
            writer.append(Integer.toString(stackCStr.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(stackCStr.pop());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Test StackD
     */
    private static void testQ1StackD(){
        StackD<Integer> stackDInt = new StackD<>();
        StackD<Double> stackDDouble = new StackD<>();
        StackD<Character> stackDChar = new StackD<>();
        StackD<String> stackDStr = new StackD<>();

        String line;

        try {
            BufferedReader bf = new BufferedReader(new FileReader(INPUT_FILE_NAME));

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackDInt.push(Integer.parseInt(tokens[i]));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackDDouble.push(Double.parseDouble(tokens[i]));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackDChar.push(tokens[i].charAt(0));
                }
            }

            if((line = bf.readLine()) != null){
                String[] tokens = line.split(COMA_DELIMETER);

                for (int i = 0; i < tokens.length; ++i){
                    stackDStr.push(tokens[i]);
                }
            }
            bf.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            FileWriter writer = new FileWriter(TEST_1_RESULT_FILE_NAME,true);
            int size = stackDInt.size();
            writer.append(Integer.toString(stackDInt.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(Integer.toString(stackDInt.pop()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackDDouble.size();
            writer.append(Integer.toString(stackDDouble.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(Double.toString(stackDDouble.pop()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackDChar.size();
            writer.append(Integer.toString(stackDChar.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(stackDChar.pop());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = stackDStr.size();
            writer.append(Integer.toString(stackDStr.size()));
            writer.append(COMA_DELIMETER);
            for (int i = 0; i < size; i++) {
                writer.append(stackDStr.pop());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * It tests the second question. Read the test.csv file
     * and save the contents of the reverse queue to the testResult_2.csv file.
     */
    public static void testQ2(){
        MyQueue<Integer> myQueueInt = new MyQueue<>();
        MyQueue<Double> myQueueDouble = new MyQueue<>();
        MyQueue<Character> myQueueChar = new MyQueue<>();
        MyQueue<String> myQueueStr = new MyQueue<>();

        Queue<String> queueReverseStr = new LinkedList<>();
        Queue<Integer> queueReverseInt = new LinkedList<>();
        Queue<Double> queueReverseDbl = new LinkedList<>();
        Queue<Character> queueReverseChar = new LinkedList<>();


        boolean intFlag = true;
        boolean doubleFlag = false;
        boolean charFlag = false;
        boolean strFlag = false;


        try {
            BufferedReader reader = new BufferedReader(new FileReader(INPUT_FILE_NAME));
            String line;
            String[] tokens;

            if( (line = reader.readLine()) != null) {
                tokens = line.split(COMA_DELIMETER);

                if (tokens.length > 0 && intFlag) {
                    for (int i = 0; i < tokens.length; i++) {
                        myQueueInt.offer(Integer.parseInt(tokens[i]));
                        queueReverseInt.offer(Integer.parseInt(tokens[i]));
                    }
                    intFlag = false;
                    doubleFlag = true;
                }
            }
            if( (line = reader.readLine()) != null) {
                tokens = line.split(COMA_DELIMETER);
                if (tokens.length > 0 && doubleFlag) {
                    for (int i = 0; i < tokens.length; ++i) {
                        myQueueDouble.offer(Double.parseDouble(tokens[i]));
                        queueReverseDbl.offer(Double.parseDouble(tokens[i]));
                    }
                    doubleFlag = false;
                    charFlag = true;
                }
            }
            if( (line = reader.readLine()) != null) {
                tokens = line.split(COMA_DELIMETER);
                if (tokens.length > 0 && charFlag) {
                    for (int i = 0; i < tokens.length; i++) {
                        myQueueChar.offer(tokens[i].charAt(0));
                        queueReverseChar.offer(tokens[i].charAt(0));
                    }

                    charFlag = false;
                    strFlag = true;
                }
            }
            if( (line = reader.readLine()) != null) {
                tokens = line.split(COMA_DELIMETER);
                if (tokens.length > 0 && strFlag){
                    for (int i = 0; i < tokens.length; i++) {
                        myQueueStr.offer(tokens[i]);
                        queueReverseStr.offer(tokens[i]);
                    }
                    strFlag = false;
                }
            }

            myQueueInt.reverse();
            myQueueDouble.reverse();
            myQueueChar.reverse();
            myQueueStr.reverse();

            myQueueStr.reverse(queueReverseStr);
            myQueueInt.reverse(queueReverseInt);
            myQueueChar.reverse(queueReverseChar);
            myQueueDouble.reverse(queueReverseDbl);

            FileWriter writer = new FileWriter(TEST_2_RESULT_FILE_NAME);
            int size = myQueueInt.size();

            for (int i = 0; i < size; i++) {
                writer.append(Integer.toString(myQueueInt.poll()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = myQueueDouble.size();
            for (int i = 0; i < size; ++i){
                writer.append(Double.toString(myQueueDouble.poll()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = myQueueChar.size();
            for (int i = 0; i < size; ++i){
                writer.append(myQueueChar.poll());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = myQueueStr.size();
            for (int i = 0; i < size; ++i){
                writer.append(myQueueStr.poll());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = queueReverseInt.size();
            for (int i = 0; i < size; ++i){
                writer.append(Integer.toString(queueReverseInt.poll()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = queueReverseDbl.size();
            for (int i = 0; i < size; ++i){
                writer.append(Double.toString(queueReverseDbl.poll()));
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = queueReverseChar.size();
            for (int i = 0; i < size; ++i){
                writer.append(queueReverseChar.poll());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            size = queueReverseStr.size();
            for (int i = 0; i < size; ++i){
                writer.append(queueReverseStr.poll());
                writer.append(COMA_DELIMETER);
            }
            writer.append(NEW_LINE_DELIMETER);

            writer.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}

