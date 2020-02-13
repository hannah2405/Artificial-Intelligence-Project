#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
//#include <conio.h>
#include <fstream>
#include "models.h"
#include <cmath>

using namespace std;





//******************************************************************************
//******************************************************************************
//
//  Parameter setting for the storage capacity
//
//******************************************************************************
//******************************************************************************

//The maximum capacity (maximum number of characters allowed) 
//		for the storing the vocabulary set
#define VocabularyStorageLimit 20000

//The maximum capacity (maximum number of characters allowed) 
//		for storing the corrupted words during spelling recognition
#define NoisyWordsStorageLimit 15000





//******************************************************************************
//******************************************************************************
//
//  Parameter settings (in global variables) for the Spelling model
//
//******************************************************************************
//******************************************************************************
double prSpRepeat = 0.2;
//The probability of repeating the current cognitive state again as the next state
//we make it 0.2 initially, but you can try different values to see the effects.

double prSpMoveOn = 0.8;
//The probability of moving from the current cognitive state to other states
//	as the next state
//we make it 0.8 initially, but you can try different values to see the effects.

//********************************************************
//Note that prSpRepeat + prSpMoveon should always equal 1
//********************************************************

double spDegenerateTransitionDistancePower = 2;
//The likelihood of moving from the cognitive state of typing some character in a word 
//to the next cognitive state is proportion to the inverse of 
//(spDegenerateTransitionDistancePower) to the 
//(the distance between the current state to the next state)th power.
//In the setting of the original spelling model in the project,
//we make it 2, but you can try different values to see the effects.

double spDegenerateInitialStateDistancePower = 2;
//The likelihood of some character in a word as the initial cognitive state
//is proportion to the inverse of 
//(spDegenerateInitialStateDistancePower) to the 
//(the position of the character in the word)th power.
//In the setting of the original spelling model in the project,
// spDegenerateInitialStateDistancePower and spDegenerateTransitionDistancePower
//have the same value, but you can make them different to see the effects
//By default, we make it 2, but you can try different values to see the effects.


void setParametersSpellingModel()
{
	cout << endl
		<< "Reset the parameters of the spelling model:" << endl << endl;

	cout << "Reset P_moveOn, the probability of moving on" << endl
		<< "   from the current cognitive state to other states:" << endl
		<< "P_moveOn = ";
	cin >> prSpMoveOn;

	prSpRepeat = 1 - prSpMoveOn;
	cout << endl
		<< "Automatically reset P_repeat to 1-P_moveOn" << endl
		<< "P_repeat = " << prSpRepeat << endl;

	cout << endl
		<< "Do you want to change the deg_sp? (y or n)" << endl;

	if (cin.get() == 'y')
	{
		cout << "Reset deg_sp, the power coefficient governing the probability of " << endl
			<< "   skipping from the current cognitive state to other states:" << endl
			<< "deg_sp = ";

		cin >> spDegenerateTransitionDistancePower;

		spDegenerateInitialStateDistancePower = spDegenerateTransitionDistancePower;
	}
}

void displayParametersSpellingModel()
{
	cout << endl
		<< "Parameter values of the spelling model:" << endl
		<< "   P_repeat  = " << prSpRepeat << endl
		<< "   P_moveOn  = " << prSpMoveOn << endl
		<< "   deg_sp = " << spDegenerateTransitionDistancePower << endl << endl;
}

//******************************************************************************
//******************************************************************************
//
//  Parameter settings (in global variables) for the keyboard model
//
//******************************************************************************
//******************************************************************************

double prKbHit = 0.6;
//The probability that you want to type a character and you do successfully make it
//In the setting of the original keyboard model in the project,
//we make it 0.9, but you can try different values to see the effects.

double prKbMiss = 0.4;
//The sum of probabilities that you want to type a character but end in touching 
//a different character.
//we make it 0.1, but you can try different values to see the effects.

//*******************************************************
//Note that prKbHit + prKbMiss should always equal 1
//*******************************************************

//In the setting of the original keyboard model in the project,
//we make it 0.2, but you can try different values to see the effects.


double kbDegenerateDistancePower = 2;
//The likelihood you want to type a character but end in touching a different character
//is proportion to the inverse of 
//(kbDegenerateDistancePower) raised to the (distance between them) th power
//In the setting of the original keyboard model in the project,
//we make it 2, but you can try different constants to see the effects.


void setParametersKbModel()
{
	cout << endl
		<< "Reset the parameters of the keyboard model:" << endl << endl;

	cout << "Reset P_hit, the probability of hitting" << endl
		<< "   the right character wanted on the keyboard:" << endl
		<< "P_hit = ";
	cin >> prKbHit;

	prKbMiss = 1 - prKbHit;
	cout << endl
		<< "Automatically reset P_miss to 1-P_hit" << endl
		<< "P_miss = " << prKbMiss << endl;

	cout << endl
		<< "Do you want to change the deg_kb? (y or n)" << endl;

	if (cin.get() == 'y')
	{
		cout << "Reset deg_kb, the power coefficient governing the probability of " << endl
			<< "   skipping from the current cognitive state to other states:" << endl
			<< "deg_kb = ";

		cin >> kbDegenerateDistancePower;
	}
}

