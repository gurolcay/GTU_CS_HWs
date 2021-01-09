/**
 * LibrarySystem icerisindeki kitaplarin bilgilerinin ve bu kitaplarla alakalı metotlarin yer aldigi class'dir.
 *
 *
 * Created by Mehmet Gürol Çay on 20.02.2017.
 */
public class Book {

    private String name;
    private String author;
    private String yearOfPrinting;
    private boolean barrowStatus;
    private int bookID;
    private static int createdObjecNumber = 0;
    private String userNameOfBorrower;

    /**
     * Bu constructor ile yeni bir kitap objesi olusturulur.
     *
     * @param name              kitap ismi tutulur
     * @param author            kitap yazari tutulur
     * @param yearOfPrinting    kitap yayın tarihi tutulur
     */
    public Book(String name, String author, String yearOfPrinting) {
        this.name = name;
        this.author = author;
        this.yearOfPrinting = yearOfPrinting;
        this.barrowStatus = false;
        this.userNameOfBorrower = "noOne";
        bookID = createdObjecNumber;
        createdObjecNumber += 1;
    }

    /**
     * bu constructor ile yeni bir kitap objesi olusturulur.
     * @param name                  kitap ismi tutulur
     * @param author                kitap yazarinin ismi
     * @param yearOfPrinting        kitap yayin tarihi
     * @param barrowStatus          kitabin herhangi bir kullanici tarafindan odunc alinip alinmadigini tutar
     * @param bookID                herbir kitaba ozel essiz ID tutulur
     * @param userNameOfBorrower    eger kitap birisi tarafindan odunc alindiysa o kisinin ismi burda tutulur,
     *                              birisi tarafindan alinmadiysa "noOne" olarak tutulur
     */
    public Book(String name, String author, String yearOfPrinting, boolean barrowStatus,
                    int bookID, String userNameOfBorrower) {
        this.name = name;
        this.author = author;
        this.yearOfPrinting = yearOfPrinting;
        this.barrowStatus = barrowStatus;
        this.bookID = bookID;
        this.userNameOfBorrower = userNameOfBorrower;
    }

    /**
     * kitap isminin kullanilmasini saglar
     * @return kitap ismini string olarak dondurur
     */
    public String getName() {
        return name;
    }

    /**
     * kitap isminin degistirilmesini saglar
     * @param name      yeni kitap ismidir. bu eski kitap ismi yerine kaydedilir.
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * kitap yazarinin adini verir
     * @return          kitap yazarinin ismini string olarak dondurur
     */
    public String getAuthor() {
        return author;
    }

    /**
     * kitap yazarinin isminin guncellenmesi icin kullanilir.
     * @param author    kitabın yeni yazarinin adidir. yeni kitap yazari eski kitap yazari yerine kaydedilir
     */
    public void setAuthor(String author) {
        this.author = author;
    }

    /**
     * kitap basim tarihini almak icin kullanilir
     * @return          kitap yilini string olarak dondurur
     */
    public String getYearOfPrinting() {
        return yearOfPrinting;
    }

    /**
     * kitap basim tarihini yeniden girmek icin kullanilir.
     * @param yearOfPrinting    kitabin yeni basim tarihidir. eskisi yerine kaydedilir.
     */
    public void setYearOfPrinting(String yearOfPrinting) {
        this.yearOfPrinting = yearOfPrinting;
    }

    /**
     * kitabin herhangi bir kullanici tarafindan odunc alinip alinmadigi bu metot ile kontrol edilir.
     * @return  kitabın odunc verilip verilmedigi durumunu dondurur.
     */
    public boolean isBarrowStatus() {
        return barrowStatus;
    }

    /**
     * kitabin odunc verilme durumunun guncellenmesi icin kullanilir
     * @param barrowStatus      kitap eger odunc alindiysa "true" odunc alinmadiysa "false" olarak kaydedilir.
     */
    public void setBarrowStatus(boolean barrowStatus) {
        this.barrowStatus = barrowStatus;
    }

    /**
     * her bir kitaba verilen essiz ID'nin verilmesini saglar
     * @return  kac adet kitap objesi olusturuldugunu gosterir
     */
    public static int getCreatedObjecNumber() {
        return createdObjecNumber;
    }

    /**
     * Kitap birisi tarafindan odunc alinmis ise bu isim return edilir odunc alinmadiysa "noOne" return edilir
     * @return  kitabı odunc alan kisi return edilir
     */
    public String getUserNameOfBorrower() {
        return userNameOfBorrower;
    }

    /**
     * kitabi odunc alan kisinin ismi set edilir eski isim yerine yeni isim yazilir.
     * @param userNameOfBorrower    kitabi yeni odunc alan kisinin ismi
     */
    public void setUserNameOfBorrower(String userNameOfBorrower) {
        this.userNameOfBorrower = userNameOfBorrower;
    }

    /**
     * kitaba ozel essiz ID dondurulur
     * @return essiz kitap ID'sini dondurur
     */
    public int getBookID() {
        return bookID;
    }

    /**
     * essis kitap ID'sinin degistirilmesini saglar
     * @param bookID    yeni essiz kitap id'si
     */
    public void setBookID(int bookID) {
        this.bookID = bookID;
    }

    /**
     * Book class'inin elemanlarinin ekrana formatli bir sekilde basilmasini saglar
     * @return  formatli sekilde string degeri dondurur. book class'ndaki elemanlar vardir.
     */
    @Override
    public String toString() {

        String bookInfo = new String();

        bookInfo = getName() + "\t" + getAuthor() + "\t" + getYearOfPrinting() +
                "\t" + getCreatedObjecNumber() + "\t" + isBarrowStatus();
        if(barrowStatus)
            bookInfo = bookInfo + "\t" + getUserNameOfBorrower();

        return bookInfo;
    }
}
