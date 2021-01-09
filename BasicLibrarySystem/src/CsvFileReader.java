import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingDeque;

/**
 * Created by Mehmet Gürol Çay on 26.02.2017.
 */
public class CsvFileReader {
    private static final String COMA_DELIMETER = ",";
    private static final String USER_RECORD_FILE_NAME = "usersRecord.csv";
    private static final String STAFF_RECORD_FILE_NAME = "staffsRecord.csv";
    private static final String BOOKS_RECORD_FILE_NAME = "booksRecord.csv";

    /**
     * Sistem baslangicinda User'larin kayitli oldugu dosyati okuyarak olusturulmus sistem listesine
     * kayit yapilir.
     * @param system    program balagincinda olusturulan sistem. bunu parametre olarak alarak
 *                      bu sistem uzerinde degisiklik yapilir.
     * @return          eger sistem dogru okuma yaparsa "true" dosya bos veya dosya mevcut
 *                      degilse "false" doner
     */
    public static boolean readUserCsvFile(LibrarySystem system){
        try {
            BufferedReader reader = new BufferedReader(new FileReader(USER_RECORD_FILE_NAME));

            String line = null;

            try {
                reader.readLine();

                while((line = reader.readLine()) != null){
                    String[] tokens = line.split(COMA_DELIMETER);
                    if (tokens.length > 0 ){
                        User user = new User(tokens[0],tokens[1],system);
                        system.getUsers().add(user);
                    }
                }

                reader.close();
            } catch (IOException e) {
                return false;
            }

        } catch (FileNotFoundException e) {
            return false;
        }

        return true;
    }

    /**
     * Sistem baslangicinda Staff'larin kayitli oldugu dosyati okuyarak olusturulmus sistem listesine
     * kayit yapilir.
     * @param system    program balagincinda olusturulan sistem
     * @return          eger dosya dogru bir sekilde okunursa "true" doner, eger doysa yoksa
     *                  veya bos ise "false" doner
     */
    public static boolean readStaffCsvFile(LibrarySystem system){
        try {
            BufferedReader reader = new BufferedReader(new FileReader(STAFF_RECORD_FILE_NAME));

            String line = null;

            try {
                reader.readLine();

                while((line = reader.readLine()) != null){
                    String[] tokens = line.split(COMA_DELIMETER);
                    if (tokens.length > 0){
                        Staff staff = new Staff(tokens[0],tokens[1],system);
                        system.getStaffs().add(staff);
                    }
                }

                reader.close();
            } catch (IOException e) {
               return false;
            }
        } catch (FileNotFoundException e) {
            return false;
        }

        return true;
    }

    /**
     * Sistem baslangicinda Book'larin kayitli oldugu dosyati okuyarak olusturulmus sistem listesine
     * kayit yapilir.
     * @param system    program balagincinda olusturulan sistem
     * @return          eger dosya dogru okunursa sistem icindeki kitap arraylist'i doldurulur ve
     *                  true doner, eger dosya yoksa veya bos ise false doner
     */
    public static boolean readBooksCsvFile(LibrarySystem system){

        try {
            BufferedReader reader = new BufferedReader(new FileReader(BOOKS_RECORD_FILE_NAME));

            String line = null;

            try {
                reader.readLine();

                while((line = reader.readLine()) != null){
                    String[] tokens = line.split(COMA_DELIMETER);
                    if (tokens.length > 0){
                        Book book = new Book(tokens[0],tokens[1],tokens[2], Boolean.parseBoolean(tokens[3]),
                                                Integer.parseInt(tokens[4]),tokens[5]);
                        if(book.isBarrowStatus()){
                            for (User user : system.getUsers()) {
                                if (book.getUserNameOfBorrower().equals(user.getUserName())){
                                    user.getBarrowedBook().add(book);
                                }

                            }
                        }
                        system.getBooks().add(book);
                    }
                }

            } catch (IOException e) {
                return false;
            }
        } catch (FileNotFoundException e) {
            return false;
        }

        return true;
    }

}