void displayParametersKbModel()
{
	cout << endl
		<< "Parameter values of the keyboard model:" << endl
		<< "   P_hit  = " << prKbHit << endl
		<< "   P_miss = " << prKbMiss << endl
		<< "   deg_kb = " << kbDegenerateDistancePower << endl << endl;
}


//******************************************************************************
//******************************************************************************
//
//  Trace flag and the alphabet table
//
//******************************************************************************
//******************************************************************************
bool traceON = true;   // output tracing messages



					   /************************************************************************/
					   //Calculate and return the probability of charGenerated actually typed
					   //given charOfTheState as the underlying cognitive state.
					   /************************************************************************/

double prCharGivenCharOfState(char charGenerated, char charOfTheState)
{   // KEYBOARD STATE
	// CharGenerated = What we actually touched (typed)
	// CharOfTheState = What we want to type in our mind (our cognitive state)

	int distance = 0;
	double x = 0;
	double probability = 0;
	double sum = 0;


	for (int i = 0; i<12; i++)
		sum += 2 * (1 / pow(2, i));

	sum += 1 / pow(2, 12);
	x = prKbMiss / sum;

	distance = abs(charGenerated - charOfTheState);

	if (distance == 0)
		probability = prKbHit;

	else if (distance<13)
		probability = x / (pow(2, distance - 1));

	else
	{
		distance = abs(distance - 26);
		probability = x / (pow(2, distance - 1));

	}



	return probability;


}



/************************************************************************/
//The following function implement part of the spelling model for
// spelling a word with sizeOfTable characters,
//This function
//(i) calculates for each cognitive state excluding the special states I and F,
//     the probability of that cognitive state being the first cognitive state
//     after the transition out of the special state I, and
//(ii) stores these probabilities in a prTable array of sizeOfTable elements.
//Note that the value of the parameter sizeOfTable should be
//     exactly the number characters in the word.
/************************************************************************/

void getPrTableForPossibleInitialStates(double prTable[], int sizeOfTable)
{
	//It is a word of sizeOfTable characters:
	//     i.e. the number of character states is sizeOfTable.
	//     let's index these characters from 0 to sizeOfTable-1.
	//

	//First calculate the sum of ratios of probabilities of
	//     going from the special I state into these character states.
	//This allows you to calculate the scaling factor to determine the probabilities.


	//Second, for each character state calculate the probability
	//     transitioning from the special I state into the character state.

	//**************************************************
	//Replace the following with your own implementation
	//**************************************************
	double sum = 0;
	double x;
	for (int i = 0; i<sizeOfTable; i++)
		sum += 1 / (pow(2, i));

	x = 1 / sum;


	//Second, for each character state calculate the probability
	//     transitioning from the special I state into the character state.

	for (int i = 0; i<sizeOfTable; i++)
		prTable[i] = x*(1 / pow(2, i));

}




/************************************************************************/
//The following function implement part of the spelling model for
// spelling a word with sizeOfTable-1 characters,
// i.e. sizeOfTable should be 1 + the number characters in the word.
//This function
//(i) calculates for each actual cognitive state for a word
//     (excluding the special I state but including the special F state),
//     the probability of that cognitive state being the next cognitive state
//     given currentState as the index of the current state in the word, and
//(ii) stores these probabilities in a transitionPrTable array of sizeOfTable elements.
//Note that the value of the parameter sizeOfTable should be
//     1 + the number characters in the word.
/************************************************************************/

void getPrTableForPossibleNextStates
(double transitionPrTable[], int sizeOfTable, int currentState)
{
	//We are working on a word of sizeOfTable-1 characters:
	//     i.e. the number of character states is sizeOfTable-1.
	//Index these character states from 0 to sizeOfTable-2 respectively, while
	//     the index of the special final state F is sizeOfTable-1.
	//currentState is the index of the current state in the word

	//First calculate the sum of ratios of probabilities of
	//     going from the current state into the other down-stream states down in word
	//     including all the down-stream character states and the
	//     special F final state.
	//This allows you to calculate the scaling factor to determine the probabilities.

	//Second, for each state (excluding the special I state)
	//     calculate the probability of
	//     transitioning from the current state into the character state
	//     and store the probability into the table.


	//**************************************************
	//Replace the following with your own implementation
	//**************************************************
	double sum = 0;
	double x;
	int power = 0;
	for (int i = currentState; i<sizeOfTable - 1; i++)
	{

		sum += 1 / (pow(2, power));
		power++;
		//cout<<"\nsum:"<<sum;

	}

	x = prSpMoveOn / sum;
	//cout<<"\nx"<<x;



	//Second, for each state (excluding the special I state)
	//     calculate the probability of
	//     transitioning from the current state into the character state
	//     and store the probability into the table.
	power = 0;
	for (int i = 0; i<sizeOfTable; i++)
	{
		if (currentState>i) {
			transitionPrTable[i] = 0;
		}
		else if (currentState == i) {
			transitionPrTable[i] = prSpRepeat;
		}
		else {
			transitionPrTable[i] = x / pow(2, power);
			power++;
		}
	}

}



