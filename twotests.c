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

// Some Windows toolchains (or build flags) default to the GUI subsystem and
// expect a WinMain entry point. Provide one that attaches to a console and
// forwards into our normal main(), so the program still behaves as a terminal app.
#ifdef _WIN32
int main(void);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  (void)hInstance;
  (void)hPrevInstance;
  (void)lpCmdLine;
  (void)nShowCmd;

  if (!AttachConsole(ATTACH_PARENT_PROCESS))
  {
    AllocConsole();
  }

  FILE *fp;
  freopen_s(&fp, "CONIN$", "r", stdin);
  freopen_s(&fp, "CONOUT$", "w", stdout);
  freopen_s(&fp, "CONOUT$", "w", stderr);

  return main();
}
#endif

#define MAX_TEXT 5000
#define MAX_NAME 64
#define MAX_SCORES 1000
#define LEADERBOARD_LIMIT 10

typedef struct
{
  char name[MAX_NAME];
  int wpm;
  double accuracy;
} ScoreEntry;

typedef struct
{
  int timed_out;
  int wpm;
  double accuracy;
  double seconds;
} TestResult;

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

int compareScores(const void *a, const void *b)
{
  const ScoreEntry *sa = (const ScoreEntry *)a;
  const ScoreEntry *sb = (const ScoreEntry *)b;

  if (sb->wpm != sa->wpm)
    return sb->wpm - sa->wpm;

  if (sb->accuracy > sa->accuracy)
    return 1;
  if (sb->accuracy < sa->accuracy)
    return -1;
  return strcmp(sa->name, sb->name);
}

int loadScores(const char *filename, ScoreEntry scores[], int maxScores)
{
  FILE *fp = fopen(filename, "r");
  if (!fp)
    return 0;

  int count = 0;
  char line[256];
  while (count < maxScores && fgets(line, sizeof(line), fp))
  {
    ScoreEntry entry;
    if (sscanf(line, "%63[^|]|%d|%lf", entry.name, &entry.wpm, &entry.accuracy) == 3)
    {
      scores[count++] = entry;
    }
  }
  fclose(fp);
  return count;
}

void appendScore(const char *filename, const char *name, int wpm, double accuracy)
{
  FILE *fp = fopen(filename, "a");
  if (!fp)
  {
    printf("Could not save leaderboard entry.\n");
    return;
  }

  fprintf(fp, "%s|%d|%.2f\n", name, wpm, accuracy);
  fclose(fp);
}

void showLeaderboard(const char *title, const char *filename)
{
  ScoreEntry scores[MAX_SCORES];
  int count = loadScores(filename, scores, MAX_SCORES);
  if (count == 0)
  {
    printf("\n===== %s Leaderboard =====\n", title);
    printf("No scores yet.\n");
    return;
  }

  qsort(scores, count, sizeof(ScoreEntry), compareScores);

  printf("\n===== %s Leaderboard =====\n", title);
  int limit = count < LEADERBOARD_LIMIT ? count : LEADERBOARD_LIMIT;
  for (int i = 0; i < limit; i++)
  {
    printf("%2d. %-20s WPM: %3d  Accuracy: %6.2f%%\n",
           i + 1, scores[i].name, scores[i].wpm, scores[i].accuracy);
  }
}

TestResult typingTest(char *original,int TEST_TIME)
{
TestResult result;
result.timed_out = 0;
result.wpm = 0;
result.accuracy = 0.0;
result.seconds = 0.0;

char input[MAX_TEXT];

time_t start,end;

int timed_out=0;

int pos=0;

printf("\nStart typing:\n\n");
start=time(NULL);

while(1)
{
if(difftime(time(NULL),start)>=TEST_TIME)
{
timed_out=1;
break;
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

int wpmInt=(int)(wpm+0.5);

printf("\n\n===== Results =====\n");

if(timed_out)
{
printf("Time limit reached (%d seconds). Try again!\n",TEST_TIME);
result.timed_out = 1;
result.seconds = difftime(end,start);
return result;
}

printf("Time: %.2f sec\n",
difftime(end,start));

printf("WPM: %d\n",wpmInt);

printf("Accuracy: %.2f%%\n",accuracy);

result.timed_out = 0;
result.wpm = wpmInt;
result.accuracy = accuracy;
result.seconds = difftime(end,start);
return result;

}

void memoryTest(const char *playerName)
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

TestResult result=typingTest(chosen,45);

if(!result.timed_out)
{
if(result.accuracy>90.0)
{
appendScore("memory_leaderboard.txt",playerName,result.wpm,result.accuracy);
printf("Score saved to Sentence Recall leaderboard.\n");
}
else
{
printf("Score not saved (accuracy must be above 90%%).\n");
}
}

showLeaderboard("Sentence Recall Challenge","memory_leaderboard.txt");

}

void randomWordTest(const char *playerName)
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

TestResult result=typingTest(sentence,60);

if(!result.timed_out)
{
if(result.accuracy>90.0)
{
appendScore("random_leaderboard.txt",playerName,result.wpm,result.accuracy);
printf("Score saved to Random Word leaderboard.\n");
}
else
{
printf("Score not saved (accuracy must be above 90%%).\n");
}
}

showLeaderboard("Random Word Typing Test","random_leaderboard.txt");

}

int main(void)
{

#ifndef _WIN32
init_terminal();
#endif

srand(time(NULL));

int choice;
char playerName[MAX_NAME];

printf("\nChoose one:\n");
printf("1 Random word typing test\n");
printf("2 Sentence Recall Challenge\n");
printf("Choice: ");

if (scanf("%d",&choice) != 1)
{
printf("Invalid choice\n");
return 1;
}

getchar();

printf("Enter player name: ");
if (fgets(playerName, sizeof(playerName), stdin) == NULL)
{
printf("Invalid name input\n");
return 1;
}
playerName[strcspn(playerName, "\n")] = '\0';
if (strlen(playerName) == 0)
strcpy(playerName, "Player");

if(choice==1)
randomWordTest(playerName);
else if(choice==2)
memoryTest(playerName);
else
{
printf("Invalid choice\n");
return 1;
}

return 0;

}