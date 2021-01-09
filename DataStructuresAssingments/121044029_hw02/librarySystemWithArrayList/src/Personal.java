/**
 * Created by Mehmet Gürol Çay on 20.02.2017.
 */
public interface Personal {

    public String getUserName();
    public String getPassword();

    public boolean addBook(Book newBook) throws MyException;
    public boolean removeBook(int bookID)throws MyException;

    public void addUser(String userName, String password) throws MyException;
    public void removeUser(String userName) throws MyException;

    public boolean barrowBook(int bookID);
    public boolean returnBook(int bookID);
}
