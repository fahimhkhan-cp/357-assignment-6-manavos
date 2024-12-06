#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <ctype.h>


typedef struct CountyData{
    char county[100];
    char state[3];
    float educationBachelor; //Education.Bachelor's Degree or Higher
    float educationHS; //Education.High School or Higher
    float ethnicityAI; //Ethnicities.American Indian and Alaska Native Alone	
    float ethnicityA; //Ethnicities.Asian Alone	
    float ethnicityB; //Ethnicities.Black Alone	
    float ethnicityH; //Ethnicities.Hispanic or Latino	
    float ethnicityNH; //Ethnicities.Native Hawaiian and Other Pacific Islander Alone	
    float ethnicity2; //Ethnicities.Two or More Races	
    float ethnicityW; //Ethnicities.White Alone	
    float ethnicityW2; //Ethnicities.White Alone not Hispanic or Latino	
    int incomeM; //Income.Median Household Income
    int incomeC; //Income.Per Capita Income	
    float incomeP; //Income.Persons Below Poverty Level
    int population_2014; //Population.2014 Population
} CountyData;

const char *valid_columns[] = {"County", "State", "Education.Bachelor's Degree or Higher", "Education.High School or Higher", "Ethnicities.American Indian and Alaska Native Alone", "Ethnicities.Asian Alone", "Ethnicities.Black Alone", "Ethnicities.Hispanic or Latino", "Ethnicities.Native Hawaiian and Other Pacific Islander Alone", "Ethnicities.Two or More Races", "Ethnicities.White Alone", "Ethnicities.White Alone not Hispanic or Latino", "Income.Median Household Income", "Income.Per Capita Income", "Income.Persons Below Poverty Level", "Population.2014 Population"};
enum { COUNTY, STATE, ED1, ED2, ET1, ET2, ET3, ET4, ET5, ET6, ET7, ET8, I1, I2, I3, POPULATION, VALID_COLUMN_COUNT};
int valid_indices[VALID_COLUMN_COUNT];
int dataCount = 0;
CountyData data[4000];

