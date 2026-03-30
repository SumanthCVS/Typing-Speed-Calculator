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
    // Large, diverse, non-repetitive English word bank
    char *word_bank[] = {
        "apple", "banana", "car", "dog", "elephant", "forest", "guitar", "house", "island", "jungle",
        "kangaroo", "lemon", "mountain", "notebook", "orange", "pencil", "queen", "river", "sun", "tree",
        "umbrella", "violin", "window", "xylophone", "yacht", "zebra", "adventure", "breeze", "candle", "dolphin",
        "engine", "feather", "garden", "horizon", "iceberg", "jacket", "kitten", "lantern", "mirror", "nebula",
        "ocean", "parrot", "quartz", "rainbow", "saddle", "tiger", "unicorn", "valley", "whale", "yogurt",
        "zephyr", "anchor", "blossom", "crystal", "drizzle", "ember", "fossil", "galaxy", "harbor", "ink",
        "jewel", "kettle", "lizard", "meadow", "nectar", "opal", "petal", "quiver", "ripple", "sphinx",
        "thunder", "utopia", "velvet", "willow", "yawn", "zenith", "asteroid", "bison", "coral", "daisy",
        "falcon", "glacier", "hazel", "iris", "koala", "lily", "mango", "nectarine", "onyx", "pebble",
        "quokka", "reef", "sage", "topaz", "urchin", "violet", "wren", "yeti", "azure", "bamboo",
        "cascade", "dew", "elm", "fern", "grove", "heather", "ivy", "jade", "kelp", "lichen",
        "moss", "nutmeg", "owl", "pine", "quince", "reed", "spruce", "thistle", "umbra", "vine",
        "walnut", "yucca", "zinnia", "acorn", "clover", "dune", "echo", "fjord", "gale", "harp",
        "juniper", "koi", "lotus", "maple", "oak", "petunia", "rose", "tulip", "urn", "yarrow",
        "zen", "ant", "bee", "cat", "duck", "eel", "fox", "goat", "hen", "ibis",
        "jay", "kite", "lion", "mole", "newt", "pig", "quail", "rat", "slug", "toad",
        "vole", "wolf", "yak", "arch", "bridge", "cave", "dock", "escarpment", "gorge", "hill",
        "isle", "jetty", "knoll", "ledge", "mesa", "nook", "outcrop", "peak", "quarry", "ridge",
        "slope", "tor", "upland", "vale", "warren", "yard", "ziggurat", "cloud", "river", "stone",
        "field", "star", "planet", "comet", "orbit", "rocket", "space", "dream", "story", "music",
        "dance", "light", "shadow", "echo", "voice", "song", "poem", "book", "page", "pen",
        "ink", "paper", "note", "line", "verse", "rhyme", "tale", "game", "play", "move",
        "jump", "run", "walk", "climb", "slide", "spin", "roll", "laugh", "smile", "cry",
        "shout", "whisper", "talk", "speak", "listen", "hear", "see", "look", "watch", "glance",
        "gaze", "peek", "peer", "stare", "view", "scan", "survey", "build", "make", "create",
        "form", "shape", "mold", "craft", "design", "draw", "paint", "cook", "bake", "boil",
        "fry", "roast", "grill", "steam", "stew", "blend", "mix", "plant", "grow", "water",
        "harvest", "pick", "dig", "sow", "prune", "trim", "weed", "drive", "ride", "sail",
        "fly", "row", "paddle", "steer", "guide", "lead", "follow", "search", "find", "seek",
        "explore", "discover", "learn", "study", "read", "write", "count", "add", "subtract", "multiply",
        "divide", "measure", "weigh", "compare", "estimate", "calculate", "solve", "think", "know", "guess",
        "wonder", "imagine", "dream", "plan", "decide", "choose", "prefer", "begin", "start", "continue",
        "finish", "end", "stop", "pause", "wait", "rest", "sleep", "wake", "rise", "stand",
        "sit", "lie", "bend", "stretch", "reach", "touch", "hold", "carry", "bring", "take",
        "fetch", "send", "deliver", "return", "leave", "stay", "remain", "open", "close", "shut",
        "lock", "unlock", "enter", "exit", "arrive", "depart", "visit", "meet", "greet", "welcome",
        "invite", "join", "gather", "collect", "assemble", "group", "team", "help", "assist", "support",
        "aid", "care", "protect", "defend", "guard", "save", "rescue", "love", "like", "enjoy",
        "admire", "respect", "trust", "value", "cherish", "appreciate", "hope", "wish", "desire", "want",
        "need", "require", "demand", "expect", "ask", "request", "thank", "praise", "congratulate", "applaud",
        "cheer", "encourage", "motivate", "inspire", "teach", "train", "practice", "review", "test", "quiz",
        "examine", "check", "mark", "grade"
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
    printf("%s\n\n", random_sentence);
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
