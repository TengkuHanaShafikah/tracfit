# TracFit
Author: Tengku Hana Shafikah

## Summary
In the fall of 2020, I took part in the Embedded Fitness Challenge, which served as my final term project for the Embedded Systems course at NYU Tandon. The purpose of the project is to build a fitness tracker using the Adafruit Circuit Playground and program it using Arduino IDE.

## Objective
The COVID pandemic has forced almost all gyms to close until further notice. Because of this, many of us have become sedentary and are suffering from lack of sufficient exercise causing fatigue and obesity creating the need for new exercising options. The objective of this semester’s embedded challenge is to use the data collected from a single accelerometer (on your microcontroller board) to record body movements to identify one of 4 exercises:
* Situps
* Pushups
* Jumping Jacks
* Squats

## How to Use
1. First, clip the device onto the front of your collar.
2. Turn on the device. The main menu will show four distinct pairs of lit-up LEDs. One pair is color-coded to a specific exercise:
    - Top Left (Green): Jumping Jacks
    - Bottom Left (Blue): Squats
    - Bottom Right (Purple): Situps
    - Top Right (Pink): Pushups
Colors that appear on the menu correspond to exercise routines that are not yet complete.
3. Get into the initial/starting position of the type of exercise you wish to perform.
4. To begin tracking your exercise, press either of the two buttons on your device. The device will beep and the LEDs will turn white, indicating that it is actively tracking your starting position and movement.
5. If the device begins to beep and blink in a color correspoding to a specific exercise, then the device has detected the exercise. Otherwise, begin performing your exercise until the device detects it.
6. The device will track your progress on your exercise routine. One routine consists of 10 repetitions. For each repetition you perform, one LED will light up. Continue your exercise routine until all 10 LEDs light up.
7. Once you've completed your routine, the device will play a victory song and return to the main menu. Colors that correspond to the completed exercise routine will no longer appear on the main menu.
8. Repeat steps 3 - 7 for the rest of the exercises shown on the main menu.
9. To reset your progress, simply turn the device off then on again.