/************************************************************************/
//  Programming #2 
//
//  List below are function prototypes of functions given  or required 
//		to be implemented in Programming #2 
//
/************************************************************************/

/************************************************************************/
//Given the probabilities (of sizeOfTable elements) stored in prTable,
//	try to randomly take a sample out of sizeOfTable elements
//	according to the probabilities of sizeOfTable elements;
//Return the index of the non-deterministically sampled element.
/************************************************************************/
int take1SampleFrom1PrSpace(double prTable[], int sizeOfTable)
{

	int i;
	double prSum = 0;
	for (i = 0; i<sizeOfTable; i++)
		prSum += prTable[i];
	if (prSum < 0.999 || prSum > 1.001)
		cout << "Something is wrong with a random sampleing" << endl
		<< "The sum of all probabilities in the table is " << prSum << endl;

	//Calculate the probability intervals of the characters
	double prAccumulated = 0;
	double * prIntervals;
	prIntervals = new double[sizeOfTable];
	for (i = 0; i<sizeOfTable; i++)
	{
		prAccumulated = prAccumulated + prTable[i];
		prIntervals[i] = prAccumulated;
	}

	/*
	cout << endl;
	for (i=0; i<sizeOfTable; i++)
	{ if (i>0)
	cout << "state " << i << " lower bounded by " << prIntervals[i-1]<< endl;
	cout << "state " << i << " upper bounded by " << prIntervals[i]<< endl;
	}
	*/

	// Generate a random number in [0,1]
	i = rand() % 1001;
	double temp = i / 1000.0;
	//cout << "The random number pair generated is " << i << endl << temp << endl;

	bool sampleTaken = false;

	for (i = 0; i<sizeOfTable && !sampleTaken; i++)
		if (temp <= prIntervals[i])
		{
			delete[] prIntervals;
			//cout << "The random interval id is " << i << endl;
			return i;
		}
	return sizeOfTable - 1;
}





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
void getKeyboardProbabilityTable(char charToType, double prTable[])
{

	int distance = 0;
	double x = 0;
	double sum = 0;


	for (int i = 0; i<12; i++)
		sum += 2 * (1 / pow(2, i));

	sum += 1 / pow(2, 12);
	x = prKbMiss / sum;
	char alphabet = 97;


	for (int i = 0; i<26; i++) {

		distance = abs(alphabet - charToType);

		if (distance == 0)
			prTable[i] = prKbHit;

		else if (distance<13)
			prTable[i] = x / (pow(2, distance - 1));

		else
		{
			distance = abs(distance - 26);
			prTable[i] = x / (pow(2, distance - 1));

		}

		//cout<<endl<<prTable[i];
		alphabet++;
	}



	return;

}


/************************************************************************/
//Simulate the keyboard model:
//Given the charToTye, simulate what character may actually
//	be typed and return it as the result.
/************************************************************************/
char typeOneChar(char charToType)
{
	double prTable[26];
	double charTable[26]; char x = 'a';
	for (int i = 0; i<26; i++)
	{
		charTable[i] = x;
		x++;
	}
	getKeyboardProbabilityTable(charToType, prTable);
	//cout<<endl<<take1SampleFrom1PrSpace(prTable, 26);
	int ans = take1SampleFrom1PrSpace(prTable, 26);
	//cout<<endl<<output;
	//Remove and write your own code
	return charTable[ans];
}



/************************************************************************/
//Simulate the combination of the spelling model and the keyboard model:
//Given a word stored in the word array, simulate what may actually
//	be typed and store the result in the output array.
//maxOutput specifies the capacity limit of the output array, by default it is 100.
//When traceON is true (by default it is false), extra outputs are provided as traces.
/************************************************************************/
void typeOneWord(char word[], char output[], bool traceON, int maxOutput)
{
	//Write your own code


	int charCount = strlen(word) + 1;
	double *prTable = new double[charCount];


	int i = 0, n = 0;
	while (i<charCount - 1)
	{
		if (i == 0)
		{
			getPrTableForPossibleInitialStates(prTable, charCount);
			i = take1SampleFrom1PrSpace(prTable, charCount);
			//cout<<"\nnow at state: "<<i;

			if (i == charCount - 1)
				output[n] = '\0';

			else
			{
				output[n] = typeOneChar(word[i]);
				//cout<<"\ninserted: "<<output[n];
				n++;
			}
		}


		else if (i<charCount - 1)
		{
			getPrTableForPossibleNextStates(prTable, charCount, i);
			i = take1SampleFrom1PrSpace(prTable, charCount);
			//cout<<"\nnow at state: "<<i;

			if (i == charCount - 1)
				output[n] = '\0';

			else
			{
				output[n] = typeOneChar(word[i]);
				//cout<<"\ninserted: "<<output[n];
				n++;
			}
		}


	}


}// end of the function
 /*******************************************************************/

