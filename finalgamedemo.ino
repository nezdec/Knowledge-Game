#include <Adafruit_CircuitPlayground.h>

float midi[127];
int A_four = 440; // a is 440 hz...
// Arrays to store Scale MIDI pitch sequence
int c_major[8] = {60, 62, 64, 65, 67, 69, 71, 72}; // c_major scale on C4
int c_minor[8] = {60, 62, 63, 65, 67, 68, 70, 72}; // c_minor scale on C4

// Example of 2D Array
// First number is how many notes your melody will play
// Second number is how many elements of data will be stored for each note, 2 (MIDI pitch, Duration in milliseconds)
int song[7][2] = {
    {67, 200},  // song[0][0]], song[0][1]] G4
    {64, 200},  // song[1][0]], song[1][1]] E4
    {67, 400},  // song[2][0]], song[2][1]]
    {127,100}, //pause
    {67, 200},  // song[3][0]], song[3][1]]
    {64, 200},  // song[4][0]], song[4][1]]
    {67, 400},  // song[5][0]], song[5][1]]
//  {127, 200},  // 127 will result in a frequency too high for the speaker to play, resulting in silence. You can use this as a "rest" or blank space in your melody
  };


void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  generateMIDI();

  // Set up LEDs
  CircuitPlayground.setBrightness(10); // Set LED brightness
  CircuitPlayground.clearPixels(); // Turn off all LEDs

  //display intro when serial monitor open
  while (!Serial) {
    delay(100);
  }
}

void playPredeterminedMelody() {
  for(int i = 0; i < sizeof(song) / sizeof(song[0]); i++) // Calculate how many rows are in the array using: sizeof(song) / sizeof(song[0])
  {
    CircuitPlayground.playTone(midi[song[i][0]], song[i][1]);
    Serial.println(midi[song[i][0]]);
    delay(1);
  }
}
void sweep(bool correct) {
  for (int i = 0; i < 10; i++) {
    if (correct) {
      CircuitPlayground.setPixelColor(i, 0, 255, 0); //green
    } else {
      CircuitPlayground.setPixelColor(i, 255, 0, 0); //red
    }
    CircuitPlayground.clearPixels();
    delay(50);
  }
}

void loop() {
  Serial.println("Welcome to the General Knowledge Quiz!\nYou will progress through five different levels, each level more difficult than the last.");
  delay(1000);
  // Questions
  String questions[] = {
    "What is the capital of France? A: Lyon B: Paris",
    "Who wrote 'Romeo and Juliet'? A: William Shakespeare B: Karl Marx ",
    "What is the chemical symbol for water? A: H2O B: K",
    "Which planet is known as the Red Planet? A: Jupiter B: Mars",
    "Who painted the Mona Lisa? A: Pablo Picasso B: Leonardo da Vinci",
    "In which year did World War I begin? A: 1914 B: 1918",
    "Who discovered penicillin? A: Alexander Fleming B: Albert Einstein",
    "What is the largest mammal in the world? A: Blue whale B: African elephant",
    "Who developed the theory of relativity? A: Isaac Newton B: Albert Einstein",
    "What is the smallest country in the world? A: Monaco B: Vatican City"
  };

  // Level indicators
  String levels[] = {
    "Level 1: Easy",
    "Level 2: Moderate",
    "Level 3: Challenging",
    "Level 4: Difficult",
    "Level 5: Expert"
  };

  // Correct answers (right button for true, left button for false)
  bool correctAnswers[] = {
    true,  // Paris is the capital of France
    false, // William Shakespeare wrote 'Romeo and Juliet'
    false, // Chemical symbol for water is H2O
    true,  // Mars is known as the Red Planet
    true,  // Leonardo da Vinci painted the Mona Lisa
    true,  // World War I began in 1914
    true,  // Alexander Fleming discovered penicillin
    true,  // Blue whale is the largest mammal
    false, // Albert Einstein developed the theory of relativity
    true   // Vatican City is the smallest country
  };

  // Scoring
  int score = 0;

  // Ask questions and get user input
  for (int i = 0; i < 10; i++) {
    // Display level indicator
    if (i == 5) {
      Serial.println("Level 2: Moderate - You're advancing!");
      delay(1000);
    } else if (i == 7) {
      Serial.println("Level 3: Challenging - Keep going!");
      delay(1000);
    }

    // Display question
    Serial.println(questions[i]);

    // Wait for user input
    while (!CircuitPlayground.leftButton() && !CircuitPlayground.rightButton() && Serial.available() == 0) {
      delay(100);
    }

    bool userAnswer;
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
    } else {
      userAnswer = CircuitPlayground.rightButton(); // right button for true
    }

    // Check if the answer is correct
    if (userAnswer == correctAnswers[i]) {
      Serial.println("Correct!");
      sweep(true); // green LEDS light when answer is correct
      score++; // Increase score for each correct answer
    } else {
      Serial.println("Incorrect!");
      sweep(false); // Red LED for incorrect answer
//CircuitPlayground.clearPixels();
    }
    delay(1000);
  }

  // Display final score
  Serial.print("Your final score is: ");
  Serial.print(score);
  Serial.print(" out of ");
  Serial.println(10); // Total number of questions
  playPredeterminedMelody();

  // Wait before restarting the game
  delay(5000);
}
void generateMIDI() {
  for (int x = 0; x < 127; ++x) {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
}
}