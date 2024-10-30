int numOfWords (char *fileName){
    FILE * file = fopen(fileName, "r");
    char word [25] ={ 'a'};
    int count = 0;
    while ( fscanf(file, "%s", word) != EOF){
        count++;
    }
    fclose(file);
    // free(word);
    return count;
}
int main() {
    // strcpy(arr[0], "hello");
    // printf("%s\n", arr[0]);
    // printf("%lu\n", strlen(arr[0]));
    // readWords("/Users/gracewang/Documents/Stony Brook/CSE220/HW/test/words.txt");
        // printWords(words, 235885);
        printf("%d", numOfWords("words.txt"));

    return 0;
}