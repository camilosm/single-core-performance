import java.util.*;

public class MatrixProduct{

	public static void OnMult(int m_ar, int m_br) {
		long time1, time2;

		String st = new String("");
		double temp;
		int i, j, k;

		double[] pha = new double[m_ar*m_ar];
		double[] phb = new double[m_ar*m_ar];
		double[] phc = new double[m_ar*m_ar];

		for(i=0; i<m_ar; i++)
			for(j=0; j<m_ar; j++)
				pha[i*m_ar + j] = (double)1.0;

		for(i=0; i<m_br; i++)
			for(j=0; j<m_br; j++)
				phb[i*m_br + j] = (double)(i+1);

		time1 = System.currentTimeMillis();

		for(i=0; i<m_ar; i++){
			for(j=0; j<m_br; j++){
				temp = 0;
				for( k=0; k<m_ar; k++){	
					temp += pha[i*m_ar+k] * phb[k*m_br+j];
				}
				phc[i*m_ar+j]=temp;
			}
		}

		time2 = System.currentTimeMillis();

		System.out.printf("Time: %3.3f seconds\n", (double)(time2-time1)/1000);
	}

	public static void OnMultLine(int m_ar, int m_br) {
		long time1, time2;

		String st = new String("");
		double temp;
		int i, j, k;

		double[] pha = new double[m_ar*m_ar];
		double[] phb = new double[m_ar*m_ar];
		double[] phc = new double[m_ar*m_ar];

		for(i=0; i<m_ar; i++)
			for(j=0; j<m_ar; j++)
				pha[i*m_ar + j] = (double)1.0;

		for(i=0; i<m_br; i++)
			for(j=0; j<m_br; j++)
				phb[i*m_br + j] = (double)(i+1);
		
		for(i=0; i<m_ar; i++)
			for(j=0; j<m_ar; j++)
				phc[i*m_ar + j] = (double)0;

		time1 = System.currentTimeMillis();

		for(i=0; i<m_ar; i++){
			for( k=0; k<m_ar; k++){	
				for(j=0; j<m_br; j++){
					phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_br+j];
				}
			}
		}

		time2 = System.currentTimeMillis();

		System.out.printf("Time: %3.3f seconds\n", (double)(time2-time1)/1000);
	}



	public static void main(String[] args) {	
		char c;
		int lin, col, blockSize;
		int op;

		long[] values = new long[20];
		int ret;
		Scanner stdin = new Scanner(System.in);

		op=1;
		do{
			System.out.println("1. Multiplication");
			System.out.println("2. Line Multiplication");

			System.out.print("Selection: ");
			op = stdin.nextInt();

			if(op==0) break;

			System.out.print("Dimensions: ");
			lin = stdin.nextInt();
			col = lin;

			switch (op){
				case 1: 
					OnMult(lin, col);
					break;
				case 2:
					OnMultLine(lin, col);  
					break;
			}
		} while (op != 0);
	}

}