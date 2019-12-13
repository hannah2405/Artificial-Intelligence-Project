#include <ctime>
#include <cstring>
#include <string>
#include <iostream>
#include <conio.h>	
#include <fstream>
//For windows, include <conio.h> to use _getch().
//Instead of using _getch() and <conio.h>, 
//	you can declare a chacter variable "c" and use "cin >> c"
//  to read in a character.

using namespace std;

#include "models.h"
//to use the implementation of the models, you have to include <conio.h>


/************************************************************************/
/************************************************************************/
void demoOfKeyboardModel()
{
	cout << endl << endl 
		 << "*********************************************************" << endl
		 << "Test the correctness of the keyboard model implementation, and" << endl
		 << "show the probabilities of generating (touching) various characters" << endl 
		 << "when mentally you are in a state of trying to type something." << endl
		 << "*********************************************************" << endl; 
	cout<<endl<<"Press space to continue."<<endl;	

	_getch();
	char asciiMap[] = "abcdefghijklmnopqrstuvwxyz";
	for (int i=0; i< 26; i++)
	{double prSum;
	 prSum = 0;
	 cout << "Probability distribution regarding " << endl
		  << "all possible resulting characters when trying to type "
		  << asciiMap[i] << endl;
	 for (int j=0; j< 26; j++)
	{	
			cout << '\t' 
				 << "Pr[Char=" << asciiMap[j] << "|State=" << asciiMap[i] << "]="
				 << prCharGivenCharOfState(asciiMap[j], asciiMap[i]) << endl;
			prSum += prCharGivenCharOfState(asciiMap[j], asciiMap[i]);
	}
	 cout << "The sum of probability distribution over " << endl
		  << "all possible resulting characters when trying to type \'" << endl
		  << asciiMap[i] 
		  << "\' has been verified as " 
		  << prSum  << endl << endl;

	 cout<<endl<<"Press space to continue."<<endl;		
	 _getch();

	}
	cout << endl << endl ;
}



/************************************************************************/
/************************************************************************/
void demoInitialProbaility()
{
	
	cout << endl << endl 
		 << "*********************************************************" << endl
		 << "Consider the 3-character word \"his\" "<< endl
		 << "Demonstrate the probability distribution over the possible cognitive states" << endl
		 << "after the transition from the special initial state I."  << endl
		 << "*********************************************************" << endl; 
	cout<<endl<<"Press space to continue."<<endl;		
	_getch();

	int sizeOfTable = strlen("his");
	int currentState;
	double prSum = 0;
	double initialPrTable[3];
	getPrTableForPossibleInitialStates(initialPrTable,3);

	string word = "his";
	for (currentState=0; currentState < sizeOfTable; currentState++)
	{	cout << '\t' 
				 << "Pr[State=" << word[currentState]  << "]="
				 << initialPrTable[currentState] << endl;
			prSum += initialPrTable[currentState];
	}

	cout << "The sum of probability distribution over " << endl
		 << "all possible states after the first transition" << endl
		 << "has been verified as " << prSum  << endl << endl;
	cout<<endl<<"Press space to continue." << endl;		
	_getch();

}


/************************************************************************/
/************************************************************************/
void demoTransitionProbability()
{

	cout << endl << endl 
		 << "*********************************************************" << endl
		 << "Consider the 3-character word \"his\" "<< endl
		 << "Show the transition probabilities when spelling the word." << endl
		 << "*********************************************************" << endl
		 << "Demonstrate transition probabilities from current state to the next state."  << endl
		 << "We use special character \'_\' to represent the special final state F."  << endl
		 << "*********************************************************" << endl;  
	cout<<endl<<"Press space to continue."<<endl;		
	_getch();

	//One more example
	int sizeOfSpace = strlen("his")+1;
	string word="his_";
	for (int currentState=0; currentState< sizeOfSpace-1; currentState++)
	{	double prSum, transitionPrTable[4];
		cout << "Probability distribution over " << endl
			<< "all possible next states given that the current state is "
			<< word[currentState] << endl;

		prSum = 0;
		//nextStatePrTableGivenCurrentState(currentState,transitionPrTable,6);
		getPrTableForPossibleNextStates
		(transitionPrTable, 4, currentState);

		for (int nextState=0; nextState< sizeOfSpace; nextState++)
		{	cout << '\t' 
				 << "Pr[NextState=" << word[nextState] << "|CurrentState=" << word[currentState] << "]="
				 << transitionPrTable[nextState] << endl;
			prSum += transitionPrTable[nextState];
		}

		cout << "The sum of probabilities over " << endl
		  << "all possible next states given that CurrentState="
		  << word[currentState] << endl
		  << "is verified as " 
		  << prSum  << endl << endl;

		cout<<endl<<"Press space to continue."<<endl;		
		_getch();
	}


	//One more example
	cout << endl << endl 
		 << "*********************************************************" << endl
		 << "Consider the 5-character word \"right\" "<< endl
		 << "Show the transition probabilities when spelling the word." << endl
		 << "*********************************************************" << endl
		 << "Demonstrate transition probabilities from current state to the next state."  << endl
		 << "We use special character \'_\' to represent the special final state F."  << endl
		 << "*********************************************************" << endl;  
	cout<<endl<<"Press space to continue."<<endl;		
	_getch();
	sizeOfSpace = strlen("right")+1;
	word="right_";
	for (int currentState=0; currentState< sizeOfSpace-1; currentState++)
	{	double prSum, transitionPrTable[6];
		cout << "Probability distribution over " << endl
			<< "all possible next states given that the current state is "
			<< word[currentState] << endl;

		prSum = 0;
		//nextStatePrTableGivenCurrentState(currentState,transitionPrTable,6);
		getPrTableForPossibleNextStates
		(transitionPrTable, 6, currentState);

		for (int nextState=0; nextState< sizeOfSpace; nextState++)
		{	cout << '\t' 
				 << "Pr[NextState=" << word[nextState] << "|CurrentState=" << word[currentState] << "]="
				 << transitionPrTable[nextState] << endl;
			prSum += transitionPrTable[nextState];
		}

		cout << "The sum of probabilities over " << endl
		  << "all possible next states given that CurrentState="
		  << word[currentState] << endl
		  << "is verified as " 
		  << prSum  << endl << endl;

		cout<<endl<<"Press space to continue."<<endl;		
		_getch();
	}

}

