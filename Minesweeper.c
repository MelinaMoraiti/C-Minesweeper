#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <time.h>
int newScreen(int**,int , int ,int );
void play(int** ,int** ,int ,int ,int);
int saveInTxt(const char* ,const char* ,int** ,int ,int );
void addNumber(int** , int ,int );
void printMat(int**,int ,int ,FILE*);
int count_nearby_mines(int** ,int , int ,int , int );
int GetIntWithinLimits(char* message,int, int);
int **Dynamic_2d (int ,int );
int GetRandom (int , int );
void fillMat (int**,int ,int ,int);
void put_mines (int **,int,int,int);
void free_2d(int**,int);
void clear(void);

int main(int argc, char *argv[]) 
{  
    int M,N,K;
	int **Minefield;
	int **ScreenMinefield;
	FILE* outFile;
	M = GetIntWithinLimits("Enter number of rows (maximum 35): ",1,35);
	N = GetIntWithinLimits("Enter number of columns (maximum 35): ",1,35);
    K = GetIntWithinLimits("Enter number of bombs: ",0,M*N);
    clear();
    
	Minefield = Dynamic_2d(M,N);
	ScreenMinefield = Dynamic_2d(M,N);
	fillMat(Minefield,M,N,' '); 
	fillMat(ScreenMinefield,M,N,219); 
	put_mines(Minefield,M,N,K);
	addNumber(Minefield,M,N);
	if( ! saveInTxt("Cheat Sheet.txt","w",Minefield,M,N)) printf("Can't open file.\n");
	play(Minefield,ScreenMinefield,M,N,K); 
	free_2d(Minefield,M);
	free_2d(ScreenMinefield,M);
	return 0;
}
int saveInTxt(const char* Filename,const char* Mode,int** M,int Rows,int Cols)
{
	FILE* outFile;
    outFile = fopen(Filename,Mode);
	if (outFile == NULL) 
	{
		return 0;
	}
    printMat(M,Rows,Cols,outFile);
	fclose(outFile);
	return 1;
}

int GetIntWithinLimits (char* message,int l1,int l2)
{
    int P;
    do
    {
		printf("%s",message);
        scanf ("%d", &P);
    }
    while (P < l1 || P > l2);
    return P;
}
void clear(void)
{
	system("pause");
	#ifdef _WIN32
        system("cls");
	#else 
        system("clear");    
	#endif
	
}
int **Dynamic_2d (int R,int C)
{
	int **Mat2D,i;
	Mat2D = (int **) malloc (R * sizeof(int *));
	if ( Mat2D == NULL)
	{
		puts("Not enough memory.\n");
	  	exit(1);
	}
	for (i=0;i < R;i++)
	{
	  Mat2D[i] = (int *) malloc(C * sizeof(int));
	  if (Mat2D[i] == NULL)
	  {
	  	puts("Not Enough memory.\n");
	  	exit(1);
 	  }	  
    }
	return Mat2D;
}

void fillMat (int **Board,int R,int C,int num)
{   
    int i,j;
	for (i=0;i<R;i++)
	{ 
		for (j=0;j<C;j++)
		{
			Board[i][j] = num;	
		}
	}	
}

void addNumber(int** Mat, int rows,int cols)
{
	int i,j,nob;
	for (i = 0;i < rows;i++)                                   
    {
        for (j = 0;j < cols;j++)
        {
        	if (Mat[i][j] == ' ')
			{
				nob = count_nearby_mines(Mat,i,j,rows,cols);
				if (nob) Mat[i][j] = nob;
			}
        	
		}
	}
}
void put_mines (int **Board,int R,int C,int B)
{
	int x,i,j;
	srand(time(NULL));
	for (x=0;x<B;x++)
	{
		i=rand()%R ;
		j=rand()%C ;
	    if (Board[i][j] !='*')
		{			
		   Board[i][j] ='*';
		}
		else x--;		
	}
}
int count_nearby_mines(int** Mat,int line, int col,int lines, int cols)
{
	int i,j;
	int bombs=0;
    for (i = line - 1; i <= line + 1; i++)
    {
        for (j = col - 1; j <= col + 1; j++)
        {
          if (j >= 0 && i >= 0 && i < lines && j < cols ) 
          {
           if (i!=line || j!=col)
		   {
			 if (Mat[i][j] == '*') bombs++;
		   }
	      }
        }
    }
    return bombs;
}

void printMat(int** mat,int r,int c,FILE* fp)
{
	int i,j;
	fprintf(fp,"\n");
	for (i = 0;i < c;i++) fprintf(fp," %4d",i+1);
	fprintf(fp,"\n");
    for (i = 0;i < r;i++)
	{
		fprintf(fp,"\n");
		fprintf(fp,"%2d", i+1);
       for (j = 0;j < c;j++)
	   {
	   	  fprintf(fp," ");
	   	  if(mat[i][j] < 9 && mat[i][j] > 0)
	   	  {
	   	  	fprintf(fp," [%d]",mat[i][j]);
		  }
		  else fprintf(fp," [%c]",mat[i][j]);
	   }

	}
}

void play(int** realMat,int** screenMat,int rows,int cols,int bombs)
{
	int i,j,square,rounds=0,safeSquares;
	safeSquares=rows*cols - bombs;
	do
	{
		clear();
		printMat(screenMat,rows,cols,stdout);
		puts("\n\nChoose a square");
		i = GetIntWithinLimits("Line: ",1,rows);
		j = GetIntWithinLimits("Column: ", 1,cols);
		square = realMat[i-1][j-1] ;
		if(newScreen(screenMat,square,i-1,j-1)) rounds++;
		if (rounds == safeSquares)	
		{
			clear();
			printf("\n            YOU WON ! \n");
			printMat(realMat,rows,cols,stdout);
		}
		if (square == '*')
		{
			clear();
			puts("\n              YOU LOST :(");
			printMat(realMat,rows,cols,stdout);
		}
		
	} while (square != '*' &&  rounds != safeSquares);
	
}

int newScreen(int** scrMat,int new_square ,int scr_i,int scr_j)
{
	if(scrMat[scr_i][scr_j]  != new_square)
	{
		scrMat[scr_i][scr_j]  = new_square;
		return 1;
	}
	else  return 0;
	 
}


void free_2d (int **Board,int R)
{
	int i;
	for (i=0;i< R;i++) free (Board[i]);
	free (Board);
}
	
	



