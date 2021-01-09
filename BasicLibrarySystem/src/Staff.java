/**
 * Created by Mehmet Gürol Çay on 20.02.2017.
 */
public class Staff implements Personal {

    private String userName;
    private String password;
    private LibrarySystem system;

    /**
     * One parameter constructor
     * Composition yapabilmek için bir parametre alan constructor LibrarySystem objesi alır.
     * @param system    LibrarySystem objesidir. Bu obje sayesinde ana sistemdeki tüm gelismelerden staff class'inin
     *                  haberi olur.
     */
    public Staff(LibrarySystem system){
        userName = new String();
        password = new String();
        this.system = system;
    }

    /**
     * Uc parametreli constructor yeni bir kullanici olusturulacagi zaman kullanilir.
     * @param userName      Kullanici adi
     * @param password      Kullanici parolasi
     * @param system        LibrarySystem objesi. Bu obje sayesinde Staff objesi ana sistemden haberbar olur.
     */
    public Staff(String userName, String password, LibrarySystem system) {
        this.userName = userName;
        this.password = password;
        this.system = system;
    }

    /**
     * yeni bir kitao eklenmek istendiginde kullanılır.
     * @param newBook       Eklenmek istenen kitabin objesi
     * @return              eger kitap eklenemez ise false dondurulur, kitap eklenirse true dondurur
     */
    public boolean addBook(Book newBook) {
        try{
            getSystem().getBooks().add(newBook);
        }catch (Exception e){
            return false;
        }

        return true;
    }

    /**
     * bir kitap kaldirilmak istenirse kullanilir.
     * @param bookID        kaldırılmak istenen kitabin essiz ID'si
     * @return              kitap dogru bir sekilde silinirse true doner, eger kitap silinemez ise false doner.
     */
    public boolean removeBook(int bookID) {
        for (Book obj : getSystem().getBooks()) {
            if (bookID == obj.getBookID()){
                getSystem().getBooks().remove(obj);
                return true;
            }
        }
        return false;
    }

    /**
     * yeni bir kullanici eklenmek istenirse bu metot kullanilir.
     * @param userName      yeni eklenmek istenen kullanicinin adi
     * @param password      yeni eklenmek istenen kullanicinin parolasi
     * @throws MyException  eger kullanici ismi ayni ise bu kullanici zaten mevcut diye exception fırlatır.
     */
    @Override
    public void addUser(String userName, String password) throws MyException{

        if(getSystem().getUsers().isEmpty())
            getSystem().getUsers().add(new User(userName,password,getSystem()));
        else{
            for (User user : getSystem().getUsers()) {
                if(!user.getUserName().equals(userName)){
                    getSystem().getUsers().add(new User(userName,password,getSystem()));
                    break;
                } else {
                    throw new MyException("This username already taken!");
                }
            }
        }
    }

    /**
     * Bir kullanici silmek istendigi zaman kullanilir.
     * @param userName      sistemden silinmek istenen kullanici adi
     * @throws MyException  eger sistemde hic kullanici yoksa veya o kullanici isminde kullanici sistemde kayitli
     * degilse exception fırlatılır.
     */
    @Override
    public void removeUser(String userName) throws MyException{
        if(getSystem().getUsers().isEmpty())
            throw new MyException("There is no exist any user!");
        else{
            for (User user : getSystem().getUsers()) {
                if (user.getUserName().equals(userName)) {
                    getSystem().getUsers().remove(getSystem().getUsers().indexOf(user));
                    break;
                }else{
                    throw new MyException("There is no exist username");
                }
            }
        }

    }

    /**
     * This is stub method.
     * @param bookID    odunc alinmak istenen kitabin essiz numarasi
     * @return          kitap dogru bir sekilde odunc alinabilirse true doner, donmez ise false doner.
     */
    @Override
    public boolean barrowBook(int bookID) {
        //STUB METHOD
        return false;
    }

    /**
     * This is stub method.
     * @param bookID    iade edilmek istenen kitabın essiz numarasi
     * @return          eger kitab dogru bi sekilde iade edilirse true, edilmez ise false donecektir.
     */
    @Override
    public boolean returnBook(int bookID) {
        //STUB METHOD
        return false;
    }

    /**
     * kullanici adini dondurur
     * @return
     */
    @Override
    public String getUserName() {
        return userName;
    }

    /**
     * kullanici parolasini dondurur
     * @return
     */
    @Override
    public String getPassword() {
        return password;
    }

    /**
     * sistemin objesini dondurur
     * @return
     */
    public LibrarySystem getSystem() {
        return system;
    }
}
