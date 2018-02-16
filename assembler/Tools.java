import java.util.regex.Matcher;
import java.util.regex.Pattern;
public class Tools {
    public static int hex2dec(String hex)
    {
        int x = 0;
        int j = 0;
        int size = hex.length();
        for (int i = size - 1; i >= 0; i--)
        {
            x = x + (char2int(hex.charAt(i)) * ((int)Math.pow(16, j)));
            j++;
        }
        return x;
    }

    public static String dec2hex(int dec)
    {
        String hex = Integer.toHexString(dec);
        return hex;
    }
    
    public static int processNumber(String number) {
        Pattern hexPred = Pattern.compile("0x");
        Matcher hexMatcher = hexPred.matcher(number);
        Pattern negPred = Pattern.compile("-");
        Matcher negMatcher= negPred.matcher(number);
        boolean negative = false;
        if(negMatcher.find()) {
          negative = true;
          number = number.substring(1);
        }
        if(hexMatcher.find()) {
            return Tools.hex2dec(number.substring(2)) * (negative? -1: 1);
        } else {
            Pattern numPred = Pattern.compile("\\d+");
            Matcher numMatcher = numPred.matcher(number);
            numMatcher.find();
            return Integer.parseInt(numMatcher.group(0)) * (negative? -1: 1);
        }
    }

    public static int char2int(char num)
    {
        switch(num)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return Character.getNumericValue(num);

        case 'a':
        case 'A':
            return 10;

        case 'b':
        case 'B':
            return 11;

        case 'c':
        case 'C':
            return 12;

        case 'd':
        case 'D':
            return 13;
            
        case 'e':
        case 'E':
            return 14;
            
        case 'f':
        case 'F':
            return 15;
        }
        return -1;
    }
}
