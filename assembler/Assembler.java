import java.util.HashMap;
import java.util.ArrayList;
import java.util.regex.*;
import java.io.FileReader;
import java.io.BufferedReader;

public class Assembler {
  
  public static void main(String[] args) 
  {
    
  }
  
  public int hex2dec(String hex, int size)
	{
		int x = 0;
		int j = 0;
		for (int i = size - 1; i >= 0; i--)
		{
			x = x + (char2int(hex.charAt(i)) * ((int)Math.pow(16, j)));
			j++;
		}
		return x;
	}

	public String dec2hex(int dec)
	{
		String hex = Integer.toHexString(dec);
		return hex;
	}

	public int char2int(char num)
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

			default:
				return -1;
		}
	}	
}
}

