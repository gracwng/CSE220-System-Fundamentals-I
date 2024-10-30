#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_words(const char *str) {
    int count = 0;
char *token = strtok((char *)str, " ");

    // Count the tokens (words)
    while (token != NULL) {
        count++;
        token = strtok(NULL, " "); // Get the next token
    }

    return count;
}

int main() {
    FILE *file_exp, *file_act;
    char exp_str[1000000], act_str[1000000];
    const char *exp_file_path = "/Users/gracewang/Documents/Stony Brook/CSE220/HW/cse220_hw2/src/exp_single_line.txt";
    const char *act_file_path = "/Users/gracewang/Documents/Stony Brook/CSE220/HW/cse220_hw2/src/act_single_line.txt";

    // Open exp_single_line.txt for reading
    file_exp = fopen(exp_file_path, "r");
    if (file_exp == NULL) {
        perror("Error opening exp_single_line.txt");
        return 1;
    }

    // Read the content of exp_single_line.txt into exp_str
    fgets(exp_str, sizeof(exp_str), file_exp);

    // Close exp_single_line.txt
    fclose(file_exp);

    // Open act_single_line.txt for reading
    file_act = fopen(act_file_path, "r");
    if (file_act == NULL) {
        perror("Error opening act_single_line.txt");
        return 1;
    }

    // Read the content of act_single_line.txt into act_str
    fgets(act_str, sizeof(act_str), file_act);

    // Close act_single_line.txt
    fclose(file_act);

    // Count the words in each string
    int exp_word_count = count_words(exp_str);
    int act_word_count = count_words(act_str);

    // Compare the word counts
    if (exp_word_count == act_word_count) {
        printf("The number of words in exp_single_line.txt and act_single_line.txt are the same: %d.\n", exp_word_count);
    } else {
        printf("The number of words in exp_single_line.txt (%d) and act_single_line.txt (%d) are different.\n", exp_word_count, act_word_count);
    }

    return 0;
}
