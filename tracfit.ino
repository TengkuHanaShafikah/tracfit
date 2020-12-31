////////////////////////////////////////////////////////////////////////////
// 
// TracFit - Fitness Tracker for the Adafruit Circuit Playground
// Author: Tengku Hana Shafikah
// Fall 2020 Embedded Fitness Challenge
// 
// HOW TO USE:
// 1) First, clip the device onto the front of your collar.
// 2) Turn on the device. The main menu will show four distinct pairs of
//    lit-up LEDs. One pair is color-coded to a specific exercise:
//      - Top Left (Green): Jumping Jacks
//      - Bottom Left (Blue): Squats
//      - Bottom Right (Purple): Situps
//      - Top Right (Pink): Pushups
//    Colors that appear on the menu correspond to exercise routines that are
//    not yet complete.
// 3) Get into the initial/starting position of the type of exercise you wish
//    to perform.
// 4) To begin tracking your exercise, press either of the two buttons on
//    your device. The device will beep and the LEDs will turn white, indicating
//    that it is actively tracking your starting position and movement.
// 5) If the device begins to beep and blink in a color correspoding to
//    a specific exercise, then the device has detected the exercise.
//    Otherwise, begin performing your exercise until the device detects it.
// 6) The device will track your progress on your exercise routine.
//    One routine consists of 10 repetitions. For each repetition you perform, 
//    one LED will light up. Continue your exercise routine until all 10 LEDs
//    light up.
// 7) When you complete your routine, the device will play a victory song and
//    return to the main menu. Colors that correspond to the completed exercise
//    routine will no longer appear on the main menu.
// 8) Repeat steps 3 - 7 for the rest of the exercises shown on the main menu.
// 9) To reset your progress, simply turn the device off then on again.
// 
////////////////////////////////////////////////////////////////////////////

#include <Adafruit_CircuitPlayground.h>

////////////////////////////////////////////////////////////////////////////
// CONSTANTS & GLOBAL VARIABLES //
//////////////////////////////////

#define DEBOUNCE        100       // The default debounce time in ms.
#define REPS            10        // The number of repetitions in one routine.
#define GREEN           0xFDFE01  // Hex values for the colors.
#define BLUE            0x00F188 
#define PURPLE          0x0101FF
#define PINK            0xFC0839
#define WHITE           0xFFFFFF
#define HALF            1100      // Durations of a whole note in ms.
#define EIGHTH          275     
#define EIGHTH_TRIPLE   183
#define SIXTEENTH       EIGHTH/2

// The following variables keep track of whether a 10-repetition
// routine for a specific exercise has been completed.
boolean finishedJumpJacks = false;
boolean finishedSquats = false;
boolean finishedSitUps = false;
boolean finishedPushUps = false;

// The following variables keep track of how many repetitions
// are completed for each of the four types of exercises.
int numOfJumpJacks = 0;
int numOfSquats = 0;
int numOfSitUps = 0;
int numOfPushUps = 0;

// The following floats represent the x, y and z values
// of the accelerometer data.
float X, Y, Z;


////////////////////////////////////////////////////////////////////////////
// FUNCTIONS //
///////////////

// Set all NeoPixel LEDs on the board to a certain color.
void setAllLeds(uint32_t color) {
  for (int i = 0; i <= 9; i++) {
    CircuitPlayground.setPixelColor(i, color);
  }
}


// Play a sound while blinking all LEDs twice in a specific
// color. This animation is used to indicate that a specific
// exercise has occured.
void blinkAnimLed(uint32_t color) {
  CircuitPlayground.clearPixels();
  for (int i = 0; i <= 1; i++) {
    setAllLeds(color);
    CircuitPlayground.playTone(440, 100);
    delay(DEBOUNCE);
    CircuitPlayground.clearPixels();
    delay(DEBOUNCE);
  }
}


