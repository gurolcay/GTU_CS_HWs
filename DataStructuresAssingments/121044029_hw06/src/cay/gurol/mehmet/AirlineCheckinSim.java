package cay.gurol.mehmet;

/**
 * Created by Mehmet Gürol Çay on 21.04.2017.
 */
public class AirlineCheckinSim {
    private BinaryHeap <Passenger> passengerQueueBinaryHeap;

    /**
     *
     */
    public AirlineCheckinSim() {
        passengerQueueBinaryHeap = new BinaryHeap<>();
        Passenger.setMaxProcessingTime(20);
    }

    /**
     *
     * @param passenger
     */
    public void addToQueue(Passenger passenger){
        passengerQueueBinaryHeap.offer(passenger);
    }
}
