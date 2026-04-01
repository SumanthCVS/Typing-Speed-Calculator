#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#define MAX_TEXT 2000
#define TEST_TIME 60

#ifndef _WIN32

struct termios oldt;

void reset_terminal()
{
    tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
}

void init_terminal()
{
    struct termios newt;

    tcgetattr(STDIN_FILENO,&oldt);

    newt=oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO,TCSANOW,&newt);

    fcntl(STDIN_FILENO,F_SETFL,O_NONBLOCK);

    atexit(reset_terminal);
}

int kbhit()
{
    char ch;

    if(read(STDIN_FILENO,&ch,1)==1)
    {
        ungetc(ch,stdin);
        return 1;
    }

    return 0;
}

int getch()
{
    char ch;

    if(read(STDIN_FILENO,&ch,1)==1)
    return ch;

    return 0;
}

#endif

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

#ifndef _WIN32
init_terminal();
#endif

char *word_bank[]={
"the","and","have","that","for","you","with","say","this","they",
"but","his","from","not","she","what","their","can","who","get",
"would","her","make","about","know","will","one","time","there","year",
"think","when","which","them","some","people","take","out","into","just",
"see","him","your","come","could","now","than","like","other","how",
"then","its","our","two","more","these","want","way","look","first",
"also","new","because","day","use","no","man","find","here","thing",
"give","many","well","only","those","tell","very","even","back","any",
"good","woman","through","us","life","child","work","down","may","after",
"should","call","world","over","school","still","try","in","as","on",
"is","it","at","by","an","be","if","or","do","so",
"up","long","great","where","little","round","show","every","me","most",
"much","before","line","right","too","means","old","same","boy","follow",
"three","small","set","put","end","does","another","large","must","big",
"even","such","turn","ask","went","men","read","need","land","different",
"home","move","kind","hand","picture","again","change","off","play","spell",
"air","away","animal","house","point","page","letter","mother","answer","found",
"study","learn","America","high","near","add","food","between","own","below",
"country","plant","last","father","keep","tree","never","start","city","earth",
"eye","light","thought","head","under","story","saw","left","few","while",
"along","might","close","something","seem","next","hard","open","example","begin",
"always","both","paper","together","got","group","often","run","important","until",
"children","side","feet","car","mile","night","walk","white","sea","began",
"grow","took","river","four","carry","state","once","book","hear","stop",
"without","second","later","miss","idea","enough","eat","face","watch","far",
"Indian","really","almost","let","above","girl","sometimes","mountain","cut","young",
"talk","soon","list","song","being","leave","family","body","music","color",
"stand","sun","questions","fish","area","mark","dog","horse","birds","problem",
"complete","room","knew","since","ever","piece","told","usually","friends","easy",
"heard","order","red","door","sure","become","top","ship","across","today",
"during","short","better","best","however","low","hours","black","products","happened",
"whole","measure","remember","early","waves","reached","listen","wind","rock","space",
"covered","fast","several","hold","toward","five","step","morning","passed","vowel",
"true","hundred","against","pattern","numeral","table","north","slowly","money","map",
"farm","pulled","draw","voice","seen","cold","cried","plan","notice","south",
"sing","war","ground","fall","king","town","unit","figure","certain","field",
"travel","wood","fire","upon","done","English","road","halt","ten","fly",
"gave","box","finally","wait","correct"
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
    strcat(random_sentence,
    word_bank[unique_indices[i]]);

    if(i<num_words-1)
    strcat(random_sentence," ");
}

char input[MAX_TEXT];

clearScreen();

printf("===== Typing Speed Test =====\n\n");

printf("Type the following sentence as fast as you can:\n\n");

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

printf("\n\n");

printf("Press ENTER when ready...");
getchar();

printf("Start typing (60 seconds):\n\n");

time_t start=0,end;

int timed_out=0;

int timer_started=0;

int pos=0;

while(1)
{
    if(timer_started)
    {
        if(difftime(time(NULL),start)>=60)
        {
            timed_out=1;
            break;
        }
    }

#ifdef _WIN32
    if(_kbhit())
    {
        int ch=_getch();
#else
    if(kbhit())
    {
        int ch=getch();
#endif

        if(!timer_started)
        {
            start=time(NULL);
            timer_started=1;
        }

        if(ch=='\n' || ch==13)
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
(inputLen<totalChars)?inputLen:totalChars;

for(int i=0;i<minLen;i++)
{
    if(input[i]==original[i])
    correctChars++;
}

double accuracy=
((double)correctChars/totalChars)*100;

int wordCount=0;

for(int i=0;i<inputLen;i++)
{
    if(input[i]==' ')
    wordCount++;
}

if(inputLen>0)
wordCount++;

double wpm=
wordCount/timeTaken;

printf("\n===== Results =====\n");

if(timed_out)
printf("Time is up! Try again!\n");
else
{
printf("Time taken: %.2f seconds\n",
difftime(end,start));

printf("Words per minute (WPM): %.2f\n",
wpm);

printf("Accuracy: %.2f%%\n",
accuracy);
}

return 0;
}