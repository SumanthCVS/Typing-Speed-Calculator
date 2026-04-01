#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define sleep(x) Sleep(1000*(x))
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#define MAX_TEXT 5000

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

double calculateAccuracy(char *original,char *input)
{
int correct=0;

int total=strlen(original);

int inputLen=strlen(input);

int minLen=inputLen<total?inputLen:total;

for(int i=0;i<minLen;i++)
{
if(original[i]==input[i])
correct++;
}

return ((double)correct/total)*100;
}

double calculateWPM(char *input,double timeTaken)
{
int wordCount=0;

int len=strlen(input);

for(int i=0;i<len;i++)
{
if(input[i]==' ')
wordCount++;
}

if(len>0)
wordCount++;

return wordCount/timeTaken;
}

void typingTest(char *original,int TEST_TIME)
{

char input[MAX_TEXT];

time_t start=0,end;

int timer_started=0;

int timed_out=0;

int pos=0;

printf("\nStart typing:\n\n");

while(1)
{

if(timer_started)
{
if(difftime(time(NULL),start)>=TEST_TIME)
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

double accuracy=
calculateAccuracy(original,input);

double wpm=
calculateWPM(input,timeTaken);

printf("\n\n===== Results =====\n");

if(timed_out)
printf("Time up!\n");

printf("Time: %.2f sec\n",
difftime(end,start));

printf("WPM: %.2f\n",wpm);

printf("Accuracy: %.2f%%\n",accuracy);

}

void memoryTest()
{

char *sentences[]={
"The small cat slept quietly near the warm window during the afternoon",
"Practice a little every day and your skills will slowly become stronger",
"Hard work with clear focus usually brings better results over time",
"A good plan today can prevent many problems tomorrow",
"Learning new things keeps the mind active and fresh",
"Simple habits done daily can change your future",
"Patience and effort together make difficult tasks easier",
"Reading carefully helps you understand things better",
"Step by step progress is still real progress",
"Clear thinking helps you solve most problems"
};

int size=
sizeof(sentences)/sizeof(sentences[0]);

int idx=rand()%size;

char *chosen=
sentences[idx];

clearScreen();

printf("Memorize this sentence (you have 10 seconds):\n\n");

printf("%s",chosen);

sleep(12);

clearScreen();

printf("Press ENTER to start typing...");
getchar();

typingTest(chosen,45);

}

void randomWordTest()
{

char *word_bank[]={
/* YOUR FULL WORD LIST HERE EXACTLY AS PROVIDED */
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

int size=
sizeof(word_bank)/sizeof(word_bank[0]);

int num_words=20;

int unique[500];

int count=0;

while(count<num_words)
{

int idx=rand()%size;

int dup=0;

for(int j=0;j<count;j++)
{
if(unique[j]==idx)
{
dup=1;
break;
}
}

if(!dup)
unique[count++]=idx;

}

char sentence[MAX_TEXT]="";

for(int i=0;i<num_words;i++)
{

strcat(sentence,
word_bank[unique[i]]);

if(i<num_words-1)
strcat(sentence," ");

}

clearScreen();

printf("Type this:\n\n");

printf("%s\n",sentence);

printf("\nPress ENTER when ready...");
getchar();

typingTest(sentence,60);

}

int main()
{

#ifndef _WIN32
init_terminal();
#endif

srand(time(NULL));

int choice;

while(1)
{

printf("\nChoose one:\n");

printf("1 Random word typing test\n");

printf("2 Memory sentence test\n");

printf("3 Exit\n");

printf("Choice: ");

scanf("%d",&choice);

getchar();

if(choice==1)
randomWordTest();

else if(choice==2)
memoryTest();

else if(choice==3)
break;

else
printf("Invalid choice\n");

}

return 0;

}