// Play a victory song when the user completes a routine.
// Melody from: https://en.wikipedia.org/wiki/Charge_(fanfare)
void victorySong() {
  // Notes and length of tone are stored in two seperate arrays.
  int durations[] = {EIGHTH_TRIPLE/2, EIGHTH_TRIPLE/2, EIGHTH_TRIPLE/2,
                    EIGHTH/2, SIXTEENTH/2, HALF/2};
  int notes[] = {392, 523, 659, 784, 659, 784};
  for (int i = 0; i <= 5; i++) {
    CircuitPlayground.playTone(notes[i], durations[i]);
  }
}


// Display LEDs for the main menu. Exercises that have
// routines not yet completed will have their LEDs lit up. 
// Exercise routines that are completed will have their LEDs
// switched off and tracking for those exercises are disabled.
void showMainMenu() {
  // Turn off all LEDs.
  CircuitPlayground.clearPixels();
  // Check status of each exercise and light up LEDs accordingly.
  if (!finishedJumpJacks) {
    // Top Left (Green): Jumping Jacks
    CircuitPlayground.setPixelColor(0, GREEN);
    CircuitPlayground.setPixelColor(1, GREEN);
  }
  if (!finishedSquats) {
    // Bottom Left (Blue): Squats
    CircuitPlayground.setPixelColor(3, BLUE);
    CircuitPlayground.setPixelColor(4, BLUE);
  }
  if (!finishedSitUps) {
    // Bottom Right (Purple): Sit Ups
    CircuitPlayground.setPixelColor(5, PURPLE);
    CircuitPlayground.setPixelColor(6, PURPLE);
  }
  if (!finishedPushUps) {
    // Top Right (Pink): Push-Ups
    CircuitPlayground.setPixelColor(8, PINK);
    CircuitPlayground.setPixelColor(9, PINK);
  }
}


// Track the number of sit ups the user performs.
// Once the user completes 10 situps, mark the situp
// routine as complete and return to the main menu.
void trackSitUps() {
  // Animate the LEDs to indicate start of routine.
  blinkAnimLed(PURPLE);
  CircuitPlayground.clearPixels();

  // Create a variable to track the current state of the situp.
  // (true = up; false = down)
  boolean isUpSitUp = false;
  
  // Each time the user does one pushup, light up one LED.
  while (numOfSitUps < REPS) {
    // Track the X, Y and Z accelerometer values.
    X = CircuitPlayground.motionX();
    Y = CircuitPlayground.motionY();
    Z = CircuitPlayground.motionZ();

    // The upward part of the situp occurs when the X < Y, and the
    // downward part of a situp occurs when X > Y. A situp occurs
    // when the user returns to the downward position of the situp.
    if ((isUpSitUp) && (X > Y)) {
      // If the previous state was up and current state is 
      // now down, change the state of the pushup to down. 
      isUpSitUp = false;
      // Light up corresponding LED and play sound for each valid pushup.
      CircuitPlayground.setPixelColor(numOfSitUps, PURPLE);
      CircuitPlayground.playTone(440, 100);
      // Increment the situp count.
      numOfSitUps++; 
    }
    else if ((!isUpSitUp) && (X < Y)){
      // If the previous state was down and current motion is 
      // now up, change the state of the situp to up. 
      isUpSitUp = true;
    }

    // Set the debounce time to 100 ms.
    delay(DEBOUNCE);
  }
  
  // Play a victory song and animate the LEDs
  // to indicate that the routine has been completed.
  setAllLeds(PURPLE);
  victorySong();
  
  // Mark the situp routine as finished.
  finishedSitUps = true;
}


