package cay.gurol.mehmet;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.*;

/**
 * Abstract base class for graphs. This class adds
 * some features to AbstractGraph.
 * Created by Mehmet Gürol Çay on 20.05.2017.
 */
public abstract class AbstractGraphExtended extends AbstractGraph {

    public AbstractGraphExtended(int numV, boolean directed) {
        super(numV, directed);
    }

    /**
     * This method selects a random number between 0 and edgeLimit
     * then adds that much edges to calling graph. The source and
     * target edges of the edges are randomly selected again.
     * @param edgeLimit Upper limit for random number.
     * @return The number of edges added to the graph.
     */
    public int addRandomEdgesToGraph(int edgeLimit) {
        int addedNumOfEdge = 0;
        int verticesNum;
        int randSrc;
        int randDst;
        int addNumOfEdge;

        Random rand = new Random();
        Random randSource = new Random();
        Random randDest = new Random();

        System.out.println("edgeLimit: " + edgeLimit);


        if (this instanceof ListGraph || this instanceof MatrixGraph) {

            addNumOfEdge = rand.nextInt(edgeLimit);

            for (int i = 0; i < addNumOfEdge; i++) {
                verticesNum = this.getNumV();
                randSrc = randSource.nextInt(verticesNum);
                randDst = randDest.nextInt(verticesNum);

                if (!this.isEdge(randSrc, randDst)) {
                    Edge e = new Edge(randSrc, randDst, 1);
                    this.insert(e);
                    ++addedNumOfEdge;
                }
            }
        }

        return addedNumOfEdge;
    }

    /**
     * Perform a breadth-first search of a graph.
     * post: The array parent will contain the predecessor
     * of each vertex in the breadth-first
     * search tree.
     * @param start The start vertex
     * @return The array of parents
     */
    public int[] breadthFirstSearch(int start) {
        Queue<Integer> theQueue = new LinkedList<Integer>();

        int[] parentArr = new int[this.getNumV()];

        for (int i = 0; i < this.getNumV(); i++) {
            parentArr[i] = -1;
        }

        boolean[] identified = new boolean[this.getNumV()];
        identified[start] = true;
        theQueue.offer(start);

        while (!theQueue.isEmpty()) {
            int current = theQueue.remove();
            Iterator<Edge> iter = this.edgeIterator(current);

            while (iter.hasNext()) {
                Edge edge = iter.next();
                int neighbor = edge.getDest();

                if (!identified[neighbor]) {
                    identified[neighbor] = true;
                    theQueue.offer(neighbor);
                    parentArr[neighbor] = current;
                }
            }
        }
        return parentArr;
    }

    /**
     * This method finds connected components in a graph, creates
     * ListGraph or MatrixGraph instances for
     * each connected component and returns them in a Graph array.
     * @return Graph array of connected component
     */
    public Graph[] getConnectedComponentUndirectedGraph() {
        return null;
    }

    /**
     * A bipartite graph is a graph whose vertices can be divided into
     * two independent sets, U and V such that every edge (u, v) either
     * connects a vertex from U to V or a vertex from V to U
     * @return  returns true if calling graph instance is bipartite graph, false otherwise.
     */
    public boolean isBipartiteUndirectedGraph() {
        ArrayList<String> U = new ArrayList<>();
        ArrayList<String> V = new ArrayList<>();
        U.add("RED");

        for (int i = 0, j = 0; i < this.getNumV(); i++) {
            Iterator<Edge> iter = this.edgeIterator(i);
            U.add("RED");
            while (iter.hasNext()) {
                iter.next();
                V.add("BLUE");
            }
        }

        for (int i = 0; i < U.size(); i++) {
            for (int j = 0; j < V.size(); j++) {
                if (U.get(i).equals(V.get(j))) {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * This method will do the reverse of createGraph method in the
     * AbstractGraph abstract class and write number of vertices,
     * source and destination vertex of each edge in a file.
     * @param fileName Output file name
     */
    public void writeGraphToFile(String fileName) {
        try {
            PrintWriter writer = new PrintWriter(fileName);

            writer.println(this.getNumV());

            for (int i = 0; i < this.getNumV(); i++) {
                Iterator<Edge> iter = this.edgeIterator(i);
                while (iter.hasNext()) {
                    Edge e = iter.next();
                    writer.println(e.getSource() + " " + e.getDest());
                }
            }
            writer.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    /**
     * Return a String representation of the graph
     * @return A stirng representation of the graph
     */
    @Override
    public String toString() {
        StringBuilder str = new StringBuilder();
        for (int i = 0; i < this.getNumV(); i++) {
            Iterator<Edge> iter = this.edgeIterator(i);
            while (iter.hasNext()) {
                Edge e = iter.next();
                str.append(e.toString() + "\n");
            }
        }
        return str.toString();
    }
}