void typeOneArticle(char* corruptedMessageFile, char* sourceArticle, bool trace)
{

	/************************************************************************/

	// corruptedMessageFile and sourceArticle are
	//two character arrays to store the names of two files

	// trace is a boolean flag:

	//  output traces to the screen for your debugging purpose
	// when it is true ; otherwise keep silent.

	//Open the file whose name is stored
	//in sourceArticle as a file for input.

	//Open the file whose name is stored
	//in corruptedMessageFile as a file for output.


	//Read the words one by one from the file sourceArticle:

	//For each word,

	// (i) simulate how the word may be typed according
	// to the spelling model and the keyboard model, and then

	// (ii) store the resulting word into the file corruptedMessageFile.

	/************************************************************************/

	ofstream myFile;
	ifstream statement;

	statement.open(sourceArticle);
	string str;
	string contents[72];
	int count = 0;

	//open file to copy statement into contents
	while (getline(statement, str))
	{

		contents[count] = str;
		count++;

	}
	statement.close();

	//copy each line into word and run getOneWord function
	char output[101];
	string newContents[72];
	for (int i = 0; i<72; i++)
	{
		//(word, output, true);
		char word[21] = "";
		for (int j = 0; j<contents[i].length(); j++)
		{

			word[j] = contents[i][j];
		}
		//cout<<"\nword: "<<word;
		typeOneWord(word, output, false);

		/*cout << endl << "[" << i+1 << "]: get \"" << output
		<< "\" while trying to type \"" << word << "\"" << endl << endl << endl;*/

		newContents[i] = output;
	}

	myFile.open(corruptedMessageFile);
	for (int i = 0; i<72; i++)
	{
		myFile << newContents[i] << "\n";
	}
	myFile.close();

}

//-----------------------------------------------------------------
//program 3


double prOf1CharSeriesWhenTyping1Word(string observedString, string wordString)
{
	//The function should calculate and return
	//    the probability of getting a series of characters stored in
	//    the character array observedString
	//    when you want to type the word stored in
	//    the character array wordString

	int hiddenSize = wordString.size();

	int observedSize = observedString.length();

	//cout<<endl<<"observed: "<<observedString;
	//cout <<endl<<"word: "<<wordString;

	int countHidden = 0;
	//cout<<endl<<"hidden size: "<<hiddenSize;
	for (int i = 0; i<wordString.length(); i++)
	{
		if (wordString[i]>96 &&
			wordString[i]<123)
			countHidden++;
		else
			break;
	}
	hiddenSize = countHidden;
	//cout<<"\nnew size: "<<hiddenSize;

	int countObserved = 0;
	//cout<<endl<<"observed size: "<<observedSize;
	for (int i = 0; i<observedString.length(); i++)
	{
		if (observedString[i]>96 &&
			observedString[i]<123)
			countObserved++;
		else
			break;
	}
	observedSize = countObserved;
	//cout<<"\nnew size: "<<observedSize;

	double* finalState;
	finalState=new double[observedSize + 2];
	for (int i = 0; i<observedSize + 2; i++)
	{
		if (i<observedSize + 1)
			finalState[i] = 0;
		else
			finalState[i] = 1;
	}

	double* initialprTable;
	initialprTable=new double[hiddenSize];

	getPrTableForPossibleInitialStates(initialprTable, hiddenSize);
	double *realInitialPrTable;
	realInitialPrTable=new double [hiddenSize + 2];
	for (int i = 0; i<hiddenSize + 2; i++)
	{
		if (i == 0 || i == hiddenSize + 1)
			realInitialPrTable[i] = 0;
		else
			realInitialPrTable[i] = initialprTable[i - 1];
		//cout<<"\ninitial a: "<<realInitialPrTable[i];
	}

	double* calculatedA;
	calculatedA=new double[hiddenSize + 2];
	for (int i = 0; i<hiddenSize + 2; i++)
	{
		calculatedA[i] = 0;
	}

	vector<double> tempResult;

	for (int i = 0; i<hiddenSize + 2; i++)
		tempResult.push_back(0);

	//initialization for the first column (initial state pi's)
	for (int i = 0; i<observedSize + 2; i++) //going through hidden state including I and F by columns
	{
		if (i == 0) //first column
		{
			for (int j = 0; j<hiddenSize + 2; j++) //going thru rows
			{
				if (j == 0)
					calculatedA[j] = 1;
				else
					calculatedA[j] = 0;
			}
		}

		else //other columns than the initial state
		{
			for (int j = 0; j<hiddenSize + 2; j++) //row by row
			{

				if (j == 0) //first row is always 0
					calculatedA[j] = 0;

				else //starting with 2nd row
				{
					//a for first sum
					calculatedA[j] = tempResult[0];

					calculatedA[j] = calculatedA[j] * realInitialPrTable[j];
					//cout<<endl<<"first a: "<<calculatedA[j];
					//a's for character states
					for (int k = 0; k<hiddenSize; k++)//summation
					{

						int num = j - 1;
						double *transitionPrTable;
						transitionPrTable=new double[hiddenSize + 1];
						getPrTableForPossibleNextStates(transitionPrTable, hiddenSize + 1, k);

						calculatedA[j] += (tempResult[k + 1] * transitionPrTable[num]);
						//cout<<endl<<"next a: "<<tempResult[k+1]*transitionPrTable[num];
						num++;

					}
					//add last a
					if (j < hiddenSize + 1)
						calculatedA[j] += 0 * tempResult[hiddenSize + 1];
					else
						calculatedA[j] += tempResult[hiddenSize + 1];
		
					//cout<<endl<<"last a: "<<tempResult[hiddenSize+1]*finalState[j];


					//multiply b's to the result column
					if (j<hiddenSize + 1)
					{
						double keyboardPrTable[26];
						//getting probability table for each state of mind

						int observed = observedString[i - 1] - 97;
						getKeyboardProbabilityTable(wordString[j - 1], keyboardPrTable);
						calculatedA[j] = calculatedA[j] * keyboardPrTable[observed];

					}
					else //state F, which does not have a chart
					{

						if (i != observedSize + 1)
							calculatedA[j] *= 0;
						else //last observed state F
							calculatedA[j] *= 1;
					}
				}//each row except for first

			}//end of j loop


		}//end of else (besides first column)
		for (int n = 0; n<hiddenSize + 2; n++)
		{
			if (n == 0)
				tempResult[n] = calculatedA[n];
			else
				tempResult[n] = calculatedA[n];
		}
		for (int i = 0; i<hiddenSize + 2; i++)
			tempResult[i];
		//cout<<endl;
	}//end of i for loop


	return tempResult[hiddenSize + 1];
}



