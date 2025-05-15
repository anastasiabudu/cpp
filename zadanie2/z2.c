#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h" // ReLU funkcia a pomocne funkcie
#include "data.h" // Makra, vahy a bias hodnoty


int choice;

void fun1(){
    int neuron_index;
    scanf("%d", &neuron_index);

    if (neuron_index >= 0 && neuron_index < NUMBER_OF_NEURONS) {
        for (unsigned int i = 0; i < IMAGE_SIZE; i++) {
            printf("%10.2lf ", weights[neuron_index * IMAGE_SIZE + i]);

            if ((i + 1) % 10 == 0) {
                printf("\n");
            }
        }
    }
}

void fun2(int choice){
    int neuron_index;
    scanf("%d", &neuron_index);

    if (neuron_index >= 0 && neuron_index < NUMBER_OF_NEURONS) {
        // Read pixel values for the image
        double input_data[IMAGE_SIZE]; //784
        for (unsigned int i = 0; i < IMAGE_SIZE; i++) {
            scanf("%lf", &input_data[i]);
        }

        double weighted_sum = 0.0;
        for (unsigned int i = 0; i < IMAGE_SIZE; i++) {
            weighted_sum += input_data[i] * weights[neuron_index * IMAGE_SIZE + i];
        }

        // Add bias term
        weighted_sum += bias[neuron_index];

        // Print the result with 2 decimal places
        printf("%.2lf\n", weighted_sum);
    }

}



void fun3() {

    double input_value;
    scanf("%lf", &input_value);

    double result = relu(input_value);

    printf("%.2lf\n", result);
}



void softmax(double *z, unsigned int len) {
    double sum_exp = 0.0;

    for (unsigned int i = 0; i < len; i++) {
        z[i] = exp(z[i]);
        sum_exp += z[i];
    }

    for (unsigned int i = 0; i < len; i++) {
        z[i] /= sum_exp;
    }
}



void fun4(){
    double input_values[10];

    load_data(input_values, 10);

    softmax(input_values, 10);

    for (unsigned int i = 0; i < 10; i++) {
        printf("%.2lf ", input_values[i]);
    }
    printf("\n");
}

int find_max_index(const double *arr, const unsigned int len) {
    int max_index = 0;
    double max_value = arr[0];

    for (unsigned int i = 1; i < len; i++) {
        if (arr[i] > max_value) {
            max_value = arr[i];
            max_index = i;
        }
    }

    return max_index;
}

void fun5(){
    double input_values[10];
    load_data(input_values, 10);
    int max_index = find_max_index(input_values, 10);

    printf("%d\n", max_index);
}

void fun6(){
    double input_image[IMAGE_SIZE];

    load_data(input_image, IMAGE_SIZE);

    print_image(input_image, 28, 28);
}


int fun7(const double *input_image) {
    double z[10];

    for (int i = 0; i < NUMBER_OF_NEURONS; i++) {
        z[i] = bias[i];
        for (int j = 0; j < IMAGE_SIZE; j++) {
            z[i] += input_image[j] * weights[i * IMAGE_SIZE + j];
        }
    }
    softmax(z, 10);

    int max_index = find_max_index(z, 10);

    return max_index;
}




int main(void) {
    if (scanf("%d", &choice) == 1) {
        if (choice == 1) {
            fun1(choice);
        } else if (choice == 2) {
            fun2(choice);
        } else if (choice == 3) {
            fun3(choice);
        } else if( choice == 4){
            fun4(choice);
        }else if(choice == 5){
            fun5(choice);
        }else if(choice == 6){
            fun6(choice);
        }else if(choice == 7){
            double input_image[784];
            for (int i = 0; i < 784; i++) {
                scanf("%lf", &input_image[i]);
            }

            int digit = fun7(input_image);
            printf("%d", digit);

        }
    }

    return 0;
}
