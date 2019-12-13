#include <string>
using namespace std;


/************************************************************************/
//  Programming #1 
//
//  List below are function prototypes of functions given  or required 
//		to be implemented in Programming #1 
//
/************************************************************************/



double prCharGivenCharOfState(char charGenerated, char charOfTheState);
/************************************************************************/
//Calculate and return the probability of charGenerated actually typed
//given charOfTheState as the underlying cognitive state. 
/************************************************************************/


void getPrTableForPossibleInitialStates(double prTable[], int sizeOfTable);
/************************************************************************/
//Calculate for each actual cognitive state in a word of sizeOfTable characters,
//	the probability of that cognitive state being the actual first cognitive state
//	when the user types the word.
//Store these prbabilities in the prTable array.
/************************************************************************/


void getPrTableForPossibleNextStates
(double transitionPrTable[], int sizeOfTable, int currentState);
/************************************************************************/
//Calculate for each actual cognitive state in a word of sizeOfTable characters,
//	the transition probability of that cognitive state being the next cognitive state
//	given currentState as the current state.
//Store these prbabilities in the transitionPrTable[] array.
/************************************************************************/


void setParametersSpellingModel();
/************************************************************************/
//Reset the parameters of the spelling model
/************************************************************************/


void displayParametersSpellingModel();
/************************************************************************/
//Display the parameters of the spelling model
/************************************************************************/



void setParametersKbModel();
/************************************************************************/
//Reset the parameters of the keyboard model
/************************************************************************/



void displayParametersKbModel();
/************************************************************************/
//Display the parameters of the keyboard model
/************************************************************************/





/************************************************************************/
//  Programming #2 
//
//  List below are function prototypes of new functions given  or required 
//		to be implemented in model.cpp for Programming #2 
//
/************************************************************************/



int take1SampleFrom1PrSpace(double prTable[], int sizeOfTable);
/************************************************************************/
//Given the probabilities (of sizeOfTable elements) stored in prTable,
//	try to randomly take a sample out of sizeOfTable elements
//	according to the probabilities of sizeOfTable elements;
//Return the index of the non-deterministically sampled element.
/************************************************************************/
//This function is actually already implemented in model.cpp.
//	However, if you choose to implement this function on your own from scratch
//	you'll get an extra point.
/************************************************************************/


void getKeyboardProbabilityTable(char charToType, double prTable[]);
/************************************************************************/
//
//Given the character to type (charToType) 
//	(assuming that the 1D keyboard of 26 keys is used),
//	(assuming that prTable[] for storing 26 prbabilities),
//	calculate pr(charGenerated = 'a' | charToType),
//	calculate pr(charGenerated = 'b' | charToType), 
//	...
//	calculate pr(charGenerated = 'z' | charToType), and
//	store these probabilities in prTable.
/************************************************************************/
	



char typeOneChar(char charToType);
/************************************************************************/
//Simulate the keyboard model:
//Given the charToType, simulate what character may actually be typed 
//	when the user trying to type charToType and return it as the result.
/************************************************************************/


void typeOneWord(char word[], char output[], bool traceON = false, int maxOutput=100);
/************************************************************************/
//Simulate the combination of the spelling model and the keyboard model:
//Given a word stored in the word array, simulate what may actually
//	be typed and store the result in the output array.
//maxOutput specifies the capacity limit of the output array, by default it is 100.
//When traceON is true (by default it is false), extra outputs are provided as traces.
/************************************************************************/


/************************************************************************/
// Also see below how option A in the main function in demo.cpp 
//		call typeOneChar and typeOneWord to simulate the typing behavior
/************************************************************************/

void typeOneArticle(char * corruptedMessageFile, char * sourceArticle, bool trace = false);


/*

int main()
{
	...
	...

	case 'A': case 'a':// 
		{
			displayParametersKbModel();
			displayParametersSpellingModel();

			char charToType;
			cout << endl << "Now give me a character to type for 10 times:";
			cin >> charToType;

			for (int i=0; i<10; i++)
			{	
				cout << endl << "[" << i+1 << "]: get \"" << typeOneChar(charToType) 
					 << "\" while trying to type \"" << charToType << "\"" << endl << endl << endl;
			}

			char word[21];
			char output[101];
	
			cout << endl << "Now give me a word (upto 20 characters) to type (10 times):";
			cin >> word;

			for (int i=0; i<10; i++)
			{
				typeOneWord(word, output, true);
				cout << endl << "[" << i+1 << "]: get \"" << output 
					 << "\" while trying to type \"" << word << "\"" << endl << endl << endl;
			}

		}
		endOfService("[Simulation of typing behavior.]");

		....
*/

double prOf1CharSeriesWhenTyping1Word(string observedString, string wordString);
double logPrOfGettingDocument1WhenTypingDocument2(string document1, string document2);
void bestParametersOfGettingDocument1WhenTypingDocument2(string document1, string document2);
void tryToRecoverMessageFromcorruptedMessage();
void tryToRecoverMessageFromcorruptedMessage2();
void checkPrecision();
