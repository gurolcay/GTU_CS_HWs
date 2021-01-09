package cay.gurol.mehmet;

import java.util.Random;

/**
 * Created by Mehmet Gürol Çay on 21.04.2017.
 */
public class Passenger {
    // Data Fields
    /** The ID number for this passenger. */
    private int passengerId;

    /** The time needed to process this passenger. */
    private int processingTime;

    /** The time this passenger arrives. */
    private int arrivalTime;

    /** The maximum time to process a passenger. */
    private static int maxProcessingTime;

    /** The sequence number for passengers. */
    private static int idNum = 0;
    /**     */
    private boolean isFrequentFlyer;

    /** Create a new passenger.
     @param arrivalTime The time this passenger arrives */
    public Passenger(int arrivalTime, boolean isFrequentFlyer) {
        this.arrivalTime = arrivalTime;
        processingTime = 1 + (new Random()).nextInt(maxProcessingTime);
        passengerId = idNum++;
        this.isFrequentFlyer = isFrequentFlyer;
    }

    /** Get the arrival time.
     @return The arrival time */
    public int getArrivalTime() {
        return arrivalTime;
    }

    /** Get the processing time.
     @return The processing time */
    public int getProcessingTime() {
        return processingTime;
    }

    /** Get the passenger ID.
     @return The passenger ID */
    public int getId() {
        return passengerId;
    }

    /** Set the maximum processing time
     @param maxProcessTime The new value */
    public static void setMaxProcessingTime(int maxProcessTime) {
        maxProcessingTime = maxProcessTime;
    }

    /**
     *
     * @return
     */
    public boolean isFrequentFlyer() {
        return isFrequentFlyer;
    }
}
