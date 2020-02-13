# Artificial-Intelligence-Project
Build a program that tracks different author's writing habit and converts 
corrupted messages back to their original as close as possible



***************** Parameter settings (in global variables) for the Spelling model ***************
    
    The probability of repeating the current cognitive state again as the next state
    is initially set at 0.2
  
    The probability of moving from the current cognitive state to other states
    as the next state is set to 0.8
    
    The likelihood of moving from the cognitive state of typing some character in a word 
    to the next cognitive state is proportion to the inverse of 
    (spDegenerateTransitionDistancePower) to the 
    (the distance between the current state to the next state)th power.
    In the setting of the original spelling model in the project, it is set to 2
    
    The likelihood of some character in a word as the initial cognitive state
    is proportion to the inverse of (spDegenerateInitialStateDistancePower) to the 
    (the position of the character in the word)th power.
    In the setting of the original spelling model in the project,
    spDegenerateInitialStateDistancePower and spDegenerateTransitionDistancePower
    have the same value
    
**************************************************************************************************


*****************  Parameter settings (in global variables) for the keyboard model ***************

    The probability that you want to type a character and you do successfully make it
    In the setting of the original keyboard model in the project, the probability is set to 0.9
    
    The sum of probabilities that you want to type a character but end in touching a different 
    character is set to 0.1
    
    The likelihood you want to type a character but end in touching a different character
    is proportion to the inverse of (kbDegenerateDistancePower) raised to the (distance between 
    them) th power, and is set to 2
    
**************************************************************************************************


The final product can
1) demonstrate keyboard model,
2) demonstrate spelling model,
3) simulate typing a word,
4) calculate the probability of one character series observed when 
   typing one word,
5) calculate log probability of receiving observed document
   when typing the original document,
6) simulate typing an article twice,
7) recover a corrupted document,
and 8) calculate the precision of the recovery