double logPrOfGettingDocument1WhenTypingDocument2(string document1, string document2)
{
	/************************************************************************/

	//Return the logarithm of the probability of getting words stored in a file
	//   whose name is in the string document1

	//when you want to type the words stored in a file
	//   whose name is in the string document2 (biola statement).

	/************************************************************************/
	ifstream statementDoc, corruptedDoc;

	//doc2=actual doc
	statementDoc.open(document2);
	string str;
	string contents[72];
	int count = 0;

	//open file to copy statement into contents
	while (getline(statementDoc, str))
	{

		contents[count] = str;
		count++;

	}
	statementDoc.close();

	corruptedDoc.open(document1);
	string corruptedStr;
	string corruptedContents[72];
	count = 0;

	//open file to copy statement into contents
	while (getline(corruptedDoc, corruptedStr))
	{

		corruptedContents[count] = corruptedStr;
		count++;

	}
	corruptedDoc.close();

	//copy each line into word and run getpr function
	double sum = 0;
	double prResult[72];

	//cout<<prOf1CharSeriesWhenTyping1Word(corruptedContents[0],contents[0]);
	//cout<<endl<<prResult[0];


	for (int i = 0; i<72; i++)
	{
		//cout<<endl<<corruptedContents[i]<<"|"<<contents[i];
		prResult[i] = prOf1CharSeriesWhenTyping1Word(corruptedContents[i], contents[i]);
		//cout<<endl<<"pr: "<<prResult[i];
		sum += log(prResult[i]);
	}

	return sum;
}

void bestParametersOfGettingDocument1WhenTypingDocument2(string document1, string document2)
{
	/*
	(ii) search to find the best parameter values for Prhit and Prrepeat
	(iii) set the vlaues of Prhit and Prrepeat (and also Prmiss and PrmoveOn) accordingly
	*/
	double prHitIncrement = 0.1;
	double prRepeatIncrement = 0.1;
	double bestPrHit = 0;
	double bestPrRepeat = 0;
	double prCompare = 0;
	for (int i = 0; i<9; i++)
	{
		//cout<<endl<<i;
		for (int j = 0; j<9; j++)
		{
			if (i == 0 && j == 0)
			{

				//set the parameter by 0.1
				prSpRepeat = prRepeatIncrement;
				prSpMoveOn = 1 - prRepeatIncrement;
				prKbHit = prHitIncrement;
				prKbMiss = 1 - prHitIncrement;
				//cout<<endl<<"rep: "<<prSpRepeat;
				//cout<<endl<<"hit: "<<prKbHit;
				prCompare = logPrOfGettingDocument1WhenTypingDocument2(document1, document2);
				//cout<<endl<<logPrOfGettingDocument1WhenTypingDocument2(document1, document2);
				bestPrHit = prHitIncrement;
				bestPrRepeat = prRepeatIncrement;
			}
			else
			{
				//reset the parameter by incremented values
				prSpRepeat = prRepeatIncrement;
				prSpMoveOn = 1 - prRepeatIncrement;
				prKbHit = prHitIncrement;
				prKbMiss = 1 - prHitIncrement;
				//cout<<endl<<"rep: "<<prSpRepeat;
				//cout<<endl<<"hit: "<<prKbHit;
				//cout<<endl<<logPrOfGettingDocument1WhenTypingDocument2(document1, document2);
				if (prCompare<logPrOfGettingDocument1WhenTypingDocument2(document1, document2))
				{
					bestPrHit = prHitIncrement;
					bestPrRepeat = prRepeatIncrement;
					prCompare = logPrOfGettingDocument1WhenTypingDocument2(document1, document2);
				}

			}
			//cout<<endl<<"bestHit:"<<bestPrHit;
			//cout<<endl<<"bestRep:"<<bestPrRepeat;
			//cout<<endl;
			if (j == 8)
				prRepeatIncrement = 0.1;
			else
				prRepeatIncrement += 0.1;
		}
		prHitIncrement += 0.1;
	}
	prKbHit = bestPrHit;
	prKbMiss = 1 - bestPrHit;
	prSpRepeat = bestPrRepeat;
	prSpMoveOn = 1 - bestPrRepeat;
	cout << "\nNew parameter values:" << endl
		<< "   P_hit  = " << prKbHit << endl
		<< "   P_miss = " << prKbMiss << endl
		<< "   P_Repeat  = " << prSpRepeat << endl
		<< "   P_MoveOn = " << prSpMoveOn << endl
		<< "   deg_kb = " << kbDegenerateDistancePower << endl << endl;

}

