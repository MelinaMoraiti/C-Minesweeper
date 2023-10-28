#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//The safeFree macro calls the saferFree function and explicitly casts the pointer type to void when we call the function
#define safeFree(p) saferFree((void**)&(p))

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
	fillMat(ScreenMinefield,M,N,'X'); 
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
// Function to safely read an integer from a file stream
int getInt(char* buffer, FILE* instream)
{
	char* stringFound;
	int number;
	do 
	{
		fgets(buffer,sizeof(buffer),instream);
		*(buffer + strlen(buffer) - 1) = 0;
     	number = strtol(buffer,&stringFound,10);	
	} while(strlen(stringFound) > 0);
	return number;
}

int GetIntWithinLimits (char* message,int l1,int l2)
{
    int P;
    char* buffer = (char*)calloc(10,sizeof(char));
    do
    {
	printf("%s",message);
        P = getInt(buffer,stdin);  
    }
    while (P < l1 || P > l2);
    free(buffer);
    return P;
}
// Function to clear the console screen (cross-platform)
void clear(void)
{
	#ifdef _WIN32
	 system("pause");
         system("cls");
	#else 
         system("clear");    
	#endif
}
// Function to dynamically allocate a 2D array
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
// Function to fill a 2D array with a specified value
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
// Function to add numbers to the minefield based on nearby mines
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
// Function to place mines randomly on the minefield
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
// Function to count the number of nearby mines for a cell
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
    // Print the column headers
	fprintf(fp, "    ");
	for (i = 0;i < c;i++) fprintf(fp,"%2d   ",i+1);
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
	fprintf(fp,"\n");
}
// Function to manage the game logic
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
// Function to update the screen display with the revealed square
int newScreen(int** scrMat,int new_square ,int scr_i,int scr_j)
{
	if(scrMat[scr_i][scr_j]  != new_square)
	{
		scrMat[scr_i][scr_j]  = new_square;
		return 1;
	}
	else  return 0;
	 
}
// Function to safely free a pointer
// Checks the pointer passed to see whether it is NULL and sets the pointer to NULL before it returns.
void saferFree(void **ptr) {
	if (ptr != NULL && *ptr != NULL) 
	{
		free(*ptr);
		*ptr = NULL;
	}
}
void free_2d (int **Board,int R)
{
	int i;
	for (i=0;i< R;i++) safeFree (Board[i]);
	safeFree (Board);
}
	
	





