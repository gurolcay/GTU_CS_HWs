package cay.gurol.mehmet;

import java.io.File;
import java.io.IOException;
import java.util.Scanner;

/**
 * Created by Mehmet Gürol Çay on 20.05.2017.
 */
public class Main {
    public static void main(String[] args) {
        ListGraph listGraph;
        MatrixGraph matrixGraph;

        Scanner scanner = null;
        try {
            scanner = new Scanner(new File("ExampleIGraphInputOrOutputFile.txt"));
            listGraph = (ListGraph) AbstractGraph.createGraph(scanner,true,"List");
            System.out.println("ListGraph is testing... ");
            System.out.println("Read graph from file: ");
            System.out.println(listGraph.toString());
            System.out.println("Number of edges added: "
                                    + listGraph.addRandomEdgesToGraph(10));
            System.out.println("New list graph");
            System.out.println(listGraph.toString());
            listGraph.writeGraphToFile("newListGraph");


            System.out.println("MatrixGraph is testing...");
            scanner = new Scanner(new File("ExampleIGraphInputOrOutputFile.txt"));
            matrixGraph = (MatrixGraph) AbstractGraph.createGraph(scanner,true,"Matrix");
            System.out.println("Read graph from file: ");
            System.out.println(matrixGraph.toString());

            System.out.println("Number of edges added to graph "
                            + matrixGraph.addRandomEdgesToGraph(5));
            System.out.println("New matrix graph");
            System.out.println(matrixGraph.toString());

            matrixGraph.writeGraphToFile("newMatrixGraph");


            System.out.println("Breadth First Search testing: ");
            int[] arr;

            System.out.println("Graph: \n" + listGraph.toString());

            System.out.println("Breath First Search testing; start is 0: ");

            arr = listGraph.breadthFirstSearch(0);
            for (int j = 0; j < arr.length; j++) {
                System.out.print(arr[j] + " ");
            }
            System.out.println("");
            System.out.println("BipartiteUndirectedGraph is testing...");
            ListGraph listGraph1 =new ListGraph(6,false);
            listGraph1.insert(new Edge(0,1));
            listGraph1.insert(new Edge(0,3));
            listGraph1.insert(new Edge(0,5));
            listGraph1.insert(new Edge(2,1));
            listGraph1.insert(new Edge(2,5));
            listGraph1.insert(new Edge(4,3));
            listGraph1.insert(new Edge(4,5));
            System.out.println("Graph: ");
            System.out.println(listGraph1.toString());
            System.out.println("isBipartite: " + listGraph1.isBipartiteUndirectedGraph());

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
