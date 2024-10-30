void init_student(int id, int credits, char *name, struct student *record){

}

void init_student_array(int num_students, int id_list[], int credits_list[], char *names, struct student records[]) {

    int *pointerToIDs = id_list;
    int *pointerToCredits = credits_list;
    char *pointerToNames = names;  
    struct student *pointerToRecords = records;                      
    for (int i = 0; i < num_students; i++){
        *pointerToIDs = *(pointerToIDs + i*4);
        *pointerToCredits = *(pointerToCredits + i*4);
        pointerToRecords = (pointerToRecords + i*8);
        // iterate through the names array and load each byte into the register.
        char * name = "";
        while (*pointerToNames != '\0'){
            name += *pointerToNames;
            pointerToNames++;
        }
        name += '\0';
        init_student(*pointerToIDs, *pointerToCredits, name, pointerToRecords); 

    }

    int insert(struct student *record, struct student *table[], int table_size){
        int studentID = record->ID;
        int initialIndex = studentID % table_size;
        int countSize = 0;
        while (countSize < table_size) {
        if (table[initialIndex] == NULL) {
            table[initialIndex] = record;
            return initialIndex;  // Return the index where the record was inserted
        } else {
            countSize++;
            initialIndex = (initialIndex + 1) % table_size;
        }
    }

    return -1;  // Table is full
}

(struct student*, int) search(int id, struct student *table[], int table_size) {
    int initialIndex = id % table_size;
    int countSize = 0;
    while (countSize < table_size){
        // if we encounter an empty cell, then return NULL (0) in v0 and -1 in v1
        if (table[initialIndex] == 'X')
        return (NULL, -1) // (jump to search done)
        // if we encounter a tombstone then go to next posiiton
        if (table[iniitalIndex] == 'T'){ // if it's not equal, jump to search else
            countSize++;
            initialIndex = (initialIndex + 1) % table_size; // jump to search_while
        }
        // check if our current student's id matches our given id
        else {
            struct * currStudent = table[initialIndex];
            int currID = currStudent->id;
            if (currID == id){
                return (currStudent, iniitalIndex); // (jump to search_done)
            }
            else{
                // move to next index
               countSize++;
                initialIndex = (initialIndex + 1) % table_size; 
            }

        }
        

    }
}

int delete(int id, struct student *table[], int table_size){
    struct student* = search();
    int index = search();
    if (index == -1) return -1 // (in v0)
    else{
        table[index] = 'T'
        return -1
    }
    // go to the index
    // replace the table's index with a -1
}


// }


