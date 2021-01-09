package cay.gurol.mehmet;

import com.sun.scenario.effect.impl.sw.sse.SSEBlend_SRC_OUTPeer;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Main {

    public static void main(String[] args) {
        MyHuffmanTree hfftree = new MyHuffmanTree();
        String encoded = "";
        hfftree.readFromFile();


        encoded = hfftree.encode("gurol");

        System.out.println("Given string: gurol");
        if (encoded.equals("100001000010010100110110")) {
            System.out.println("encoded string: " + encoded);
            System.out.println("true");
        }
        else
            System.out.println("false");

        encoded = hfftree.encode("g rol");

        System.out.println("Given string: g rol");
        if (encoded.equals("1000011110010100110110")) {
            System.out.println("encoded string: " + encoded);
            System.out.println("true");
        }
        else
            System.out.println("false");

//        AirlineCheckinSim airlineCheckinSim = new AirlineCheckinSim();
//        airlineCheckinSim.addToQueue(new Passenger(6,true));
//        airlineCheckinSim.addToQueue(new Passenger(18,true));
//        airlineCheckinSim.addToQueue(new Passenger(29,true));
//        airlineCheckinSim.addToQueue(new Passenger(20,true));
//        airlineCheckinSim.addToQueue(new Passenger(28,true));
//        airlineCheckinSim.addToQueue(new Passenger(39,true));
//        airlineCheckinSim.addToQueue(new Passenger(66,true));
//        airlineCheckinSim.addToQueue(new Passenger(37,true));
//        airlineCheckinSim.addToQueue(new Passenger(26,true));
//        airlineCheckinSim.addToQueue(new Passenger(76,true));
//        airlineCheckinSim.addToQueue(new Passenger(32,true));
//        airlineCheckinSim.addToQueue(new Passenger(74,true));
//        airlineCheckinSim.addToQueue(new Passenger(89,true));
//        airlineCheckinSim.addToQueue(new Passenger(8,true));


    }
}
