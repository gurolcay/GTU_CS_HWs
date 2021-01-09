import java.util.ArrayList;
import java.util.List;

/**
 * Created by Mehmet Gürol Çay on 20.02.2017.
 */
public class User implements Personal {

    private String userName;
    private String password;
    private LibrarySystem system;
    private List<Book> barrowedBook;

    /**
     *
     * @param system
     */
    public User(LibrarySystem system) {
        this.system = system;
        barrowedBook = new ArrayList<>();
    }

    /**
     *
     * @param userName
     * @param password
     */
    public User(String userName, String password, LibrarySystem system) {
        this.userName = userName;
        this.password = password;
        this.system = system;
        barrowedBook = new ArrayList<Book>();
    }

    /**
     *
     */
    @Override
    public boolean barrowBook(int bookID){
        for (Book bookObj : getSystem().getBooks() ) {
            if(bookID == bookObj.getBookID()){
                this.getBarrowedBook().add(bookObj);
                bookObj.setBarrowStatus(true);
                bookObj.setUserNameOfBorrower(getUserName());
                return true;
            }
        }
        return false;
    }

    /**
     *
     */
    @Override
    public boolean returnBook(int bookID){
        for(Book bookObj: getSystem().getBooks()){
            if(bookID == bookObj.getBookID()){
                this.getBarrowedBook().remove(bookObj);
                bookObj.setBarrowStatus(false);
                bookObj.setUserNameOfBorrower("");
                return true;
            }
        }
        return false;
    }

    /**
     *
     * @return
     */
    @Override
    public String getUserName() {
        return userName;
    }

    /**
     *
     * @return
     */
    @Override
    public String getPassword() {
        return password;
    }

    /**
     *
     * @return
     */
    @Override
    public boolean addBook(Book book) throws MyException{
        throw new MyException("User can't add any book!");
    }

    /**
     *
     * @return
     */
    @Override
    public boolean removeBook(int bookID) throws MyException{
        throw new MyException("User can't remove any book!");
    }

    /**
     *
     * @param userName
     * @param password
     */
    @Override
    public void addUser(String userName, String password) throws MyException{
        throw new MyException("User can't add any user!");
    }

    /**
     *
     */
    @Override
    public void removeUser(String userName) throws MyException{
        throw new MyException("User can't remove any user!");
    }

    /**
     *
     * @param userName
     */
    public void setUserName(String userName) {
        this.userName = userName;
    }

    /**
     *
     * @param password
     */
    public void setPassword(String password) {
        this.password = password;
    }

    /**
     *
     * @return
     */
    public List<Book> getBarrowedBook() {
        return barrowedBook;
    }

    /**
     *
     * @param barrowedBook
     */
    public void setBarrowedBook(List<Book> barrowedBook) {
        this.barrowedBook = barrowedBook;
    }

    /**
     *
     * @return
     */
    public LibrarySystem getSystem() {
        return system;
    }
}