// Track the number of pushups the user performs.
// Once the user completes 10 pushups, mark the pushup
// routine as complete and return to the main menu.
void trackPushUps() {
  // Animate the LEDs to indicate start of routine.
  blinkAnimLed(PINK);
  CircuitPlayground.clearPixels();

  // Create a variable to track the current state of the pushup.
  // (true = down; false = up)
  boolean isDownPushUp = false;
  
  // Each time the user does one pushup, light up one LED.
  while (numOfPushUps < REPS){
    // Track the X, Y and Z accelerometer values.
    X = CircuitPlayground.motionX();
    Y = CircuitPlayground.motionY();
    Z = CircuitPlayground.motionZ();

    // The downward part of the pushup occurs when the Z < X,
    // and the upward part of a pushup occurs when Z > X. A pushup is
    // counted once the user returns to the upward part of the pushup.
    if ((isDownPushUp) && (Z > X)) {
      // If the previous state was down and current state is 
      // now up, change the state of the pushup to up. 
      isDownPushUp = false;
      // Light up corresponding LED and play sound for each valid pushup.
      CircuitPlayground.setPixelColor(numOfPushUps, PINK);
      CircuitPlayground.playTone(440, 100);
      // Increment the pushup count.
      numOfPushUps++; 
    }
    else if ((!isDownPushUp) && (Z < X)){
      // If the previous state was up and current motion is 
      // now down, change the state of the pushup to down. 
      isDownPushUp = true;
    }

    // Set the debounce time to 100 ms.
    delay(DEBOUNCE);
  }
  
  // Play a victory song and animate the LEDs
  // to indicate that the routine has been completed.
  setAllLeds(PINK);
  victorySong();
  
  // Mark the pushup routine as finished.
  finishedPushUps = true;
}


// Track the number of squats the user performs.
// Once the user completes 10 squats, mark the squat
// routine as complete and return to the main menu.
void trackSquats() {
  // Animate the LEDs to indicate start of routine.
  blinkAnimLed(BLUE);
  CircuitPlayground.clearPixels();

  // Create a variable to track the current state of the squat.
  // (true = downwards motion of squat; false = upwards motion)
  boolean isDownSquat = true;
  
  // Each time the user does one squat, light up one LED.
  while (numOfSquats < REPS){
    // Track the X, Y and Z accelerometer values.
    X = CircuitPlayground.motionX();
    Y = CircuitPlayground.motionY();
    Z = CircuitPlayground.motionZ();

    // The downward motion of a squat occurs when the Z < Y,
    // and the up motion of a squat occurs when Z > Y. A squat is
    // counted once the user returns to the up motion of a squat.
    if ((isDownSquat) && (Z > Y)) {
      // If the previous motion was downwards and current motion is 
      // now upwards, change the state of the squat to upwards. 
      isDownSquat = false;
      // Light up corresponding LED and play sound for each valid squat.
      CircuitPlayground.setPixelColor(numOfSquats, BLUE);
      CircuitPlayground.playTone(440, 100);
      // Increment the squat count.
      numOfSquats++; 
    }
    else if ((!isDownSquat) && (Z < Y)){
      // If the previous motion was upwards and current motion is 
      // now downwards, change the state of the squat to downwards. 
      isDownSquat = true;
    }

    // Set the debounce time to 250 ms.
    delay(DEBOUNCE*2.5);
  }
  
  // Play a victory song and animate the LEDs
  // to indicate that the routine has been completed.
  setAllLeds(BLUE);
  victorySong();
  
  // Mark the squat routine as finished.
  finishedSquats = true;
}


// Track the number of jumping jacks the user performs.
// Once the user completes 10 squats, mark the squat
// routine as complete and return to the main menu.
void trackJumpJacks() {
  // Animate the LEDs to indicate start of routine.
  blinkAnimLed(GREEN);
  CircuitPlayground.clearPixels();
  
  // Create a variable to track the number of jumps.
  // One jumping jack consists of two jumps. The device has
  // already detected the user doing doing one jump.
  int totalJumps = 1;

  // Each time the user does one jumping jack, light up one LED.
  while (numOfJumpJacks < REPS) {
    // Track the X, Y and Z accelerometer values.
    X = CircuitPlayground.motionX();
    Y = CircuitPlayground.motionY();
    Z = CircuitPlayground.motionZ();

    // A jump occurs when the user's body moves downwards with 
    // an X value of -15 or lower. Every two jumps equate to 
    // one jumping jack.
    if (X <= -15) {
      // Increment number of total jumps.
      totalJumps++;
      // If the number of total jumps are even,
      // increment number of jumping jacks.
      if (totalJumps % 2 == 0) {
        // Light up corresponding LED and play sound for each valid squat.
        CircuitPlayground.setPixelColor(numOfJumpJacks, GREEN);
        CircuitPlayground.playTone(440, 100);
        // Increment the squat count.
        numOfJumpJacks++; 
      }
    }

    // Set debounce time to 150 ms.
    delay(DEBOUNCE*1.5);
  }

  // Play a victory song and animate the LEDs
  // to indicate that the routine has been completed.
  setAllLeds(GREEN);
  victorySong();
  
  // Mark the jumping jack routine as finished.
  finishedJumpJacks = true;
}


