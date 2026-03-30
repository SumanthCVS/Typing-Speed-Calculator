#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_TEXT 2000

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    // Simple, common English words for typing practice
    char *word_bank[] = {
        "the", "and", "have", "that", "for", "you", "with", "say", "this", "they",
        "but", "his", "from", "not", "she", "what", "their", "can", "who", "get",
        "would", "her", "make", "about", "know", "will", "one", "time", "there", "year",
        "think", "when", "which", "them", "some", "people", "take", "out", "into", "just",
        "see", "him", "your", "come", "could", "now", "than", "like", "other", "how",
        "then", "its", "our", "two", "more", "these", "want", "way", "look", "first",
        "also", "new", "because", "day", "use", "no", "man", "find", "here", "thing",
        "give", "many", "well", "only", "those", "tell", "very", "even", "back", "any",
        "good", "woman", "through", "us", "life", "child", "work", "down", "may", "after",
        "should", "call", "world", "over", "school", "still", "try", "in", "as", "on",
        "is", "it", "at", "by", "an", "be", "if", "or", "do", "so",
        "up", "long", "great", "where", "little", "round", "show", "every", "me", "most",
        "much", "before", "line", "right", "too", "means", "old", "same", "boy", "follow",
        "three", "small", "set", "put", "end", "does", "another", "large", "must", "big",
        "even", "such", "turn", "ask", "went", "men", "read", "need", "land", "different",
        "home", "move", "kind", "hand", "picture", "again", "change", "off", "play", "spell",
        "air", "away", "animal", "house", "point", "page", "letter", "mother", "answer", "found",
        "study", "learn", "America", "high", "near", "add", "food", "between", "own", "below",
        "country", "plant", "last", "father", "keep", "tree", "never", "start", "city", "earth",
        "eye", "light", "thought", "head", "under", "story", "saw", "left", "few", "while",
        "along", "might", "close", "something", "seem", "next", "hard", "open", "example", "begin",
        "always", "both", "paper", "together", "got", "group", "often", "run", "important", "until",
        "children", "side", "feet", "car", "mile", "night", "walk", "white", "sea", "began",
        "grow", "took", "river", "four", "carry", "state", "once", "book", "hear", "stop",
        "without", "second", "later", "miss", "idea", "enough", "eat", "face", "watch", "far",
        "Indian", "really", "almost", "let", "above", "girl", "sometimes", "mountain", "cut", "young",
        "talk", "soon", "list", "song", "being", "leave", "family", "body", "music", "color",
        "stand", "sun", "questions", "fish", "area", "mark", "dog", "horse", "birds", "problem",
        "complete", "room", "knew", "since", "ever", "piece", "told", "usually", "friends", "easy",
        "heard", "order", "red", "door", "sure", "become", "top", "ship", "across", "today",
        "during", "short", "better", "best", "however", "low", "hours", "black", "products", "happened",
        "whole", "measure", "remember", "early", "waves", "reached", "listen", "wind", "rock", "space",
        "covered", "fast", "several", "hold", "toward", "five", "step", "morning", "passed", "vowel",
        "true", "hundred", "against", "pattern", "numeral", "table", "north", "slowly", "money", "map",
        "farm", "pulled", "draw", "voice", "seen", "cold", "cried", "plan", "notice", "south",
        "sing", "war", "ground", "fall", "king", "town", "unit", "figure", "certain", "field",
        "travel", "wood", "fire", "upon", "done", "English", "road", "halt", "ten", "fly",
        "gave", "box", "finally", "wait", "correct"
    };
    int word_bank_size = sizeof(word_bank) / sizeof(word_bank[0]);
    char random_sentence[2000] = "";
    int num_words = 20;
    int unique_indices[30]; // support up to 30
    int count = 0;
    srand((unsigned int)time(NULL));
    // Generate num_words unique random indices
    while (count < num_words) {
        int idx = rand() % word_bank_size;
        int duplicate = 0;
        for (int j = 0; j < count; j++) {
            if (unique_indices[j] == idx) {
                duplicate = 1;
                break;
            }
        }
        if (!duplicate) {
            unique_indices[count++] = idx;
        }
    }
    for (int i = 0; i < num_words; i++) {
        strcat(random_sentence, word_bank[unique_indices[i]]);
        if (i < num_words - 1) strcat(random_sentence, " ");
    }

    char input[MAX_TEXT];
    // clearScreen(); // Removed to keep the sentence visible
    printf("===== Typing Speed Test =====\n\n");
    printf("Type the following sentence as fast as you can:\n\n");
    // Print the sentence, wrapping only between words (not in the middle of a word)
    int line_len = 0;
    int max_line = 60; // characters per line before wrapping
    for (int i = 0; i < num_words; i++) {
        int word_len = strlen(word_bank[unique_indices[i]]);
        if (line_len + word_len > max_line) {
            printf("\n");
            line_len = 0;
        }
        printf("%s", word_bank[unique_indices[i]]);
        line_len += word_len;
        if (i < num_words - 1) {
            printf(" ");
            line_len++;
        }
    }
    printf("\n\n");
    printf("Press ENTER when ready...");
    getchar();

    // Do not clear the screen here, so the sentence remains visible
    printf("Start typing (you have 60 seconds):\n\n");

    time_t start, end;
    int timed_out = 0;
    input[0] = '\0';

    start = time(NULL);
    // Read input with timer
    for (int i = 0; i < MAX_TEXT - 1; i++) {
        if (difftime(time(NULL), start) >= 60) {
            timed_out = 1;
            break;
        }
        int ch = getchar();
        if (ch == '\n' || ch == EOF) {
            input[i] = '\0';
            break;
        }
        input[i] = (char)ch;
        input[i+1] = '\0';
    }
    end = time(NULL);

    double timeTaken = difftime(end, start) / 60.0; // in minutes

    char *original = random_sentence;

    // Calculate accuracy
    int correctChars = 0;
    int totalChars = strlen(original);
    int inputLen = strlen(input);
    int minLen = (inputLen < totalChars) ? inputLen : totalChars;
    for (int i = 0; i < minLen; i++) {
        if (input[i] == original[i]) {
            correctChars++;
        }
    }

    double accuracy = ((double)correctChars / totalChars) * 100;

    // Calculate WPM
    int wordCount = 0;
    for (int i = 0; i < inputLen; i++) {
        if (input[i] == ' ') {
            wordCount++;
        }
    }
    if (inputLen > 0) wordCount++; // last word

    double wpm = wordCount / timeTaken;

    // Output results
    printf("\n===== Results =====\n");
    if (timed_out) {
        printf("Time is up! You didn't finish in time. Give it another shot!\n");
    } else {
        printf("Time taken: %.2f seconds\n", difftime(end, start));
        printf("Words per minute (WPM): %.2f\n", wpm);
        printf("Accuracy: %.2f%%\n", accuracy);
    }

    return 0;
}