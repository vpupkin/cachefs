/* fopen example */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int LINE_MAX=200;

static int step=100;
static int size=100;

struct file {
   int acc;
   char filename[100];
};

int no=0, i;
struct file *vect;

int find(char *str)
{
  int i;

  for (i=0; i<no; i++)
  if ( !strcmp(vect[i].filename, str) ) return i;

  return -1;   
}

int main ()
{
  FILE * pFile= fopen ("dump.txt","r");
  FILE * file_all= fopen ("files_all.txt","w");
  FILE * file_stat= fopen ("files_stat.txt","w");
  
  char *line, *str;
  char *pch, *pch2;

  int pos;

  //init vect
  vect= (struct file*) malloc( size * sizeof(struct file) );

  line =(char *) malloc( LINE_MAX * sizeof(char) );
  str  =(char *) malloc( LINE_MAX/2 * sizeof(char) );

  if (pFile!=NULL && file_all!=NULL && file_stat!=NULL)
  {
    while ( fgets(line, 200, pFile) != NULL ) 
    {
	if ( strstr(line, "open") == NULL ) continue;

	//filename
	pch=  strchr( line, '\"');
        pch2= strchr( pch+1, '\"');
	strncpy ( str, pch+1, pch2-pch-1 );
        str[pch2-pch-1]='\0'; 
	if ( str[0]!='/' ) continue;
        fprintf( file_all, "%s - ", str );
	
	//no of accesses of file str
        pos = find( str );	
        if (pos>=0) vect[pos].acc++;
        else 
	{
           vect[no].acc= 1;
           strcpy( vect[no].filename, str ); 

           no++;
	   if (no>=size) 
	   {
 	      size+= step;
            
	      //init vect
              vect=(struct file*) realloc( vect, size * sizeof(struct file) );
	   }
        }
	
	//timestamp
 	pch= strchr( line, ':');
	strncpy ( str, pch-2, pch-line+2 );
        str[pch-line]='\0';
	fprintf( file_all, "%s\n", str ); 

    }

    printf("We have %d files\n", no);
    for (i=0; i<no; i++)
    fprintf(file_stat, "%s %d\n", vect[i].filename, vect[i].acc );

    fclose (pFile);
    fclose (file_all);
    fclose (file_stat);
  }
  else printf("Cannot open file");

  return 0;
}
