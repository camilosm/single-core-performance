#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <papi.h>

using namespace std;

#define SYSTEMTIME clock_t

void OnMult(int m_ar, int m_br){
	SYSTEMTIME time1, time2;
	
	char st[100];
	double temp;
	int i, j, k;
	double *pha, *phb, *phc;

	pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;


	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

	time1 = clock();

	for(i=0; i<m_ar; i++){
		for( j=0; j<m_br; j++){
			temp = 0;
			for( k=0; k<m_ar; k++){
				temp += pha[i*m_ar+k] * phb[k*m_br+j];
			}
			phc[i*m_ar+j]=temp;
		}
	}

	time2 = clock();

	sprintf(st, "Time: %3.3f seconds\n", (double)(time2 - time1) / CLOCKS_PER_SEC);
	cout << st;

	// display 10 elements of the result matrix to verify correctness
	// cout << "Result matrix: " << endl;
	// for(j=0; j<min(10,m_br); j++)
	// 	cout << phc[j] << " ";
	// cout << endl;

	free(pha);
	free(phb);
	free(phc);
}

// add code here for line x line matriz multiplication
void OnMultLine(int m_ar, int m_br){
	SYSTEMTIME time1, time2;
	
	char st[100];
	int i, j, k;
	double *pha, *phb, *phc;

	pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)calloc((m_ar * m_ar), sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;


	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

	time1 = clock();

	for(i=0; i<m_ar; i++){
		for( k=0; k<m_ar; k++){
			for( j=0; j<m_br; j++){
				phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_br+j];
			}
		}
	}

	time2 = clock();

	sprintf(st, "Time: %3.3f seconds\n", (double)(time2 - time1) / CLOCKS_PER_SEC);
	cout << st;

	// display 10 elements of the result matrix to verify correctness
	// cout << "Result matrix: " << endl;
	// for(j=0; j<min(10,m_br); j++)
	// 	cout << phc[j] << " ";
	// cout << endl;

	free(pha);
	free(phb);
	free(phc);
}

// add code here for block x block matriz multiplication
void OnMultBlock(int m_ar, int m_br, int bkSize){
	SYSTEMTIME time1, time2;
	
	char st[100];
	// double temp;
	int i, j;
	double *pha, *phb, *phc;

	pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)calloc((m_ar * m_ar), sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

	time1 = clock();
	
	for(int jj=0; jj<m_ar; jj+=bkSize){
		for(int kk=0;kk<m_br; kk+=bkSize){
			for(int i=0;i<m_ar;i++){
				for(int k = kk; k<((kk+bkSize)>m_ar?m_ar:(kk+bkSize)); k++){
					for(int j = jj; j<((jj+bkSize)>m_ar?bkSize:(jj+bkSize)); j++){
						phc[i*m_ar+j] +=  pha[i*m_ar+k] * phb[k*m_br+j];
					}
				}
			}
		}
	}

	time2 = clock();

	sprintf(st, "Time: %3.3f seconds\n", (double)(time2 - time1) / CLOCKS_PER_SEC);
	cout << st;

	// display 10 elements of the result matrix to verify correctness
	// cout << "Result matrix: " << endl;
	// for(j=0; j<min(10,m_br); j++)
	// 	cout << phc[j] << " ";
	// cout << endl;

	free(pha);
	free(phb);
	free(phc);
}

void init_papi(){
	int retval = PAPI_library_init(PAPI_VER_CURRENT);
	if(retval < 0){
		if(retval != PAPI_VER_CURRENT)
			printf("PAPI library version mismatch!\n");
		else
			printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
		exit(1);
	}

	cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval) << " MINOR: " << PAPI_VERSION_MINOR(retval) << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}


int main (int argc, char *argv[]){
	if(argc<2 || argc>4){
		printf("%s\n", argv[0]);
		printf("Usage: %s dimension option [blocksize]\n", argv[0]);
		return(1);
	}

	int dimension = stoi(argv[1]);
	int option = stoi(argv[2]);
	int blocksize;
	
	int EventSet = PAPI_NULL;
  	long long values[2] = {-1,-1};
  	int ret;

	ret = PAPI_library_init(PAPI_VER_CURRENT);
	if(ret != PAPI_VER_CURRENT)
		cout << "FAIL" << endl;

	ret = PAPI_create_eventset(&EventSet);
	if(ret != PAPI_OK)
		cout << "ERROR: create eventset" << endl;

	ret = PAPI_add_event(EventSet, PAPI_L1_DCM);
	if(ret != PAPI_OK)
		cout << "ERROR: PAPI_L1_DCM" << endl;

	ret = PAPI_add_event(EventSet, PAPI_L2_DCM);
	if(ret != PAPI_OK)
		cout << "ERROR: PAPI_L2_DCM" << endl;

	// start counting
	ret = PAPI_start(EventSet);
	if(ret != PAPI_OK)
		cout << "ERROR: Start PAPI" << endl;

	switch (option){
		case 1:
			OnMult(dimension, dimension);
			break;
		case 2:
			OnMultLine(dimension, dimension);
			break;
		case 3:
			blocksize = stoi(argv[3]);
			OnMultBlock(dimension, dimension, blocksize);
			break;
		default:
			break;
	}

	ret = PAPI_stop(EventSet, values);
	if(ret != PAPI_OK)
		cout << "ERROR: Stop PAPI" << endl;
	printf("L1 DCM: %lld\n", values[0]);
	printf("L2 DCM: %lld\n", values[1]);

	ret = PAPI_reset(EventSet);
	if(ret != PAPI_OK)
		cout << "FAIL reset" << endl;
	
	// for testing without papi
	exit(0);

	ret = PAPI_remove_event(EventSet, PAPI_L1_DCM);
	if(ret != PAPI_OK)
		cout << "FAIL remove event" << endl;

	ret = PAPI_remove_event(EventSet, PAPI_L2_DCM);
	if(ret != PAPI_OK)
		cout << "FAIL remove event" << endl;

	ret = PAPI_destroy_eventset(&EventSet);
	if(ret != PAPI_OK)
		cout << "FAIL destroy" << endl;
}