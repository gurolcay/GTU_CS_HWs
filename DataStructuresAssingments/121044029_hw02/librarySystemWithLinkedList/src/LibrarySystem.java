import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

/**
 * Created by Mehmet Gürol Çay on 20.02.2017.
 */
public class LibrarySystem {

    private List<Book> books;
    private List<User> users;
    private List<Staff> staffs;
    private boolean validMember;
    private CsvFileWriter csvFileWriter;
    private CsvFileReader csvFileReader;

    /**
     * No parameter constructor
     */
    public LibrarySystem() {
        books = new LinkedList<>();
        users = new LinkedList<>();
        staffs = new LinkedList<>();
        validMember = false;
        csvFileWriter = new CsvFileWriter();
        csvFileReader = new CsvFileReader();

        if(!loadSystemFromFile()){
            staffs.add(new Staff("admin", "admin", this));
        }

    }

    /**
     * LibrarySystem objesi olusturulduktan sonra bu methot cagrilarak sistemin ayaga kalkmasi saglanir.
     * Butun islemler burda yapilir.
     * Programin baslamasi ve bitmesi icin kullanicidan input beklenir "start" komutu gelirse kullanici girisi beklenir.
     * Dogru kullanici girisi yapilirsa bu kullanicinin menusu ekranda gosterilir. Yanlis kullanici girisinde tekrar
     * sistem girisi beklenir.
     */
    public void startSystem() {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String startConditional = null;
        while (!isValidMember()) {
            System.out.println("If you exit the program, you must write \"exit\"");
            System.out.println("If you start the program you must write \"start\"");
            try {
                startConditional = br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }

            if (startConditional.equals("start")) {
                System.out.println("Enter user name: ");
                String userName = null;

                try {
                    userName = br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                System.out.println("Enter password: ");
                String password = null;

                try {
                    password = br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }

                for (Staff stff : getStaffs()) {
                    if (stff.getUserName().equals(userName)) {
                        if (stff.getPassword().equals(password)) {
                            setValidMember(true);
                            staffMenu(stff);

                        } else {
                            System.err.println("Invalid password!");

                        }
                    }
                }

                for (User usr : getUsers()) {
                    if (usr.getUserName().equals(userName)) {
                        if (usr.getPassword().equals(password)) {
                            setValidMember(true);
                            userMenu(usr);
                        }
                    }
                }

                if(!isValidMember())
                    System.err.println("This username isn't exist in the system.");

            } else if(startConditional.equals("exit")) {
                getCsvFileWriter().writeUsersToCsvFile(getUsers());
                getCsvFileWriter().writeStaffsToCsvFile(getStaffs());
                getCsvFileWriter().writeBooksToCsvFile(getBooks());
                break;
            } else{
                System.out.println("You must enter \"start\" or \"exit\". Please try again.");
            }
        }
    }

    /**
     * kitaplarin tutuldugu listeyi return eder
     * @return kitaplarin tutuldugu liste dondurulur
     */
    public List<Book> getBooks() {
        return books;
    }

    /**
     * kullanicilarin tutuldugu listeyi return eder.
     * @return kullanicilarin tutuldugu liste dondurulur.
     */
    public List<User> getUsers() {
        return users;
    }

    /**
     * kutuphane gorevlilerin tutuldugu listeye erisilir.
     * @return kutuphane gorevlilerin tutuldugu liste dondurulur
     */
    public List<Staff> getStaffs() {
        return staffs;
    }

    /**
     * dosyaya yazmak icin kullanılan class'in objesi return edilir.
     * @return dosyaya yazma ile sorumlu class'in objesi dondurulur
     */
    public CsvFileWriter getCsvFileWriter() {
        return csvFileWriter;
    }

    /**
     * dosyadan veri okumakla gorevli class'in objesine erisilir.
     * @return dosyadan veri okumakla gorevli class'in obejsi dondurulur.
     */
    public CsvFileReader getCsvFileReader() {
        return csvFileReader;
    }

    /**
     * bir kullanicinin gecerli olup olmadigini tutar.
     * @return kullanicinin gecerli olup olmadigini tutar.
     */
    public boolean isValidMember() {
        return validMember;
    }

    /**
     * kullanicinin gecerli olup olmadigini tutar. gecerli bir kullanici "true" olarak tutulur. eger gecerli bir
     * kullanici degilse false olarak tutulur.
     * @param validMember   kullanicinin gecerli olup olmadigini set edecek yeni degerdir.
     */
    public void setValidMember(boolean validMember) {
        this.validMember = validMember;
    }

    /**
     * Gorevlilerin menusudur.
     * Kullanıcının girecegi degere gore bir case belirlenip islem yapilir. yanlis girme durumunda menu tekrar
     * gosterilir.
     * Gorevli "1" girdigi taktirde yeni kullanici olusturulur
     * "2" girdigi taktirde silinecek olan kullanici ismi girilmesi beklenir. var olan kullanici girildiginde o kullanici
     * Sistemden silinir. eger sistemde olmayan bir kullanici silinmeye calisirsa hata mesaji verilir.
     * "3" girdigi taktirde yeni kitap kaydi gerceklestirilir.
     * "4" girdigi taktirde mevcut kitaplardan birinin silinmesi yapilir. essiz kitap ID'si kullanilir.
     * Gorevli "5" girdigi taktirde sistemde kayitli kullanicilar gosterilir
     * Gorevli "6" girdigi taktirde sistemde kayitli kitaplar gosteriilir.
     * Gorevli "10" girdigi taktirde sistemden cikis yapmis olur ve ana giris ekranina donulur.
     */
    private void staffMenu(Staff staff) {
        printStaffMenu();

        BufferedReader bf = new BufferedReader(new InputStreamReader(System.in));

        String chic = null;
        do {
            try {
                String read = bf.readLine();
                chic = read;
            } catch (IOException e) {
                e.printStackTrace();
            }

            if (chic.equals("1")) {
                try {
                    System.out.println("Enter a user name and password");
                    staff.addUser(bf.readLine(), bf.readLine());
                } catch (IOException e) {
                    System.err.println(e);
                } catch (MyException e) {
                    System.err.println(e);
                }

            } else if (chic.equals("2")) {
                System.out.println("Enter an user name: ");
                try {
                    staff.removeUser(bf.readLine());
                } catch (MyException e) {
                    System.err.println(e);
                } catch (IOException e) {
                    System.err.println(e);
                }
            } else if (chic.equals("3")) {
                System.out.println("Enter a book name, author of book and book's year of printing");
                try {
                    Book book = new Book(bf.readLine(), bf.readLine(), bf.readLine());
                    if (staff.addBook(book))
                        System.out.println("Book is added.");
                    else
                        System.err.println("Book can't added...");
                } catch (IOException e) {
                    System.err.println(e);
                }
            } else if (chic.equals("4")) {
                listBooks();
                System.out.println("Enter book ID:");
                try {
                    String number = bf.readLine();
                    if (!staff.removeBook(Integer.parseInt(number)))
                        System.out.println("Book can't removed...");
                    else
                        System.out.println("Book's removed.");
                } catch (IOException e) {
                    System.err.println(e);
                }
            } else if (chic.equals("5")) {
                listUser();
            } else if (chic.equals("6")) {
                listBooks();
            } else if (chic.equals("10")) {
                setValidMember(false);
                break;
            } else{
                System.err.println("Invalid chic. Please try again.");
            }
            printStaffMenu();
        } while (!chic.equals("10"));
    }

    /**
     * Kullanicilarin menusudur. Kullanicinin sistem calisirken yapabilecekleri gosterilir.
     * Kullanici "1" girdigi taktirde odunc alınabilecek kitaplar listelenir ve kullanicidan kitabın secilmesi beklenir.
     * Kullanici "2" girdigi taktirde odunc almis oldugu kitaplardan iade edebilecekleri listelernir ve kullanicinin bir
     * bookID girmesi beklenir.
     * Kullanici "3" girdigi taktirde sistemde kayitli olan kitaplar listelenir.
     * Kullanici "4" girdigi taktirde, kullanicinin odunc aldigi kitaplar listelenir.
     * Kullanici "10" girdigi taktirde sistemden cikis yapmis olur ve ana giris ekranina donulur.
     */
    private void userMenu(User user) {
        printUserMenu();

        BufferedReader bf = new BufferedReader(new InputStreamReader(System.in));
        String chic = null;

        do {
            try {
                String read = bf.readLine();
                chic = read;
            } catch (IOException e) {
                System.err.println(e);
            }

            if (chic.equals("1")) {
                listBooks();
                System.out.println("Enter the book number to borrow: ");
                try {
                    if (user.barrowBook(Integer.parseInt(bf.readLine()))) {
                        System.out.println("Book is borrowed");
                    } else {
                        System.out.println("Book isn't borrowed!");
                    }

                } catch (IOException e) {
                    System.err.println(e);
                }
            } else if (chic.equals("2")) {
                listBorrowedBooks();
                System.out.println("Enter the number of the book you want to return: ");
                try {
                    if (user.returnBook(Integer.parseInt(bf.readLine()))) {
                        System.out.println("Book is returned.");
                    } else {
                        System.out.println("Book isn't returned.");
                    }
                } catch (IOException e) {
                    System.err.println(e);
                }

            } else if (chic.equals("3")) {
                listBooks();
            } else if (chic.equals("4")) {
                listBorrowedBooks();
            } else if (chic.equals("10")) {
                setValidMember(false);
                break;
            } else{
                System.err.println("Invalid chic. Please try again.");
            }
            printUserMenu();
        } while (!chic.equals("10"));

    }

    /**
     * Sistemde kayitli kullanicilar listelenir.
     */
    private void listUser() {
        System.out.println("****************************************************");
        System.out.println("User name:\t User password:");
        for (User user : getUsers()) {
            System.out.println(user.getUserName() + "\t\t" + user.getPassword());
        }
        System.out.println("****************************************************");
    }

    /**
     * Sistemde kayitli kitaplar listelenir.
     */
    private void listBooks() {
        System.out.println("****************************************************");
        System.out.println("Book name,\tAuthor,\t,Year of Printing,\t Book ID");
        for (Book book : getBooks()) {
            System.out.println(book.getName() + "\t\t" + book.getAuthor() + "\t\t" +
                    book.getYearOfPrinting() + "\t\t" + book.getBookID());
        }
        System.out.println("****************************************************");
    }

    /**
     * Gorevli menusu ekrana basilir.
     */
    private void printStaffMenu() {
        System.out.println("######################STAFF MENU######################");
        System.out.println("What do you want to do?");
        System.out.println("You can enter the chic number.");
        System.out.println("1) Add new member.  ");
        System.out.println("2) Remove member. ");
        System.out.println("3) Add new book.");
        System.out.println("4) Remove book.");
        System.out.println("5) List users.");
        System.out.println("6) List books.");
        System.out.println("10) Exit.");
        System.out.println("######################################################");
    }

    /**
     * Kullanici menusu ekrana basilir.
     */
    private void printUserMenu() {
        System.out.println("######################USER MENU######################");
        System.out.println("What do you want to do?");
        System.out.println("You can enter the chic number.");
        System.out.println("1) Barrow a book.");
        System.out.println("2) Return a book.");
        System.out.println("3) List books");
        System.out.println("4) List barrowed books.");
        System.out.println("10) Exit.");
        System.out.println("######################################################");
    }

    /**
     * Kullanicinin odunc aldigi kitaplar listelenir.
     */
    private void listBorrowedBooks() {
        System.out.println("Book name,\tAuthor,\tYear of Printing,\tBook ID,\tBorrow Status,\tBarrowed User");
        for (Book book : getBooks()) {
            if (book.isBarrowStatus()) {
                System.out.println(book.toString());
            }
        }
    }

    /**
     * Sistem kayitli dosyadan acilir.
     */
    private boolean loadSystemFromFile(){
        if( getCsvFileReader().readStaffCsvFile(this) &&
                getCsvFileReader().readUserCsvFile(this) &&
                getCsvFileReader().readBooksCsvFile(this) ){
            System.out.println("System loaded from files");
            return true;
        }
        return false;
    }
}
