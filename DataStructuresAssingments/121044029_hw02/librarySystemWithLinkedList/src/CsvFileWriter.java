
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Mehmet Gürol Çay on 20.02.2017.
 */
public class CsvFileWriter {
    private static final String COMA_DELIMETER = ",";
    private static final String NEW_LINE_DELIMETER = "\n";
    private static final String USER_RECORD_FILE_NAME = "usersRecord.csv";
    private static final String STAFF_RECORD_FILE_NAME = "staffsRecord.csv";
    private static final String BOOKS_RECORD_FILE_NAME = "booksRecord.csv";

    private static final String USER_FILE_HEADER = "userName,password";
    private static final String STAFF_FILE_HEADER = "staffName,password";
    private static final String BOOKS_FILE_HEADER = "bookName,author,yearOfPrinting,barrowStatus,bookID,nameOfBorrower";

    /**
     * sistem icindeki kullanicilarin userRecord.csv dosyasina kaydinin yapilmasini saglar.
     * @param users     librarysystem icinde list olarak tutulan kullanicilar.
     */
    public static void writeUsersToCsvFile(List<User> users){
        try {
            FileWriter writer = new FileWriter(USER_RECORD_FILE_NAME);
            writer.append(USER_FILE_HEADER);
            writer.append(NEW_LINE_DELIMETER);

            for (User user : users) {
                writer.append(user.getUserName());
                writer.append(COMA_DELIMETER);
                writer.append(user.getPassword());
                writer.append(NEW_LINE_DELIMETER);

            }
            writer.close();
        } catch (IOException e) {
            System.err.println(e);
        }

    }

    /**
     * sistem icindeki gorevlilerin staffRecord.csv dosyasina kaydinin yapilmasini saglar.
     * @param staffs    librarysystem icinde list olarak tutulan gorevliler.
     */
    public static void writeStaffsToCsvFile(List<Staff> staffs){
        try {
            FileWriter writer = new FileWriter(STAFF_RECORD_FILE_NAME);
            writer.append(STAFF_FILE_HEADER);
            writer.append(NEW_LINE_DELIMETER);
            for (Staff staff : staffs) {
                writer.append(staff.getUserName());
                writer.append(COMA_DELIMETER);
                writer.append(staff.getPassword());
                writer.append(NEW_LINE_DELIMETER);
            }
            writer.close();
        } catch (IOException e) {
            System.err.println(e);
        }

    }

    /**
     * sistem icindeki kitaplarin booksRecord.csv dosyasina kaydinin yapilmasini saglar.
     * @param books     librarysystem icinde list olarak tutulan kitaplar.
     */
    public static void writeBooksToCsvFile(List<Book> books){
        try {
            FileWriter writer = new FileWriter(BOOKS_RECORD_FILE_NAME);
            writer.append(BOOKS_FILE_HEADER);
            writer.append(NEW_LINE_DELIMETER);
            for (Book book : books) {
                writer.append(book.getName());
                writer.append(COMA_DELIMETER);
                writer.append(book.getAuthor());
                writer.append(COMA_DELIMETER);
                writer.append(book.getYearOfPrinting());
                writer.append(COMA_DELIMETER);
                writer.append(String.valueOf(book.isBarrowStatus()));
                writer.append(COMA_DELIMETER);
                writer.append(String.valueOf(book.getBookID()));
                writer.append(COMA_DELIMETER);
                writer.append(book.getUserNameOfBorrower());
                writer.append(NEW_LINE_DELIMETER);
            }
            writer.close();
        } catch (IOException e) {
            System.err.println(e);
        }
    }
}