void remove_quotes(char *str) {
    int j = 0;
    for (int i = 1;  i <= strlen(str) - 2; i++) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

int is_valid_field(const char *target, const char *array[], int array_size) {
    for (int i = 0; i < array_size; i++) {
        if (strcmp(target, array[i]) == 0) {
            return 1; //column is valid field
        }
    }
    return 0;
}

float *get_field(struct CountyData *data, char *column) { //return field value of element
    if (strcmp(column, "Education.Bachelor's Degree or Higher") == 0) return &data->educationBachelor;
    if (strcmp(column, "Education.High School or Higher") == 0) return &data->educationHS;
    if (strcmp(column, "Ethnicities.American Indian and Alaska Native Alone") == 0) return &data->ethnicityAI;
    if (strcmp(column, "Ethnicities.Asian Alone") == 0) return &data->ethnicityA;
    if (strcmp(column, "Ethnicities.Black Alone") == 0) return &data->ethnicityB;
    if (strcmp(column, "Ethnicities.Hispanic or Latino") == 0) return &data->ethnicityH;
    if (strcmp(column, "Ethnicities.Native Hawaiian and Other Pacific Islander Alone") == 0) return &data->ethnicityNH;
    if (strcmp(column, "Ethnicities.Two or More Races") == 0) return &data->ethnicity2;
    if (strcmp(column, "Ethnicities.White Alone") == 0) return &data->ethnicityW;
    if (strcmp(column, "Ethnicities.White Alone not Hispanic or Latino") == 0) return &data->ethnicityW2;
    if (strcmp(column, "Income.Median Household Income") == 0) return &data->incomeM;
    if (strcmp(column, "Income.Per Capita Income") == 0) return &data->incomeC;
    if (strcmp(column, "Income.Persons Below Poverty Level") == 0) return &data->incomeP;
    if (strcmp(column, "Population.2014 Population") == 0) return &data->population_2014;

    return NULL; // invalid column
}

void removeElement(CountyData arr[], int *size, int index) {

    //shift elements to the left
    for (int i = index; i < *size - 1; i++) {
        arr[i] = arr[i + 1];
    }

    //decrease size of array
    (*size)--;
}

void load_data(char *filename){

    FILE *file = fopen(filename, "r");
    if (file == NULL){
        perror("error opening file");
    }

    //-1 means not found
    for (int i = 0; i < VALID_COLUMN_COUNT; i++) {
        valid_indices[i] = -1;
    }
    
    char *firstline = NULL;
    size_t size;
    int num = getline(&firstline, &size, file);

    //finding valid columns (indices)
    int i = 0;
    int j = 0; //for valid index
    int array_size = sizeof(valid_columns) / sizeof(valid_columns[0]); 
    char *column_name = strtok(firstline, ",");
        while (column_name && i < 255) {
            remove_quotes(column_name);
            if (is_valid_field(column_name, valid_columns, array_size) == 1){
               valid_indices[j++] = i;
                }
            i++;
            column_name = strtok(NULL, ",");
        }

    //check if all columns were found
    for (int i = 0; i < VALID_COLUMN_COUNT; i++) {
        if (valid_indices[i] == -1) {
            fprintf(stderr, "Required column '%s' not found in CSV header\n", valid_columns[i]);
            fclose(file);
            return;
        }
    }

    //start parsing
    char *line;
    size_t size1;
    CountyData newEntry = {0};
    while (getline(&line, &size1, file) >= 0){
        int idx = 0;
        char *column = strtok(line, ",");
        while(column){
            remove_quotes(column);
            if (idx == valid_indices[COUNTY]){
                strcpy(newEntry.county, column);
            }
            else if (idx == valid_indices[STATE]){
                strcpy(newEntry.state, column);
            }
            else if (idx == valid_indices[ED1]){
                newEntry.educationBachelor = atof(column);
            }
            else if (idx == valid_indices[ED2]){
                newEntry.educationHS = atof(column);
            }
            else if (idx == valid_indices[ET1]){
                newEntry.ethnicityAI = atof(column);
            }
            else if (idx == valid_indices[ET2]){
                newEntry.ethnicityA = atof(column);
            }
            else if (idx == valid_indices[ET3]){
                newEntry.ethnicityB = atof(column);
            }
            else if (idx == valid_indices[ET4]){
                newEntry.ethnicityH = atof(column);
            }
            else if (idx == valid_indices[ET5]){
                newEntry.ethnicityNH = atof(column);
            }
            else if (idx == valid_indices[ET6]){
                newEntry.ethnicity2 = atof(column);
            }
            else if (idx == valid_indices[ET7]){
                newEntry.ethnicityW = atof(column);
            }
            else if (idx == valid_indices[ET8]){
                newEntry.ethnicityW2 = atof(column);
            }
            else if (idx == valid_indices[I1]){
                newEntry.incomeM = atoi(column);
            }
            else if (idx == valid_indices[I2]){
                newEntry.incomeC = atoi(column);
            }
            else if (idx == valid_indices[I3]){
                newEntry.incomeP = atof(column);
            }
            else if (idx == valid_indices[POPULATION]){
                newEntry.population_2014 = atoi(column);
            }
            
            column = strtok(NULL, ","); //next
            idx++;

        }
        data[dataCount++] = newEntry; //add to array

    }

    fclose(file);

    printf("Entries Loaded: %d\n", dataCount);

}

void display(){
    for (int i = 0; i < dataCount; i++) {
        printf("%s, %s\n\t2014 Population: %d\n\tEducation\n\t\tBachelor's Degree or Higher: %f%%\n\t\tHigh School or Higher: %f%%\n\tEthnicities\n\t\tAmerican Indian and Alaska Native Alone: %f%%\n\t\tAsian Alone: %f%%\n\t\tBlack Alone: %f%%\n\t\tHispanic or Latino: %f%%\n\t\tNative Hawaiian and Other Pacific Islander Alone: %f%%\n\t\tTwo or More Races: %f%%\n\t\tWhite Alone: %f%%\n\t\tWhite Alone not Hispanic or Latino: %f%%\n\tIncome\n\t\tMedian Household Income: %d\n\t\tPer Capita Income: %d\n\t\tPersons Below Poverty Level: %f%%\n\n",
               data[i].county, data[i].state, data[i].population_2014, data[i].educationBachelor, data[i].educationHS, data[i].ethnicityAI, data[i].ethnicityA, data[i].ethnicityB, data[i].ethnicityH, data[i].ethnicityNH, data[i].ethnicity2, data[i].ethnicityW, data[i].ethnicityW2, data[i].incomeM, data[i].incomeC, data[i].incomeP);
    }


}

void filter_state(char *state){
    int count = 0;
    for (int i = 0; i < dataCount;){
        if (strcmp(data[i].state, state) != 0){
            removeElement(data, &dataCount, i);
    }
    else {
        count++;
        i++; //increment i if kept
    }

    
}
    printf("Filter: state == %s (%d entries)\n", state, count);

}

void filter_field(char *field, char *comp, float num){
    int count = 0;
    for (int i = 0; i < dataCount;) { 
        float *fieldValue = get_field(&data[i], field);
            if (fieldValue == NULL){
                i++;
                continue;
            }
        
            if (strcmp(comp, "le") == 0) {  //<=
                if (*fieldValue > num) {
                    removeElement(data, &dataCount, i); //remove if greater
                } else {
                    count++;
                    i++;  //only increment i when element is kept
                }
            } else if (strcmp(comp, "ge") == 0) {  //>=
                if (*fieldValue < num) {
                    removeElement(data, &dataCount, i); //remove if less
                } else {
                    count++;
                    i++;  //only increment i when element is kept
                }
            }
    
    }
    printf("Filter: %s %s %f (%d entries)\n", field, comp, num, count);
}

int population_total(){
    int total = 0;
    for (int i = 0; i < dataCount; i++) {
        total += data[i].population_2014;
    }

    //printf("2014 population: %d\n", total);
    return total;
}

float population_field(char *field){
    //check for valid fields
    float total = 0;
    for (int i = 0; i < dataCount; i++) {
        float *val =  get_field(&data[i], field);
        float pop = (*val/100.0) * (float)(data[i].population_2014); //calculate sub population
        total += pop;
    }
    //printf("2014 %s population: %f\n", field, total);
    return total;
}


void percent(char *args) {
    char *field = strtok(args, ":"); //argument for filter
    char *comp = strtok(NULL, ":"); //comparison
    char *numStr = strtok(NULL, ":"); //value
    char *subpop = strtok(NULL, ":"); //subpopulation
 
    if (field && comp && numStr && subpop) { //if filter
        float value = atof(numStr);

        filter_field(field, comp, value); //filter data first

        float filtered_pop = population_field(subpop); //find subpopulation based on filtered data
        float total_pop = population_total(); //total population of the filtered counties

        float percentage = (filtered_pop / total_pop) * 100; //find percentage

        // Print the result
        printf("2014 %s percentage (with %s %s %s): %f%%\n", subpop, field, comp, numStr, percentage);
    } else if (strstr(args, ":") == NULL) { //no filter just calculate subpopulation for field
        float field_population = population_field(args);
        float total_population = population_total();

        float percentage = (field_population / total_population) * 100;
        printf("2014 %s percentage: %f%%\n", args, percentage);
    } else {
        fprintf(stderr, "error: percent operation\n");
    }
}


void execute_operations(char *operations_file){
    FILE *file = fopen(operations_file, "r");
    if (file == NULL){
        perror("error opening file");
    }

    char *line = NULL;
    size_t size;
    int lineNum = 0;
    while (getline(&line, &size, file) != -1){
        lineNum++;
        line[strcspn(line, "\n")] = '\0'; //null terminate line
        int matched = 0; //track if matched

        if (strcmp(line, "display") == 0) {
            display();
            matched = 1;
        }

        else if (strcmp(line, "population-total") == 0) {
            population_total();
            matched = 1;
        }

        char *operation = strtok(line, ":");
        char *arg = strtok(NULL, ":");

        if (operation) { //make sure operation is not null
            if (strcmp(operation, "filter-state") == 0) {
                filter_state(arg);
                matched = 1;
            }

            else if (strcmp(operation, "population") == 0) {
                population_field(arg);
                matched = 1;
            }

           else if (strcmp(operation, "percent") == 0) {
                //split into 4 arguments
                char *comp = strtok(NULL, ":");
                char *numStr = strtok(NULL, ":");
                char *sub = strtok(NULL, ":");

                if (comp && numStr && sub) {
                    //operation with filter
                    char full_operation[256];
                    snprintf(full_operation, sizeof(full_operation), "%s:%s:%s:%s", arg, comp, numStr, sub);
                    percent(full_operation);
                } else if (arg) {
                    //no filter only field
                    percent(arg);
                } else {
                    fprintf(stderr, "Malformed percent operation: %s\n", line);
                }
                matched = 1;
            }

            else if (strcmp(operation, "filter") == 0) {
                char *comp = strtok(NULL, ":");
                char *numStr = strtok(NULL, ":");
                if (comp && numStr) { // Ensure `comp` and `numStr` are valid
                    float num = atof(numStr);
                    filter_field(arg, comp, num);
                    matched = 1;
                } else {
                    fprintf(stderr, "Malformed filter operation: %s\n", line);
                }
            }
        }

    //if not a valid operation
    if (matched == 0) {
        fprintf(stderr, "Error: Invalid operation or unrecognized line #%d\n", lineNum);
        continue;
        }

        
    }

}


int main(int args, char *argv[]){
    
    char *filename = argv[1];
    char *operations_file = argv[2];
    
    load_data(filename);
    execute_operations(operations_file);

}