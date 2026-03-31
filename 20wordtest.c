#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_TEXT 2000

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main()
{
    char *word_bank[]={
        "the","and","have","that","for","you","with","say","this","they",
        "but","his","from","not","she","what","their","can","who","get",
        "would","her","make","about","know","will","one","time","there","year",
        "think","when","which","them","some","people","take","out","into","just",
        "see","him","your","come","could","now","than","like","other","how",
        "then","its","our","two","more","these","want","way","look","first",
        "also","new","because","day","use","no","man","find","here","thing"
    };

    int word_bank_size=
    sizeof(word_bank)/sizeof(word_bank[0]);

    char random_sentence[MAX_TEXT]="";

    int num_words=20;

    int unique_indices[30];

    int count=0;


    srand(time(NULL));

    while(count<num_words)
    {
        int idx=rand()%word_bank_size;

        int duplicate=0;

        for(int j=0;j<count;j++)
        {
            if(unique_indices[j]==idx)
            {
                duplicate=1;
                break;
            }
        }

        if(!duplicate)
        unique_indices[count++]=idx;
    }

    for(int i=0;i<num_words;i++)
    {
        strncat(random_sentence,
        word_bank[unique_indices[i]],
        MAX_TEXT-strlen(random_sentence)-1);

        if(i<num_words-1)
        strncat(random_sentence," ",
        MAX_TEXT-strlen(random_sentence)-1);
    }

    char input[MAX_TEXT];

    clearScreen();

    printf("===== Typing Speed Test =====\n\n");

    printf("Type the following sentence:\n\n");

    int line_len=0;

    int max_line=60;

    for(int i=0;i<num_words;i++)
    {
        int word_len=
        strlen(word_bank[unique_indices[i]]);

        if(line_len+word_len>max_line)
        {
            printf("\n");
            line_len=0;
        }

        printf("%s",
        word_bank[unique_indices[i]]);

        line_len+=word_len;

        if(i<num_words-1)
        {
            printf(" ");
            line_len++;
        }
    }

    printf("\n\nPress ENTER when ready...");
    getchar();

    printf("\nStart typing (60 sec):\n\n");

    time_t start,end;

    int timed_out=0;

    int pos=0;

    start=time(NULL);

    while(1)
    {
        if(difftime(time(NULL),start)>=60)
        {
            timed_out=1;
            break;
        }

        int ch=getchar();

        if(ch=='\n')
        break;

        if(ch==8 || ch==127)
        {
            if(pos>0)
            {
                pos--;
                printf("\b \b");
            }
            continue;
        }

        if(pos<MAX_TEXT-1)
        {
            input[pos++]=ch;
            putchar(ch);
        }
    }

    input[pos]='\0';

    end=time(NULL);

    double timeTaken=
    difftime(end,start)/60.0;

    if(timeTaken<=0)
    timeTaken=0.01;

    char *original=random_sentence;

    int correctChars=0;

    int totalChars=strlen(original);

    int inputLen=strlen(input);

    int minLen=
    inputLen<totalChars?inputLen:totalChars;

    for(int i=0;i<minLen;i++)
    {
        if(input[i]==original[i])
        correctChars++;
    }

    double accuracy=
    ((double)correctChars/totalChars)*100;

    double wpm=
    ((double)inputLen/5.0)/timeTaken;

    printf("\n\n===== Results =====\n");

    if(timed_out)
    printf("Time is up!\n");

    printf("Time taken: %.2f sec\n",
    difftime(end,start));

    printf("WPM: %.2f\n",wpm);

    printf("Accuracy: %.2f%%\n",accuracy);

    return 0;
}