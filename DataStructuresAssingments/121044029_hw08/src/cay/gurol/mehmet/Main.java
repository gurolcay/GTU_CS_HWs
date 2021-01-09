package cay.gurol.mehmet;

public class Main {

    public static void main(String[] args) {
	    AVLTree<String> avlTree = new AVLTree<>();

	    avlTree.add("nush");
	    avlTree.add("ile");
	    avlTree.add("uslanmayanı");
	    avlTree.add("etmeli");
	    avlTree.add("tekdir");
	    avlTree.add("tekdir");
	    avlTree.add("ile");
	    avlTree.add("uslanmayanın");
		System.out.println(avlTree.toString() + "\n");
		avlTree.add("hakkı");
		System.out.println(avlTree.toString() + "\n");
	    avlTree.add("kötektir");
		System.out.println(avlTree.toString() + "\n");

		System.out.println("Insert edille");
		avlTree.add("edille");
		System.out.println(avlTree.toString() + "\n");

		System.out.println("Insert dakik");
		avlTree.add("dakik");
		System.out.println(avlTree.toString() + "\n");

		System.out.println("Insert ferc");
		avlTree.add("ferc");
		System.out.println(avlTree.toString() + "\n");

		System.out.println("Deleting first tree item");
		avlTree.delete();
		System.out.println(avlTree.toString() + "\n");

		System.out.println("Deleting first tree item");
		avlTree.delete();
		System.out.println(avlTree.toString() + "\n");

		System.out.println("Deleting first tree item");
		avlTree.delete();
		System.out.println(avlTree.toString() + "\n");

    }
}
