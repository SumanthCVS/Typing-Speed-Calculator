#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_TEXT 500

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    char *texts[] = {
        "The quick brown fox jumps over the lazy dog",
        "C programming is powerful and efficient",
        "Typing fast requires practice and consistency"
    };

    char input[MAX_TEXT];
    int choice;
    int totalTexts = 3;

    clearScreen();
    printf("===== Typing Speed Test =====\n\n");

    // Select text
    printf("Choose a text to type:\n");
    for (int i = 0; i < totalTexts; i++) {
        printf("%d. %s\n", i + 1, texts[i]);
    }

    printf("\nEnter choice: ");
    scanf("%d", &choice);
    getchar(); // consume newline

    if (choice < 1 || choice > totalTexts) {
        printf("Invalid choice.\n");
        return 1;
    }

    clearScreen();
    printf("Type the following text:\n\n");
    printf("\"%s\"\n\n", texts[choice - 1]);

    printf("Press ENTER when ready...");
    getchar();

    clearScreen();
    printf("Start typing:\n\n");

    time_t start, end;
    time(&start);

    fgets(input, MAX_TEXT, stdin);

    time(&end);

    double timeTaken = difftime(end, start) / 60.0; // in minutes

    // Remove newline
    input[strcspn(input, "\n")] = 0;

    char *original = texts[choice - 1];

    // Calculate accuracy
    int correctChars = 0;
    int totalChars = strlen(original);

    for (int i = 0; i < totalChars && i < strlen(input); i++) {
        if (input[i] == original[i]) {
            correctChars++;
        }
    }

    double accuracy = ((double)correctChars / totalChars) * 100;

    // Calculate WPM
    int wordCount = 0;
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == ' ') {
            wordCount++;
        }
    }
    wordCount++; // last word

    double wpm = wordCount / timeTaken;

    // Output results
    printf("\n===== Results =====\n");
    printf("Time taken: %.2f seconds\n", difftime(end, start));
    printf("Words per minute (WPM): %.2f\n", wpm);
    printf("Accuracy: %.2f%%\n", accuracy);

    return 0;
}