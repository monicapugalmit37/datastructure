#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include <stdbool.h>
#define MAX_DISTRICTS 33
#define BASE_PRICE 45
#define UNIT_PRICE 25
#define MAX_USERS 100

struct User {
    char name[20];
    char address[50];
    int age;
    int choice; // To store the user's choice
};

// Define a structure for booking details
struct BookingNode {
    char phoneNumber[15];
    struct User user;
    struct BookingNode* next;
};


struct Train {
    char name[20];
    int trainNumber;
    char sourceDistrict[20];
    char destinationDistrict[20];
    char departureTime[10];
    char arrivalTime[10];
    int distanceCovered;// Distance covered by the train
};

struct TreeNode {
    struct Train train;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct TrainUserDetails {
    struct BookingNode* head;
    int userCount;
};

void insertBookingNode(struct BookingNode** head, struct User newUser, char* phoneNumber) {
    struct BookingNode* newNode = (struct BookingNode*)malloc(sizeof(struct BookingNode));
    if (newNode == NULL) {
        // Handle memory allocation failure
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->user = newUser;
    strncpy(newNode->phoneNumber, phoneNumber, sizeof(newNode->phoneNumber) - 1);
    newNode->next = NULL;

    if (*head == NULL) {
        // If the linked list is empty, make the new node the head
        *head = newNode;
    } else {
        // Otherwise, traverse to the end and add the new node
        struct BookingNode* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to cancel a ticket based on phone number
void cancelTicket(struct BookingNode** head, char* phoneNumber) {
    struct BookingNode* current = *head;
    struct BookingNode* prev = NULL;

    // Traverse the linked list to find the booking with the given phone number
    while (current != NULL && strcmp(current->phoneNumber, phoneNumber) != 0) {
        prev = current;
        current = current->next;
    }

    // If the booking is found, remove it from the linked list
    if (current != NULL) {
        if (prev == NULL) {
            // If the booking is the first node
            *head = current->next;
        } else {
            prev->next = current->next;
        }

        free(current);  // Free the memory of the canceled booking
        printf("\nTicket canceled successfully for phone number %s\n", phoneNumber);
    } else {
        printf("Invalid phone number. No booking found for the provided phone number %s\n", phoneNumber);
    }
}

// Function to find the earliest departure in the BST for a specific source and destination
void findEarliestDeparture(struct TreeNode* root, char* source, char* destination) {
    if (root != NULL) {
        findEarliestDeparture(root->right, source, destination);  // Traverse right subtree first

        if (strcmp(root->train.sourceDistrict, source) == 0 && strcmp(root->train.destinationDistrict, destination) == 0) {
             printf("\033[1;32m");
			printf("Train %d goes with departure time: %s\n", root->train.trainNumber, root->train.departureTime);
        }

        findEarliestDeparture(root->left, source, destination);  // Then traverse left subtree
    }
}


// Function to print the train details
void printTrainDetails(struct Train trains[], int n, char* source, char* destination) {
	  printf("\033[1;32m");
    printf("\n\nTrains from %s to %s:\n\n", source, destination);
      printf("\n\033[1;35m"); 
    printf("%-10s%-15s%-15s%-15s%-15s%-15s\n", "Train No", "Train Name", "Source", "Destination", "Departure", "Arrival");
    printf("-------------------------------------------------------------------------------\n");

    int i, count = 0;
    for (i = 0; i < n; i++) {
        if (strcmp(trains[i].sourceDistrict, source) == 0 && strcmp(trains[i].destinationDistrict, destination) == 0) {
            printf("%-10d%-15s%-15s%-15s%-15s%-15s\n", trains[i].trainNumber, trains[i].name, trains[i].sourceDistrict,
                   trains[i].destinationDistrict, trains[i].departureTime, trains[i].arrivalTime);
            count++;
            if (count == 2) {
                break;  // Displayed at least two trains
            }
        }
    }
}

// Function to calculate ticket price based on distance
int calculateTicketPrice(int distance, int seatClass, int numSeats) {
    int basePrice = BASE_PRICE + UNIT_PRICE * distance * 0.2;
    // Additional logic for seat-based pricing
    int seatPrice = 0;
    if (numSeats > 0) {
        seatPrice = numSeats * 2 * seatClass;  // Assuming 20 as the price per seat
    }
    return basePrice + seatPrice;
}


// Function to find the index of a district in the array
int findDistrictIndex(char districts[MAX_DISTRICTS][20], int numDistricts, char* district) {
    int i;
    for (i = 0; i < numDistricts; i++) {
        if (strcmp(districts[i], district) == 0) {
            return i;
        }
    }
    return -1;  // District not found
}

// Function to insert a train node into the BST
struct TreeNode* insertNode(struct TreeNode* root, struct Train train) {
    if (root == NULL) {
        struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
        if (newNode == NULL) {
            // Handle memory allocation failure
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        newNode->train = train;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    // Compare departure times to decide the position in the BST
    if (strcmp(train.departureTime, root->train.departureTime) < 0) {
        root->left = insertNode(root->left, train);
    } else {
        root->right = insertNode(root->right, train);
    }

    return root;
}
#define MAX_PASS_NAME_LENGTH 20
#define MAX_PASS_VALIDITY 2
#define MAX_PASS_TRAVEL_COUNT 10

struct TrainPass {
    char name[50];
    int age;
    char source[50];
    char destination[50];
    int validity;
    int travelCount;
};

// Assume you have a stack implementation
// Here, we'll use an array as a simple stack
struct TrainPass passStack[MAX_USERS];
int top = -1; // Initialize top of the stack

// Function to push a train pass onto the stack
void push(struct TrainPass newPass) {
    if (top < MAX_USERS - 1) {
        passStack[++top] = newPass;
    } else {
        printf("\033[1;31m"); // Set text color to red
        printf("Error: Train pass stack is full!\n");
        printf("\033[0m"); // Reset text color to default
    }
}

// Function to generate a train pass
void generateTrainPass() {
    struct TrainPass newPass;

    printf("Enter Name: ");
    scanf("%s", newPass.name);

    printf("Enter Age: ");
    scanf("%d", &newPass.age);

    printf("Enter Source: ");
    scanf("%s", newPass.source);

    printf("Enter Destination: ");
    scanf("%s", newPass.destination);

    printf("Enter Validity (in months): ");
    scanf("%d", &newPass.validity);

    newPass.travelCount = 10; // Fixed as per your requirement

    // Check if the stack is not full before pushing the new pass
    if (top < MAX_USERS - 1) {
        push(newPass); // Push the new pass onto the stack

        printf("\033[1;32m"); // Set text color to green
        printf("Train pass generated successfully!\n");
        printf("\033[0m"); // Reset text color to default

        // Display the generated train pass in a box format
        printf("\n\033[1;34m"); // Set text color to blue
        printf("Generated Train Pass:\n");
        printf("*\n");
        printf("* Name: %-17s \n", newPass.name);
        printf("* Age: %-18d \n", newPass.age);
        printf("* Source: %-15s \n", newPass.source);
        printf("* Destination: %-10s \n", newPass.destination);
        printf("* Validity: %-15d \n", newPass.validity);
        printf("* Travels: %-14d \n", newPass.travelCount);
        printf("*Price:500\n");
        printf("*\n");
        printf("\033[0m"); // Reset text color to default
    } else {
        printf("\033[1;31m"); // Set text color to red
        printf("Error: Train pass stack is full!\n");
        printf("\033[0m"); // Reset text color to default
    }
}

// Function to calculate energy consumption based on ticket price and distance
double calculateEnergyConsumption(int ticketPrice, int distance) {
    double percentagePrice = ticketPrice * 0.20; // 20% of the ticket price
    double energyConsumption = percentagePrice * 8.2 / distance;
    return energyConsumption;
}
double calculateCarbonFootprint(double energyConsumption) {
    // Assuming a conversion factor from energy consumption to carbon footprint
    double carbonFactor = 0.05;  // Adjust this value based on your scenario
    double carbonFootprint = energyConsumption * carbonFactor;
    return carbonFootprint;
}

// ... (Previous code)
#define MAX_USERS 100
#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20

struct UserCredentials {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    bool isLoggedIn;
};

struct UserCredentials users[MAX_USERS];
int userCount = 0;

// Function to check if a username already exists
bool isUsernameTaken(const char *username) {
	int i;
    for (  i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return true;
        }
    }
    return false;
}

// Function to create a new user account
void signUp() {
    if (userCount == MAX_USERS) {
        printf("Maximum number of users reached. Cannot sign up.\n");
        return;
    }

    struct UserCredentials newUser;
    printf("Enter username: ");
    scanf("%s", newUser.username);

    if (isUsernameTaken(newUser.username)) {
        printf("\nUsername already taken. Please choose a different one!!!.\n");
        return;
    }

    printf("Enter password: ");
    scanf("%s", newUser.password);

    newUser.isLoggedIn = false;

    users[userCount++] = newUser;

    printf("\nSIGN IN SUCCESSFULL!!!.\n\n");
}

// Function to log in an existing user
void login() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);
int i;
    for ( i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            users[i].isLoggedIn = true;
            printf("\nLOGIN SUCCESSFULL!!!\n\nWelcome back!\nWe're thrilled to see you again.\nHappy navigating!\n\n");
            func();
            return;
        }
    }

    printf("Invalid username or password.\n");
}

// Function to log out the current user
void logout() {
	int i;
    for ( i = 0; i < userCount; i++) {
        if (users[i].isLoggedIn) {
            users[i].isLoggedIn = false;
            printf("Logged out successfully.\n");
            return;
        }
    }
}
int func() {
    char districts[MAX_DISTRICTS][20] = {"Chennai", "Coimbatore", "Madurai", "Trichy", "Tirunelveli", "Salem", "Erode",
                                         "Tiruppur", "Vellore", "Tambaram", "Neyveli", "Dindigul", "Cuddalore", "Kancheepuram",
                                         "Karur", "Ramanathapuram", "Ooty", "Thanjavur", "Thoothukudi", "Kanyakumari", "Namakkal",
                                         "Virudhunagar", "Krishnagiri", "Sivaganga", "Tiruvannamalai", "Pollachi", "Rameswaram",
                                         "Nagapattinam", "Dharmapuri", "Hosur", "Tirupathur", "Ariyalur"};
                                         struct User users[MAX_USERS];
                                         
                                         
    int graph[MAX_DISTRICTS][MAX_DISTRICTS] = {
        {0, 100, 0, 50, 0},
        {0, 0, 80, 0, 0},
        {0, 0, 0, 0, 70},
        {0, 0, 0, 0, 120},
        {0, 0, 0, 0, 0}
    };
int userCount = 0;
struct TrainUserDetails trainUserDetails[MAX_DISTRICTS];
struct TreeNode* bstRoot = NULL;
    
     int V = MAX_DISTRICTS;
    int dist[MAX_DISTRICTS];
    int i;
      struct Train trains[] = {
        // ... (Previous train details)
        {"Train A", 101, "Chennai", "Coimbatore", "8:00", "10:15", 450},
    {"Train B", 102, "Chennai", "Madurai", "9:00", "12:30", 350},
    {"Train C", 103, "Chennai", "Madurai", "14:00", "17:30", 300},
    {"Train D", 104, "Coimbatore", "Trichy", "11:15", "13:30", 180},
    {"Train E", 105, "Coimbatore", "Salem", "15:00", "17:15", 100},
    {"Train F", 106, "Madurai", "Chennai", "18:30", "21:45", 400},
    {"Train G", 107, "Madurai", "Trichy", "8:45", "10:30", 150},
    {"Train H", 108, "Trichy", "Chennai", "12:00", "15:15", 250},
    {"Train I", 109, "Trichy", "Madurai", "17:30", "19:45", 200},
    {"Train J", 110, "Salem", "Chennai", "13:45", "16:00", 200},
    {"Train K", 111, "Salem", "Coimbatore", "18:15", "20:30", 130},
    {"Train L", 112, "Erode", "Chennai", "10:00", "12:15", 220},
    {"Train M", 113, "Erode", "Trichy", "14:30", "16:45", 120},
    {"Train N", 114, "Tiruppur", "Chennai", "16:45", "19:00", 150},
    {"Train O", 115, "Tiruppur", "Coimbatore", "20:30", "22:45", 70},
    {"Train P", 116, "Vellore", "Chennai", "9:15", "11:30", 50},
    {"Train Q", 117, "Vellore", "Trichy", "14:45", "17:00", 90},
    {"Train R", 118, "Tambaram", "Chennai", "7:30", "9:45", 60},
    {"Train S", 119, "Tambaram", "Coimbatore", "11:45", "14:00", 130},
    {"Train T", 120, "Neyveli", "Chennai", "16:00", "18:15", 70},
    {"Train U", 121, "Neyveli", "Trichy", "19:30", "21:45", 110},
    {"Train V", 122, "Dindigul", "Chennai", "8:30", "10:45", 150},
    {"Train W", 123, "Dindigul", "Madurai", "12:15", "14:30", 120},
    {"Train X", 201, "Trichy", "Tirunelveli", "12:30", "15:45", 220},
    {"Train Y", 202, "Tirunelveli", "Chennai", "17:00", "19:15", 300},
    {"Train Z", 203, "Chennai", "Kanyakumari", "21:30", "23:45", 350},
    // ... add more trains ...
    {"Train AA", 301, "Kanyakumari", "Chennai", "5:45", "8:00", 400},
    {"Train AB", 302, "Chennai", "Rameswaram", "9:30", "11:45", 250},
    {"Train AC", 303, "Rameswaram", "Chennai", "14:15", "16:30", 300},
    {"Train AD", 304, "Chennai", "Nagapattinam", "18:45", "21:00", 200},
    {"Train AE", 305, "Nagapattinam", "Chennai", "22:00", "23:45", 150},
    {"Train AF", 306, "Chennai", "Dharmapuri", "6:15", "8:30", 180},
    {"Train AG", 307, "Dharmapuri", "Coimbatore", "10:30", "12:45", 230},
    {"Train AH", 308, "Coimbatore", "Kancheepuram", "13:15", "15:30", 160},
    {"Train AI", 309, "Kancheepuram", "Chennai", "16:45", "19:00", 90},
    {"Train AJ", 310, "Chennai", "Krishnagiri", "20:30", "22:45", 120},
    {"Train AK", 311, "Krishnagiri", "Tirupathi", "23:00", "1:15", 200},
    {"Train AL", 312, "Tirupathi", "Chennai", "5:30", "7:45", 180}
};


    for (i = 0; i < sizeof(trains) / sizeof(trains[0]); i++) {
        bstRoot = insertNode(bstRoot, trains[i]);
    }
 while (1) {
        printf("\n\033[1;33m"); // Set text color to yellow
        printf("SERVICES!!\n\n");
        printf("1. ENQUIRE TRAIN\n");
        printf("2. TICKET PRICE\n");
        printf("3. FIRST TRAIN\n");
        printf("4. BOOKING\n");
        printf("5. CANCELLATION\n");
        printf("6. TRAIN PASS\n");
        printf("7. ENERGY CONSUMPTION\n");
        printf("8. CARBON FOOTPRINTS\n");
        printf("9. EXIT\n\n");  // New option for train pass generator
        printf("\033[0m"); // Reset text color to default

        int choice;
        printf("\033[1;32m"); // Set text color to green
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        printf("\033[0m"); // Reset text color to default

        if (choice == 1) {
           char source[20], destination[20];
            printf("\033[1;34m"); // Set text color to blue
            printf("\nEnter source district: ");
            scanf("%s", source);

            // Check if the entered source district is valid
            int sourceIndex = findDistrictIndex(districts, V, source);
            if (sourceIndex == -1) {
                printf("\033[1;31m"); // Set text color to red
                printf("Error: Invalid source district. Please enter a valid district.\n");
                printf("\033[0m"); // Reset text color to default
                continue;  // Ask the user for input again
            }

            printf("Enter destination district:");
            scanf("%s\\n\n", destination);

            // Check if the entered destination district is valid
            int destIndex = findDistrictIndex(districts, V, destination);
            if (destIndex == -1) {
                printf("\033[1;31m"); // Set text color to red
                printf("Error: Invalid destination district. Please enter a valid district.\n");
                printf("\033[0m"); // Reset text color to default
                continue;  // Ask the user for input again
            }

            // Display train details
            printTrainDetails(trains, sizeof(trains) / sizeof(trains[0]), source, destination);

            // Ask the user to select a train seat
            int trainNumber, seatClass, seat;
            printf("\nEnter the train number: ");
            scanf("%d", &trainNumber);
            printf("Select seat class (1. First Class, 2. Second Class): ");
            scanf("%d", &seatClass);
            printf("Select number of seats: ");
            scanf("%d", &seat);

            // Find the distance covered by the selected train
            int distance = 0;
            int i;
            for ( i = 0; i < sizeof(trains) / sizeof(trains[0]); i++) {
                if (trains[i].trainNumber == trainNumber) {
                    distance = trains[i].distanceCovered;
                    break;
                }
            }

            // Calculate ticket price based on the distance and seat class
            int ticketPrice = calculateTicketPrice(distance, seatClass, seat);
            printf("\033[1;32m"); // Set text color to green
            printf("\n\nTicket price for train %d (Seat Class %d): %d\n\n", trainNumber, seatClass, ticketPrice);
            printf("\033[0m"); // Reset text color to default
        
        } // ... (Previous code)

else if (choice == 2) {
    int trainNumber, distance;
    printf("\033[1;34m"); // Set text color to blue
    printf("\nEnter train number: ");
    scanf("%d", &trainNumber);

    // Check if the entered train number is valid
    if (trainNumber < 101 || trainNumber > 312) {
        printf("\033[1;31m"); // Set text color to red
        printf("Invalid train number. Please enter a valid train number.\n");
        printf("\033[0m"); // Reset text color to default
        continue;  // Ask the user for input again
    }

    // Find the distance covered by the selected train
    int i;
    for (i = 0; i < sizeof(trains) / sizeof(trains[0]); i++) {
        if (trains[i].trainNumber == trainNumber) {
            distance = trains[i].distanceCovered;
            break;
        }
    }

    // Calculate ticket price based on the distance (defaulting to second class)
    int ticketPrice = calculateTicketPrice(distance, 2, 1);
    printf("\033[1;32m"); // Set text color to green
    printf("\nTicket price for train %d (Default Second Class): %d\n\n", trainNumber, ticketPrice);
    printf("\033[0m"); // Reset text color to default
}
 // ... (Previous code)

else if (choice == 7) {
    int trainNumber, distance;
    printf("\nEnter train number: ");
    scanf("%d", &trainNumber);

    // Check if the entered train number is valid
    if (trainNumber < 101 || trainNumber > 312) {
        printf("\033[1;31m"); // Set text color to red
        printf("Invalid train number. Please enter a valid train number.\n");
        printf("\033[0m"); // Reset text color to default
        continue;  // Ask the user for input again
    }

    // Find the distance covered by the selected train
    int i;
    for (i = 0; i < sizeof(trains) / sizeof(trains[0]); i++) {
        if (trains[i].trainNumber == trainNumber) {
            distance = trains[i].distanceCovered;
            break;
        }
    }

    // Calculate ticket price based on the distance (defaulting to second class)
    int ticketPrice = calculateTicketPrice(distance, 2, 1);

    // Calculate and print energy consumption
    double energyConsumption = calculateEnergyConsumption(ticketPrice, distance);
    printf("\nEnergy consumption for train %d: %.2f Kw/Km\n\n", trainNumber, energyConsumption);
}

		
else if (choice == 4) {
    printf("\n\033[1;35m"); // Set text color to magenta
    printf("Booking Details:\n");
    
    int i;
    for (i = 0; i < userCount; i++) {
        printf("%-20s%-50s%-10d%-10d\n", users[i].name, users[i].address, users[i].age, users[i].choice);
    }
    printf("\033[0m"); // Reset text color to default

    char bookTickets;
            printf("\033[1;34m"); // Set text color to blue
            printf("Do you want to book tickets? (y/n): ");
            scanf(" %c", &bookTickets);
            printf("\033[0m"); // Reset text color to default

            while (bookTickets == 'y' || bookTickets == 'Y') {
                // ... (Previous code)
                 int trainNumber;
        printf("\033[1;34m"); // Set text color to blue
        printf("Enter train number: ");
        scanf("%d", &trainNumber);
        printf("\033[0m"); // Reset text color to default

        if (trainNumber < 101 || trainNumber > 203) {
            printf("\033[1;31m"); // Set text color to red
            printf("Invalid train number. Please enter a valid train number.\n");
            printf("\033[0m"); // Reset text color to default
            continue;  // Ask the user for input again
        }

        struct User newUser;
        printf("\n\033[1;34m"); // Set text color to blue
        printf("Enter your details:\n");
        printf("Name: ");
        scanf("%s", newUser.name);
        printf("Address: ");
        scanf("%s", newUser.address);
        printf("Age: ");
        scanf("%d", &newUser.age);
       

                char phoneNumber[15];
                printf("\033[1;34m"); // Set text color to blue
                printf("Enter your phone number: ");
                scanf("%s", phoneNumber);
                printf("\033[0m"); // Reset text color to default

                if (trainUserDetails[trainNumber - 1].userCount < MAX_USERS) {
                    // Insert the booking details into the linked list
                    insertBookingNode(&trainUserDetails[trainNumber - 1].head, newUser, phoneNumber);

                    printf("\033[1;32m"); // Set text color to green
                    printf("\n\nUser details added successfully for Train %d!\n", trainNumber);
                     printf("Welcome!!! To contribute to sustainable train transportation:\n");
    printf("1. Opt for e-tickets to reduce paper usage.\n");
    printf("2. Travel during off-peak hours to help balance energy consumption.\n");
    printf("3. Dispose of waste responsibly; use designated bins for recycling.\n");
    printf("4. Conserve energy by turning off lights and electronic devices when not in use.\n");
    printf("5. Support eco-friendly rail companies that prioritize sustainability.\n");
    printf("6. Share your positive experience with sustainable travel on social media!\n");
    printf("Let's make every journey an eco-friendly one!\n");
                    printf("\033[0m"); // Reset text color to default
                } else {
                    printf("\033[1;31m"); // Set text color to red
                    printf("Error: Maximum number of users reached for Train %d!\n", trainNumber);
                    printf("\033[0m"); // Reset text color to default
                }

                // Ask if the user wants to book another ticket
                printf("\n\033[1;34m"); // Set text color to blue
                printf("Do you want to book another ticket? (y/n): ");
                scanf(" %c", &bookTickets);
                printf("\033[0m"); // Reset text color to default
            }
} 

   else if (choice == 3) {
        char source[20], destination[20];
        printf("\033[1;34m"); // Set text color to blue
        printf("\nEnter source district: ");
        scanf("%s", source);

        // Check if the entered source district is valid
        int sourceIndex = findDistrictIndex(districts, V, source);
        if (sourceIndex == -1) {
            // ... (Error handling for invalid source district)
            continue;
        }

        printf("Enter destination district: ");
        scanf("%s", destination);

        // Check if the entered destination district is valid
        int destIndex = findDistrictIndex(districts, V, destination);
        if (destIndex == -1) {
            // ... (Error handling for invalid destination district)
            continue;
        }

        // Display train details
        printf("\n\033[1;35m"); // Set text color to magenta
        printf("\nTrain with the earliest departure time from %s to %s:\n\n", source, destination);
        findEarliestDeparture(bstRoot, source, destination);
        printf("\033[0m"); // Reset text color to default
    }
   // ... (Previous code)

else if (choice == 8) {
    int trainNumber, distance;
    printf("\nEnter train number to check the carbon footprint: ");
    scanf("%d", &trainNumber);

    // Check if the entered train number is valid
    if (trainNumber < 101 || trainNumber > 312) {
        printf("\033[1;31m"); // Set text color to red
        printf("Invalid train number. Please enter a valid train number.\n");
        printf("\033[0m"); // Reset text color to default
        continue;  // Ask the user for input again
    }

    // Find the distance covered by the selected train
    int i;
    for (i = 0; i < sizeof(trains) / sizeof(trains[0]); i++) {
        if (trains[i].trainNumber == trainNumber) {
            distance = trains[i].distanceCovered;
            break;
        }
    }

    // Calculate ticket price based on the distance (defaulting to second class)
    int ticketPrice = calculateTicketPrice(distance, 2, 1);

    // Calculate energy consumption
    double energyConsumption = calculateEnergyConsumption(ticketPrice, distance);

    // Calculate and print carbon footprint
    double carbonFootprint = calculateCarbonFootprint(energyConsumption);
    printf("\nCarbon footprint for train %d: %.2f KgCO2/Km\n\n", trainNumber, carbonFootprint);
}
//else if (choice == 7) {
          else if (choice == 5) {
            printf("\n\033[1;35m"); // Set text color to magenta
            printf("Cancel Ticket:\n");

            // Prompt the user for the train number
            int trainNumber;
             printf("\033[1;34m");
            printf("\nEnter train number for cancellation: ");
            scanf("%d", &trainNumber);

            char phoneNumber[15];
            printf("Enter your phone number for cancellation: ");
            scanf("%s", phoneNumber);
            printf("\033[0m"); // Reset text color to default

            // Call the cancelTicket function to cancel the ticket based on the provided phone number
            cancelTicket(&trainUserDetails[trainNumber - 1].head, phoneNumber);
        }

     else if (choice == 9) {
            break;
        }
    else if (choice == 6) {
            generateTrainPass();
        }
    
        
		else {
            printf("\033[1;31m"); // Set text color to red
            printf("Invalid choice. Please enter a valid option.\n");
            printf("\033[0m"); // Reset text color to default
        }
    }
    return 0;
}
void printCenteredText(const char* text) {
    // Get the width of the console
    int consoleWidth = 80; // You can adjust this value based on your console width

    // Calculate the number of spaces needed for center alignment
    int textWidth = strlen(text);
    int padding = (consoleWidth - textWidth) / 2;

    // Print the padded spaces followed by the text
    int i;
    for ( i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

int main() {
	printf("\n\n");
     printf("WELCOME TO SUSTAINABLE TRANSPORTATION\n");
       printf("*************\n\n");
    while (1) {
        printf("\n\033[1;33m"); // Set text color to yellow
        printf("MENU:\n\n");
        printf("1. SIGN IN\n");
        printf("2. LOGIN\n");
        printf("3. LOGOUT\n");
        printf("4. EXIT\n\n");
        printf("\033[0m"); // Reset text color to default

        int choice;
        printf("\033[1;32m"); // Set text color to green
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\033[0m"); // Reset text color to default

        switch (choice) {
            case 1:
                signUp();
                break;
            case 2:
                login();
                break;
            case 3:
                logout();
                break;
            case 4:
            printf("\n\nTHANK YOU!!! \n HAVE A SUCCESSFULL JOURNEY!!");
                return 0;
            default:
                printf("\033[1;31m"); // Set text color to red
                printf("Invalid choice. Please enter a valid option.\n");
                printf("\033[0m"); // Reset text color to default
        }
    }

    return 0;
}