// Track the user's starting position and movements
// to successfully detect one out of the four exercises.
// An exercise is only tracked if the user has not completed
// a 10-repetition routine for it yet. The conditions rely on
// the user's initial position when they begin tracking their
// device, so it is vital that the user gets into their
// starting positions ready right before enabling tracking.
void beginTracking() {

  // Obtain the starting position of the user.
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

  // If X < Y < Z, then the user is standing. Thus, the exercise
  // must either be a jumping jack or a squat.
  if ((X < Y) && (Y < Z)) {
    while (true) {
      // Track the X, Y and Z accelerometer values.
      X = CircuitPlayground.motionX();
      Y = CircuitPlayground.motionY();
      Z = CircuitPlayground.motionZ();
      delay(DEBOUNCE);
  
      // JUMPING JACKS: Begin tracking the jumping jack exercise routine when
      // the user jumps once. One jump occurs when the X value is -15 or lower.
      if ((!finishedJumpJacks) && (X <= -15)) {
        trackJumpJacks();
        // Once finished, pause for 0.5 sec before reloading
        // the main menu. Then, break out of the loop.
        delay(DEBOUNCE*5);
        showMainMenu();
        break;
      }
  
      // SQUATS: Begin tracking the squat exercise routine when the user 
      // squats downwards. The down motion of a squat begins when the
      // Z value goes beneath the Y value.
      else if ((!finishedSquats) && (Z < Y)) {
        trackSquats();
        // Once finished, pause for 0.5 sec before reloading
        // the main menu. Then, break out of the loop.
        delay(DEBOUNCE*5);
        showMainMenu();
        break;
      }
    }
  }

  // If Y > Z > X, then the user is generally facing downwards.
  // Thus, the exercise must be a pushup.
  else if ((!finishedPushUps) && (Y > Z) && (Z > X)) {
    trackPushUps();
  }

  // If Z > X > Y, then the user is lying down. 
  // Thus, the exercise must be a situp.
  else if ((!finishedSitUps) && (Z > X) && (X > Y)) {
    trackSitUps();
  }  

  // Once finished, pause for 0.5 sec before reloading
  // the main menu. Then, break out of the loop.
  delay(DEBOUNCE*5);
  showMainMenu();
}


////////////////////////////////////////////////////////////////////////////
// SETUP //
///////////

void setup() {  
  // Initialize Circuit Playground library.
  CircuitPlayground.begin();
  
  // Initialize LED brightness.
  CircuitPlayground.setBrightness(4);
  
  // Display the main menu.
  showMainMenu();
}


////////////////////////////////////////////////////////////////////////////
// LOOP //
//////////

void loop() {
  // If the user presses any of the two buttons on the device, it will
  // begin tracking the user's movements. Device will notify the user
  // that tracking as begun by beeping once and showing all white LEDs.
  // Otherwise, the device will continue showing the main menu and
  // staying on standby.
  if (CircuitPlayground.leftButton() || CircuitPlayground.rightButton()) {
    setAllLeds(WHITE);
    CircuitPlayground.playTone(440, 100);
    beginTracking();
  }
  
  // Delay by the default debounce time.
  delay(DEBOUNCE);
}
