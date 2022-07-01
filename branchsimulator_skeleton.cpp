/*
 * Programmer: Nnamdi Nwaokorie 
 * Net-ID: npn225
 * University-School: New York University - Tandon School of Engineering 
 * Course: Computer Systems Architecture (ECE-GY 6913)
 * Instructor: Brandon Reagen
 * Date: November 19, 2020
 * 
 * Special Thanks to Course TA Jiazhen Han
 */

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

int extract_bits( int number, int num_of_bits, int position ) {
    // This function takes the given number, extracts the number of bits 
    // from the given position, and returns the decimal form of the 
    // extracted bits.  
    // Example: For inputs - number = 72, num_of_bits = 5, and position = 0, 
    //          the extracted number is 8, since 72 == 1001000 and 
    //          8 == 01000
    
    return ( ( (1 << num_of_bits) - 1 ) & ( number >> position ) );
    
}

int main (int argc, char** argv) {
	ifstream config;
        
        // Important NOTE: These two lines of code must be un-commented
        //                 in order for this code to run in an IDE
        // The two lines of code below MUST be REMOVED when this code is submitted
         (argv[1]) = (char*)"config.txt";
         (argv[2]) = (char*)"trace.txt";
        
	config.open(argv[1]);

        // Written by Instructor - Get the needed k and m values 
        // k is the number of bits in the saturating counter
        // m is the number of least significant bits from the PC Address
	int m, k;
	config >> m >> k;
        
        // Written by Instructor - Initialize both the PC address and the taken bit
        unsigned long pc; bool taken;
        
        // Use the m value to get the size of the vector
        int vector_size = pow( 2, m );
        
        
        // Initialize the vector with the size of 2^m values
        vector<int> entries_vect( vector_size );
        
        // Initialize the vector with the value of all bits of 11
        // Note: Zero (0) represents 00, One (1) represents 01, 
        //       Two (2) represents 10, and Three (3) represents 11
        fill( entries_vect.begin(), entries_vect.end(), 3 );
        
        // Initialize the prediction and misprediction counters
        double pred_count = 0, mispred_count = 0;
        
 
	config.close();

	ofstream out;
	string out_file_name = string(argv[2]) + ".out";
	out.open(out_file_name.c_str());
	
	ifstream trace;
	trace.open(argv[2]);
        
        // While-Loop Altered by Student-Programmer - Loop through the trace.txt file
	while ( trace >> std::hex >> pc >> taken ) {
            
                // Get the index for the appropriate entry
                int entry_index = extract_bits( pc, m, 0 );
                
                // Access the appropriate entry in the vector by 
                // using the derived index
                int counter_value = entries_vect[ entry_index ];
                
                cout << "Taken Bit: " << taken << " - Counter Value 1: " << counter_value;
		
                // Written by Instructor - Initialize the prediction bool
                // Prediction bool is true if the branch is predicted to be taken
                // Prediction bool is false if the branch is predicted to be NOT-taken
                bool prediction;
		prediction = false;
                
                // Test to see if the correct prediction has been made
                
                // If the branch is Taken but predicted to be 
                // either Strongly NOT-Taken (00) or Weakly NOT-Taken (01) 
                if ( taken && ( (counter_value == 0) || (counter_value == 1) ) ) {
                    
                    // Increment the vector entry
                    ++entries_vect[ entry_index ];
                    
                    // Set the prediction bool to false 
                    prediction = false;
                    
                    // Increment the misprediction counter
                    ++mispred_count;
                    
                // If the branch is Taken and predicted to be Weakly Taken (10)
                } else if ( taken && (counter_value == 2) ) {
                
                    // Increment the vector entry
                    ++entries_vect[ entry_index ];
                    
                    // Set the prediction bool to true 
                    prediction = true;
                    
                // If the branch is Taken and predicted to be Strongly Taken (11)
                } else if ( taken && (counter_value == 3) ) {
                
                    // Set the prediction bool to true 
                    prediction = true;
                    
                // If the branch is NOT-Taken and predicted to be Strongly NOT-Taken (00) 
                } else if ( !taken && (counter_value == 0) ) {
                
                    // Set the prediction bool to false
                    prediction = false;
                    
                // If the branch is NOT-Taken and predicted to be Weakly NOT-Taken (01) 
                } else if ( !taken && (counter_value == 1) ) {
                
                    // Decrement the vector entry
                    --entries_vect[ entry_index ];
                    
                    // Set the prediction bool to false
                    prediction = false;
                    
                // If the branch is NOT-Taken but predicted to be 
                // either Weakly Taken (10) or Strongly Taken (11) 
                } else if ( !taken && ( (counter_value == 2) || (counter_value == 3) ) ) {
                
                     // Decrement the vector entry
                    --entries_vect[ entry_index ];
                    
                    // Set the prediction bool to true 
                    prediction = true;
                    
                    // Increment the misprediction counter
                    ++mispred_count;
                                   
                }
                
                // Increment the prediction counter for each prediction made
                ++pred_count;
                
                cout << " - Counter Value 2: " << entries_vect[ entry_index ] << endl;
                
		// Written by Instructor - Output results to the trace.txt.out file
                // NOTE: The "endl" was added by the student programmer 
                out << prediction << endl;
	}
        
	
        // Output important values to make certain everything is correct 
        cout << endl << "Vector Size: " << vector_size << endl;
        cout << "m value: " << m << endl;
        cout << "k value: " << k << endl << endl; 
        // cout << "argv[1] value: " << argv[1] << endl;
        
        // Output the Number of Mispredictions, the Number of Predictions, 
        // And the Misprediction Rate 
        cout << "(A) - Number of Mispredictions: " << mispred_count << endl;
        cout << "(B) - Total Number of Predictions: " << pred_count << endl;
        printf( "(C) - Misprediction Rate (C = A/B): %f", ( mispred_count/pred_count ) );
        
	trace.close();	
	out.close();
}