void tryToRecoverMessageFromcorruptedMessage()
{
	ifstream vocabDoc, corruptedDoc;
	ofstream myFile;
	//doc2=actual doc
	vocabDoc.open("vocabulary.txt");
	string str;
	string vocabs[120];
	int count = 0;

	//open file to copy statement into contents
	while (getline(vocabDoc, str))
	{

		vocabs[count] = str;
		count++;
		//cout << endl<<str;
	}
	vocabDoc.close();

	corruptedDoc.open("corruptedMessage1.txt");
	string corruptedStr;
	string corruptedContents[195];
	count = 0;

	//open file to copy statement into contents
	while (getline(corruptedDoc, corruptedStr))
	{

		corruptedContents[count] = corruptedStr;
		count++;
		//cout << endl << corruptedStr;

	}



	corruptedDoc.close();
	myFile.open("recoveredMessage_V1.txt");
	for (int i = 0; i<195; i++)
	{
		int noteVocab[4];
		double probabilities[4];
		//cout << endl << "i=" << corruptedContents[i];

		for (int j = 0; j<120; j++)
		{
			//cout << endl << "j=" << vocabs[j];
			double newProb = prOf1CharSeriesWhenTyping1Word(corruptedContents[i], vocabs[j]);
			//cout << endl << "new probability: " << newProb;
			if (j == 0)
			{
				//cout << endl << "1";
				probabilities[j] = newProb;
				noteVocab[j] = j;
				//cout << endl << j + 1 << ")" << vocabs[noteVocab[j]];
			}
			else if (j == 1)
			{
				if (newProb>probabilities[0])
				{
					//cout << endl << "2";
					probabilities[1] = probabilities[0];
					probabilities[0] = newProb;
					noteVocab[1] = noteVocab[0];
					noteVocab[0] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
				}
				else
				{
					//cout << endl << "3";
					probabilities[1] = newProb;
					noteVocab[j] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
				}

			}
			else if (j == 2)
			{
				if (newProb>probabilities[0])
				{
					//cout << endl << "4";
					probabilities[2] = probabilities[1];
					probabilities[1] = probabilities[0];
					probabilities[0] = newProb;
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = noteVocab[0];
					noteVocab[0] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
				}
				else if (newProb>probabilities[1])
				{
					//cout << endl << "5";
					probabilities[2] = probabilities[1];
					probabilities[1] = newProb;
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
				}
				else
				{
					//cout << endl << "6";
					probabilities[2] = newProb;
					noteVocab[j] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
				}

			}

			else if (j == 3)
			{
				if (newProb>probabilities[0])
				{
					//cout << endl << "7";
					probabilities[3] = probabilities[2];
					probabilities[2] = probabilities[1];
					probabilities[1] = probabilities[0];
					probabilities[0] = newProb;

					noteVocab[3] = noteVocab[2];
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = noteVocab[0];
					noteVocab[0] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[1])
				{
					//cout << endl << "8";
					probabilities[3] = probabilities[2];
					probabilities[2] = probabilities[1];
					probabilities[1] = newProb;
					noteVocab[3] = noteVocab[2];
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[2])
				{
					//cout << endl << "9";
					probabilities[3] = probabilities[2];
					probabilities[2] = newProb;
					noteVocab[3] = noteVocab[2];
					noteVocab[2] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else
				{
					//cout << endl << "10";
					probabilities[3] = newProb;
					noteVocab[j] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}

			}

			else
			{
				if (newProb>probabilities[0])
				{
					//cout << endl << "11";
					probabilities[3] = probabilities[2];
					probabilities[2] = probabilities[1];
					probabilities[1] = probabilities[0];
					probabilities[0] = newProb;

					noteVocab[3] = noteVocab[2];
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = noteVocab[0];
					noteVocab[0] = j;

					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[1])
				{
					//cout << endl << "12";
					probabilities[3] = probabilities[2];
					probabilities[2] = probabilities[1];
					probabilities[1] = newProb;

					noteVocab[3] = noteVocab[2];
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = j;

					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[2])
				{
					//cout << endl << "13";
					probabilities[3] = probabilities[2];
					probabilities[2] = newProb;

					noteVocab[3] = noteVocab[2];
					noteVocab[2] = j;

					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[3])
				{
					//cout << endl << "14";
					probabilities[3] = newProb;
					noteVocab[3] = j;

					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
			}
			//cout << endl;
		}//end of for loop j
		myFile << vocabs[noteVocab[0]] << ", " << vocabs[noteVocab[1]] << ", " << vocabs[noteVocab[2]] << ", " << vocabs[noteVocab[3]] << endl;

	}//end of for loop i
	myFile.close();

}

void tryToRecoverMessageFromcorruptedMessage2()
{
	ifstream vocabDoc, corruptedDoc;
	ofstream myFile;
	//doc2=actual doc
	vocabDoc.open("vocabulary.txt");
	string str;
	string vocabs[120];
	int count = 0;

	//open file to copy statement into contents
	while (getline(vocabDoc, str))
	{

		vocabs[count] = str;
		count++;

	}
	vocabDoc.close();

	corruptedDoc.open("corruptedMessage1.txt");
	string corruptedStr;
	string corruptedContents[195];
	count = 0;

	//open file to copy statement into contents
	while (getline(corruptedDoc, corruptedStr))
	{

		corruptedContents[count] = corruptedStr;
		count++;

	}
	corruptedDoc.close();

	corruptedDoc.open("corruptedMessage2.txt");
	string corruptedStr2;
	string corruptedContents2[195];
	count = 0;

	//open file to copy statement into contents
	while (getline(corruptedDoc, corruptedStr2))
	{

		corruptedContents2[count] = corruptedStr2;
		count++;

	}

	corruptedDoc.close();
	myFile.open("recoveredMessage_V2.txt");
	for (int i = 0; i<195; i++)
	{
		int noteVocab[4];
		double probabilities[4];
		//cout << endl << "i=" << corruptedContents[i];

		for (int j = 0; j<120; j++)
		{
			//cout << endl << "j=" << vocabs[j];
			double newProb = prOf1CharSeriesWhenTyping1Word(corruptedContents[i], vocabs[j])*prOf1CharSeriesWhenTyping1Word(corruptedContents2[i], vocabs[j]);

			//cout << endl << "new probability: " << newProb;
			if (j == 0)
			{
				probabilities[j] = newProb;
				noteVocab[j] = j;
				//cout << endl << j + 1 << ")" << vocabs[noteVocab[j]];
			}
			else if (j == 1)
			{
				if (newProb>probabilities[0])
				{
					probabilities[1] = probabilities[0];
					probabilities[0] = newProb;
					noteVocab[1] = noteVocab[0];
					noteVocab[0] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
				}
				else
				{
					probabilities[1] = newProb;
					noteVocab[j] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
				}

			}
			else if (j == 2)
			{
				if (newProb>probabilities[0])
				{
					probabilities[2] = probabilities[1];
					probabilities[1] = probabilities[0];
					probabilities[0] = newProb;
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = noteVocab[0];
					noteVocab[0] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
				}
				else if (newProb>probabilities[1])
				{
					probabilities[2] = probabilities[1];
					probabilities[1] = newProb;
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
				}
				else
				{
					probabilities[2] = newProb;
					noteVocab[j] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
				}

			}

			else if (j == 3)
			{
				if (newProb>probabilities[0])
				{
					probabilities[3] = probabilities[2];
					probabilities[2] = probabilities[1];
					probabilities[1] = probabilities[0];
					probabilities[0] = newProb;

					noteVocab[3] = noteVocab[2];
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = noteVocab[0];
					noteVocab[0] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[1])
				{
					probabilities[3] = probabilities[2];
					probabilities[2] = probabilities[1];
					probabilities[1] = newProb;
					noteVocab[3] = noteVocab[2];
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[2])
				{
					probabilities[3] = probabilities[2];
					probabilities[2] = newProb;
					noteVocab[3] = noteVocab[2];
					noteVocab[2] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else
				{
					probabilities[3] = newProb;
					noteVocab[j] = j;
					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}

			}

			else
			{
				if (newProb>probabilities[0])
				{
					probabilities[3] = probabilities[2];
					probabilities[2] = probabilities[1];
					probabilities[1] = probabilities[0];
					probabilities[0] = newProb;

					noteVocab[3] = noteVocab[2];
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = noteVocab[0];
					noteVocab[0] = j;

					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[1])
				{
					probabilities[3] = probabilities[2];
					probabilities[2] = probabilities[1];
					probabilities[1] = newProb;

					noteVocab[3] = noteVocab[2];
					noteVocab[2] = noteVocab[1];
					noteVocab[1] = j;

					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[2])
				{
					probabilities[3] = probabilities[2];
					probabilities[2] = newProb;

					noteVocab[3] = noteVocab[2];
					noteVocab[2] = j;

					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
				else if (newProb>probabilities[3])
				{
					probabilities[3] = newProb;
					noteVocab[3] = j;

					//cout << endl << "1)" << vocabs[noteVocab[0]];
					//cout << endl << "2)" << vocabs[noteVocab[1]];
					//cout << endl << "3)" << vocabs[noteVocab[2]];
					//cout << endl << "4)" << vocabs[noteVocab[3]];
				}
			}
			//cout << endl;
		}//end of for loop j
		myFile << vocabs[noteVocab[0]] << ", " << vocabs[noteVocab[1]] << ", " << vocabs[noteVocab[2]] << ", " << vocabs[noteVocab[3]] << endl;

	}//end of for loop i
	myFile.close();
}

void checkPrecision()
{
	cout << "\nRegarding the quality of recoveredMessage_V1.txt:";
	ifstream documentX, recovered1, recovered2;


	documentX.open("MessageX.txt");
	string str;
	string message[300], recoveredMsg1[300], recoverdMsg2[300];
	int count = 0;

	//open file to copy statement into contents
	while (getline(documentX, str))
	{

		message[count] = str;
		count++;

	}
	documentX.close();

	recovered1.open("recoveredMessage_V1.txt");
	int totalword = 0;
	//open file to get 4 words
	string words[195][4]; //195 lines of 4 words
	while (getline(recovered1, str))
	{
		int comma = 0, wordcount = 0;
		for (int i = 0; i<str.length(); i++)
		{
			if (str[i] != ','&&str[i] != ' ')
			{
				words[totalword][wordcount].push_back(str[i]);
			}
			else if (str[i] == ',');
			else
				wordcount++;
			comma++;

		}
		totalword++;
		//cout<<endl<<words[0]<<" "<<words[1]<<" "<<words[2]<<" "<<words[3];

	}
	recovered1.close();

	//count and calculate percentage
	double wordcount = 1; double totalcount = totalword;
	double precision;
	for (int i = 0; i<totalword; i++)
	{
		if (message[i] == words[i][0])
			wordcount++;
	}
	precision = wordcount / totalcount;
	cout << endl << "Out of the " << totalcount << " words in the original message"
		<< endl << wordcount << " words recognized in the top 1 list, " << precision << "%";

	wordcount = 1;
	for (int i = 0; i<totalword; i++)
	{
		if (message[i] == words[i][0] | message[i] == words[i][1])
			wordcount++;
	}
	precision = wordcount / totalcount;
	cout << endl << wordcount << " words recognized in the top 2 list, " << precision << "%";

	wordcount = 1;
	for (int i = 0; i<totalword; i++)
	{
		if (message[i] == words[i][0] | message[i] == words[i][1] |
			message[i] == words[i][2])
			wordcount++;
	}
	precision = wordcount / totalcount;
	cout << endl << wordcount << " words recognized in the top 3 list, " << precision << "%";

	wordcount = 1;
	for (int i = 0; i<totalword; i++)
	{
		if (message[i] == words[i][0] | message[i] == words[i][1] |
			message[i] == words[i][2] | message[i] == words[i][3])
			wordcount++;
	}
	precision = wordcount / totalcount;
	cout << endl << wordcount << " words recognized in the top 4 list, " << precision << "%";


	//****************************************************************

	cout << "\n\nRegarding the quality of recoveredMessage_V2.txt:";
	recovered2.open("recoveredMessage_V2.txt");
	totalword = 0;
	//open file to get 4 words
	string words2[195][4]; //195 lines of 4 words
	while (getline(recovered2, str))
	{
		int comma = 0, wordcount = 0;
		for (int i = 0; i<str.length(); i++)
		{
			if (str[i] != ','&&str[i] != ' ')
			{
				words2[totalword][wordcount].push_back(str[i]);
			}
			else if (str[i] == ',');
			else
				wordcount++;
			comma++;

		}
		totalword++;
		//cout<<endl<<words[0]<<" "<<words[1]<<" "<<words[2]<<" "<<words[3];

	}
	recovered2.close();

	//count and calculate percentage
	wordcount = 1; totalcount = totalword;
	precision = 0;
	for (int i = 0; i<totalword; i++)
	{
		if (message[i] == words2[i][0])
			wordcount++;
	}
	precision = wordcount / totalcount;
	cout << endl << "Out of the " << totalcount << " words in the original message"
		<< endl << wordcount << " words recognized in the top 1 list, " << precision << "%";

	wordcount = 1;
	for (int i = 0; i<totalword; i++)
	{
		if (message[i] == words2[i][0] | message[i] == words2[i][1])
			wordcount++;
	}
	precision = wordcount / totalcount;
	cout << endl << wordcount << " words recognized in the top 2 list, " << precision << "%";

	wordcount = 1;
	for (int i = 0; i<totalword; i++)
	{
		if (message[i] == words2[i][0] | message[i] == words2[i][1] |
			message[i] == words2[i][2])
			wordcount++;
	}
	precision = wordcount / totalcount;
	cout << endl << wordcount << " words recognized in the top 3 list, " << precision << "%";

	wordcount = 1;
	for (int i = 0; i<totalword; i++)
	{
		if (message[i] == words2[i][0] | message[i] == words2[i][1] |
			message[i] == words2[i][2] | message[i] == words2[i][3])
			wordcount++;
	}
	precision = wordcount / totalcount;
	cout << endl << wordcount << " words recognized in the top 4 list, " << precision << "%";


	cout << endl;
}