void endOfService(const string service)
{
	// displayProgram();
	cout<<endl << service << " done." << endl << "Press any key to continue.";
	_getch();
	cout << endl;
	cout << "****************************************************" <<endl<< endl;

}

/************************************************************************/
/************************************************************************/
void main()
{
	srand( (unsigned)time( NULL ) );

	// "Then I suppose now is the time for me to say something profound....
	// Nothing comes to mind. Let's do it."

	bool inMenu = true;	// while the user is using the program this var is set to true and the menu keeps looping.
						// when the user wants to quit, it is set to false and the loop ends.
	char keyPressed;	// This var stores which menu selection the user picks.

	while ( inMenu )
	{
		cout<< endl << endl << endl;
		cout << "****************************************************" << endl;
		cout  <<"**  MENU:(press a character to select an option)  **" << endl;
 		cout << "****************************************************" << endl;
		cout<<"Q. [QUIT]     Quit."<<endl;
		cout << endl;

 		cout << "*************	Demos of the Models	*********" << endl;
		cout<<"K. Keyboard model demo"<<endl;
		cout<<"S. Spelling model demo"<<endl;
		cout<<"A. Simulate typing of a word"<<endl;
		// cout<<"H. Keyboard+Spelling HMM model demo"<<endl;
		 cout<<"P. prOf1CharSeriesObservedWhenTyping1Word(string observed, string word)"<<endl;
		 cout << "D. logPrOfGettingDocument2WhenTypingDocument1" << endl;
		 cout << "L. bestParametersOfGettingDocument1WhenTypingDocument2" << endl;

		 cout << endl;

		cout << "****  Typing and Spelling Recognition Testings	 ****" << endl;
		//cout<<"T. Test typing a message of 20 random words" << endl;
		cout<<"B. Test typing the whole Biola vision article twice" << endl;
		//cout<<"W. Test automatic recognition of an observed strings as word"<<endl;
		cout << "R. Test automatic recovery of corrupted message generated" << endl;
		cout << "T. Test automatic recovery of corrupted message generated" << endl
			<< "   with 2 Documents" << endl;
		cout << "U. Calculate precision of recovery" << endl;

		cout << endl;

 		cout << "*************	Setting Parameters	*********" << endl;
		cout<<"X. Reset the parameters of the spelling model" << endl;
		cout<<"Y. Reset the parameters of the keyboard model" << endl;
		cout<<"Z. Display the current parameter values" << endl;
		cout << "****************************************************" <<endl<< endl;


		cout<<"Your choice is: ";
		keyPressed = _getch();
		cout<<keyPressed << endl;

		switch(keyPressed) {
		case 'Q': case 'q'://Quit
			cout << "[QUIT]:" << endl;
			inMenu = false;
			break;

		case 'K': case 'k':// 
			displayParametersKbModel();
 			cout << "[Keyboard model demo]:" << endl;
			demoOfKeyboardModel();
			endOfService("[Keyboard model demo]");
			break;

		case 'S': case 's':// 
			displayParametersSpellingModel();
			cout << "[Spelling model demo, PART I]:" << endl;
			demoTransitionProbability();
			endOfService("[Spelling model demo: state-transition probabilities]");
			cout << "[Spelling model demo, PART II]:" << endl;
			demoInitialProbaility();
			endOfService("[Spelling model demo: initial-state probabilities]");
			break;

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
			break;

		//case 'H': case 'h'://
		//	displayParametersSpellingModel();
		//	displayParametersKbModel();

		//	cout << "[Keyboard+Spellin-> combined HMM model]:" << endl;
		//	demoOfKeyboardPlusSpellingModels();
		//	endOfService("[Keyboard+Spellin-> combined HMM model]");
		//	break;


		case 'B': case 'b'://
			cout << endl << "[Now typing the whole Biola vision article into]:" << endl;
			cout << "[_1_CorruptedBiolaVision1.txt]" << endl;
			typeOneArticle("1_CorruptedBiolaVision.txt", "BiolaVision.txt");

			cout << endl << "[Now typing the whole Biola vision article into]:" << endl;
			cout << "[_2_CorruptedBiolaVision1.txt]" << endl;
			typeOneArticle("2_CorruptedBiolaVision.txt", "BiolaVision.txt", true);

			endOfService("[Test typing the whole Biola vision article twice]");
			break;

		//case 'W': case 'w'://
		//	cout << "[Test automatic recognition of character strings as words]:" << endl;
		//	repetitiveAutomaticCorrectionOfSingleWord();
		//	endOfService("[Test automatic recognition of character strings as words]");
		//	break;

		case 'R': case 'r'://
			cout << "[Recover original message]:" << endl;
			tryToRecoverMessageFromcorruptedMessage();

			cout
				<< "*********************************************************" << endl
				<< "Words in corruptedMessage1.txt examined one by one to" << endl
				<< "  determine the top 4 most likely matches in vocabulary.txt" << endl
				<< "See the recovered message in recoveredTestMessage_V1.txt" << endl
				<< "*********************************************************" << endl;


			endOfService("[Recover original message X from 1 corrupted version]");
			break;

		case 'T': case 't'://
			cout << "[Recover original message 2]:" << endl;
			tryToRecoverMessageFromcorruptedMessage2();

			cout
				<< "*********************************************************" << endl
				<< "Words in corruptedMessage1.txt and corruptedMessage2.txt" << endl
				<< "examined one by one to determine the top 4 most likely matches " << endl
				<< "in vocabulary.txt" << endl
				<< "See the recovered message in recoveredTestMessage.txt" << endl
				<< "*********************************************************" << endl;


			endOfService("[Recover original message 2]");
			break;

		case 'U':case 'u'://
			checkPrecision();
			endOfService("Check Precision");
			break;


		case 'P': case 'p'://
			displayParametersSpellingModel();
			displayParametersKbModel();

			cout << "[Probaility(observedString, actualWord)]:" << endl;
			{	string observedString,  actualWord;


				cout << "What is the observed string you see? (Enter . to quit)";
				cin >> observedString;
				while (observedString != ".")
				{
					cout << "What is the actual string you intent to type?";
					cin >> actualWord;
					cout << "Probaility(" << observedString << " | " <<  actualWord << ")="
						 << prOf1CharSeriesWhenTyping1Word(observedString, actualWord) 
						 << endl << endl;

					cout << "What is the observed string you see? (Enter . to quit)";
					cin >> observedString;
				} 
			}
			endOfService("[Probaility(observedString, actualWord)]");
			break;

		case 'D': case 'd'://
			displayParametersSpellingModel();
			displayParametersKbModel();
			cout << "[log Probability(observedDocument, actualDocument)]:";
			{	string observedDocument, actualDocument;


			cout << "\nWhat is the actual document to type? ";
			cin >> actualDocument;
			while (observedDocument != ".")
			{
				cout << "What is the observed corrupted document? (Enter . to quit) ";
				cin >> observedDocument;
				cout << "\nlog Probability(" << observedDocument << " | " << actualDocument << ") is\n"
					<< logPrOfGettingDocument1WhenTypingDocument2(observedDocument, actualDocument)
					<< " using naturual logarithm base e "
					<< endl << endl;


				//cout << "Name of another observed corrupted document (Enter . to quit)";
				//cin >> observedDocument;
			}
			}
			endOfService("[log Probability(observedDocument, actualDocument)]");
			break;

		case 'L':case 'l'://
			cout << "[bestParameters(observedDocument,actualDocument)]:";
			{
				string observedDocument, actualDocument;
				cout << "\nWhat is the actual document to type? ";
				cin >> actualDocument;
				while (observedDocument != ".")
				{
					cout << "What is the observed corrupted document? (Enter . to quit) ";
					cin >> observedDocument;
					if (observedDocument == ".")
						break;
					bestParametersOfGettingDocument1WhenTypingDocument2(observedDocument, actualDocument);
				}
			}
			endOfService("[bestParameters(observedDocument,actualDocument)]");
			break;


		case 'X': case 'x'://
			setParametersSpellingModel();
			endOfService("Reset parameters of the spelling model");
			break;

		case 'Y': case 'y'://
			setParametersKbModel();
			endOfService("Reset parameters of the keyboard model");
			break;

		case 'Z': case 'z'://
			displayParametersKbModel();
			displayParametersSpellingModel();
			endOfService("Display of parameter values");
			break;

		default:
			cout << "[?? UNKNOWN COMMAND]:" << endl;
			// Unrecognized key, do nothing
			break;
		}

	} // End of while loop.	typeOneArticle("corruptedBiolaVision1.txt", "BiolaVision.txt");



}

