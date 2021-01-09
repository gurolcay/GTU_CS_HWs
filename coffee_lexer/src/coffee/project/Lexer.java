package coffee.project;

import coffee.IdentifierList;
import coffee.REPL;
import coffee.TokenList;
import coffee.datatypes.*;
import coffee.syntax.Keywords;
import coffee.syntax.Operators;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by ft on 10/14/15.
 * Changed by Mehmet Gurol Cay on 10/30/2016
 */
public class Lexer implements REPL.LineInputCallback {
    private TokenList tokenList = TokenList.getInstance();
    private IdentifierList identifierList = IdentifierList.getInstance();


    @Override
    public String lineInput(String line) {
        System.out.println(line);
        String[] stringTokens = line.split(" ");
        int i = 0;
        for (int index = 0; index < stringTokens.length; ++index) {
            while (i < stringTokens[index].length()) {
                if (checkOperator(stringTokens[index], i)) {
                    //add operator
                    stringTokens[index] = removeParanthesis(stringTokens[index], i);
                } else if (checkKeyword(stringTokens[index])) {
                    //add keywordlist
                    break;
                } else {
                    //add identifier
                    if (checkAndAddIndentifier(stringTokens[index])) {
                        //identifierList.addIdentifier(stringTokens[index]);
                        break;
                    } else {
                        try {
                            int integer = Integer.parseInt(stringTokens[index]);
                            tokenList.addToken(new ValueInt(integer));
                        } catch (NumberFormatException e) {
                        }
                        break;
                    }
                }
            }
        }
        return line;
    }

    private boolean checkOperator(String string, int i) {
        if (string.charAt(i) == '(') {
            tokenList.addToken(new Operator(Operators.LEFT_PARENTHESIS));
            return true;
        } else if (string.charAt(i) == ')') {
            tokenList.addToken((new Operator(Operators.RIGHT_PARENTHESIS)));
            return true;
        } else if (string.charAt(i) == '+') {
            tokenList.addToken(new Operator(Operators.PLUS));
            return true;
        } else if (string.charAt(i) == '-') {
            tokenList.addToken(new Operator(Operators.MINUS));
            return true;
        } else if (string.charAt(i) == '/') {
            tokenList.addToken(new Operator(Operators.SLASH));
            return true;
        } else if (string.charAt(i) == '*') {
            tokenList.addToken((new Operator(Operators.ASTERISK)));
            return true;
        } else
            return false;
    }

    private boolean checkAndAddIndentifier(String string) {
        List<Character> token = new ArrayList();
        StringBuilder identi = new StringBuilder();
        int number = 0;
        boolean idFlag = false;
        boolean numFlag = false;
        for (int i = 0; i < string.length(); ++i) {
            if (Character.isLetter(string.charAt(i))) {
                identi.append(string.charAt(i));
                if (!idFlag) {
                    token.add('i');
                    idFlag = true;
                }
            } else if (string.charAt(i) == '(' || string.charAt(i) == ')') {
                token.add(string.charAt(i));
            } else if (Character.isDigit(string.charAt(i))){
                number = Integer.parseInt(new StringBuilder().append(string.charAt(i)).toString());
                if (!numFlag) {
                    token.add('n');
                    numFlag = true;
                }
            }

        }

        for (int i = 0; i < token.size(); ++i) {
            if (token.get(i) == '(') {
                tokenList.addToken(new Operator(Operators.LEFT_PARENTHESIS));
            } else if (token.get(i) == ')') {
                tokenList.addToken(new Operator(Operators.RIGHT_PARENTHESIS));
            } else if ((token.get(i) == 'i') && idFlag && !numFlag) {
                if (!checkKeyword(identi.toString())) {
                    identifierList.addIdentifier(identi.toString());
                    tokenList.addToken(new Identifier(identi.toString()));
                }
            } else if((token.get(i) == 'n') && numFlag && !idFlag){
                tokenList.addToken(new ValueInt(number));
            }
        }
        return (idFlag || numFlag);
    }

    private String removeParanthesis(String string, int index) {
        StringBuilder stringBuilder = new StringBuilder(string);
        stringBuilder.deleteCharAt(index);
        return stringBuilder.toString();
    }

    private boolean checkKeyword(String string) {
        if (string.equals(Keywords.AND)) {
            tokenList.addToken(new Keyword(Keywords.AND));
            return true;
        } else if (string.equals(Keywords.OR)) {
            tokenList.addToken(new Keyword(Keywords.OR));
            return true;
        } else if (string.equals(Keywords.NOT)) {
            tokenList.addToken(new Keyword(Keywords.NOT));
            return true;
        } else if (string.equals(Keywords.EQUAL)) {
            tokenList.addToken(new Keyword(Keywords.EQUAL));
            return true;
        } else if (string.equals(Keywords.APPEND)) {
            tokenList.addToken(new Keyword(Keywords.APPEND));
            return true;
        } else if (string.equals(Keywords.CONCAT)) {
            tokenList.addToken(new Keyword(Keywords.CONCAT));
            return true;
        } else if (string.equals(Keywords.SET)) {
            tokenList.addToken(new Keyword(Keywords.SET));
            return true;
        } else if (string.equals(Keywords.DEFFUN)) {
            tokenList.addToken(new Keyword(Keywords.DEFFUN));
            return true;
        } else if (string.equals(Keywords.FOR)) {
            tokenList.addToken(new Keyword(Keywords.FOR));
            return true;
        } else if (string.equals(Keywords.WHILE)) {
            tokenList.addToken(new Keyword(Keywords.WHILE));
            return true;
        } else if (string.equals(Keywords.IF)) {
            tokenList.addToken(new Keyword(Keywords.IF));
            return true;
        } else if (string.equals(Keywords.THEN)) {
            tokenList.addToken(new Keyword(Keywords.THEN));
            return true;
        } else if (string.equals(Keywords.ELSE)) {
            tokenList.addToken(new Keyword(Keywords.ELSE));
            return true;
        } else if (string.equals(Keywords.TRUE)) {
            tokenList.addToken(new ValueBinary(true));
            return true;
        } else if (string.equals(Keywords.FALSE)) {
            tokenList.addToken(new ValueBinary(false));
            return true;
        } else
            return false;
    }
}