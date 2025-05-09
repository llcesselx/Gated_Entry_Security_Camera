// include liquid crytal library 
#include <LiquidCrystal.h>
// include library for servo motor
#include <Servo.h>
// include library for ttl camera and software serial connection for camera
#include <Adafruit_VC0706.h>
#include <SoftwareSerial.h>
// include library for SD card
#include <SD.h>
// include library that houses functions for the IR remote
#define DECODE_NEC
#undef DECODE_SONY
#undef DECODE_RC5
#undef DECODE_PANASONIC
#undef DECODE_JVC
#include <IRremote.h>


// ******************* LIBRARY DEFINITIONS ****************

/////////////////////////////////////////////////////////
// LCD SCREEN ///////////////////////////////////////////
/////////////////////////////////////////////////////////
LiquidCrystal screen(A0, A1, A2, A3, A4, A5);

/////////////////////////////////////////////////////////
// SERVO MOTOR //////////////////////////////////////////
/////////////////////////////////////////////////////////
Servo myServo;
int originalPos = 10; 

/////////////////////////////////////////////////////////
// ADAFRUIT TTL CAMERA //////////////////////////////////
/////////////////////////////////////////////////////////
SoftwareSerial cameraConnection(2, 3); // RX, TX
Adafruit_VC0706 camera = Adafruit_VC0706(&cameraConnection);
uint8_t cameraBuffer[32];
char imageFilename[13];

/////////////////////////////////////////////////////////
// IR SENSOR & REMOTE ///////////////////////////////////
/////////////////////////////////////////////////////////
// Define the signal pin for the IR reciever as PWM pin 3
int irsensor = 6; 
// Create instance of IRrecv named irsig (IR Signal) using data from pin 3 (irsensor)
IRrecv irsig(irsensor);   
// variable uses to store the last decodedRawData
// leading u says that type is unsigned
// int says it is a integer value (not necessarily an int)
// the value defines the number of bits used to store the value (8, 16, 32, etc.)
// _t says that sizes are standard across all platforms
uint32_t last_decodedRawData = 0;

/////////////////////////////////////////////////////////
// PROGMEM VARIABLES ////////////////////////////////////
/////////////////////////////////////////////////////////
const char msgMotionDetected[] PROGMEM = "Motion Detected!";
const char msgEnterID[] PROGMEM = "EnterID:";
const char msgAccessGranted[] PROGMEM = "Access Granted";
const char msgAccessDenied[] PROGMEM = "Access Denied";
const char msgLookingFor[] PROGMEM = "Looking For";
const char msgMotion[] PROGMEM = "Motion...";

char buffer[17];


// ****************** PROGRAM FUNCTIONS *****************

// Depending on what button is pressed will output its value in HEX and user value
// If user holds button down, remote will detect a same value as old value and 
// will read it as a repeat
void translateIR(uint32_t code) {

  char label[10] = "";

  // Serial.print("Decoded IR: 0x");
  // Serial.println(code, HEX);  

  if (code == 0xE916FF00) {
    strcpy(label, "Lea");
    screen.setCursor(0, 1);
    screen.print(label);
    delay(1000);
    accessGranted();
  } 
  else {
    strcpy(label, "Unknown");
    screen.setCursor(0, 1);
    screen.print(label);
    delay(1000);
    accessDenied();
  }

  screen.clear();
}


void checkIR(){
  // checks to see if ir signal is present
  screen.clear();
  screen.setCursor(0, 0);
  strcpy_P(buffer, msgEnterID);
  screen.print(buffer);
  irsig.resume();

  while (!irsig.decode()){
    ;
  }
  if (irsig.decode()){
    uint32_t code = irsig.decodedIRData.decodedRawData;
    // .resume readys the IR sensor to read the next signal
    // irsig.resume();
    translateIR(code);
  }
  irsig.resume();
}

void openGate(){
  delay(10);
  myServo.write(120);
  delay(5000);
  myServo.write(originalPos);
  delay(200);
}

void motionDetected(){
  camera.setMotionDetect(false);
  screen.clear();
  screen.setCursor(0, 0);
  strcpy_P(buffer, msgMotionDetected);
  screen.print(buffer);
  delay(500);
  saveSnapshot(imageFilename);
  delay(100);
  checkIR();
  delay(100);
  camera.setMotionDetect(true);
  delay(50);
}

void accessGranted(){
  screen.clear();
  screen.setCursor(0, 0);
  strcpy_P(buffer, msgAccessGranted);
  screen.print(buffer);
  openGate();
  screen.clear();
}

void accessDenied(){
  screen.clear();
  screen.setCursor(0, 0);
  strcpy_P(buffer, msgAccessDenied);
  screen.print(buffer);
  delay(1000);
  screen.clear();
}

// void logAccess(String name, String imageFile) {
//   File logFile = SD.open("log.txt", FILE_WRITE);
//   if (logFile) {
//     logFile.print("User: ");
//     logFile.print(name);
//     logFile.print(" | Image: ");
//     logFile.print(imageFile);
//     logFile.print(" | Time: ");
//     logFile.println(millis());  // Optionally replace with real-time clock later
//     logFile.close();
//     // Serial.println("Access logged.");
//   } 
//   else {
//     ;
//     // Serial.println("Failed to write to log.txt");
//   }
// }

// Helper function to convert string to uppercase
char *strupr(char *s) {
  char *p = s;
  while (*p) {
    *p = toupper(*p);
    p++;
  }
  return s;
}

bool saveSnapshot(char* outFilename) {
  Serial.println("Taking picture...");
  if (!camera.takePicture()) {
    // Serial.println("Camera failed to take picture!");
    return false;
  }

  uint16_t jpglen = camera.frameLength();
  // Serial.print("Image size: ");
  // Serial.println(jpglen);

  for (uint8_t i = 0; i < 100; i++) {
    sprintf(outFilename, "IMG%03d.JPG", i);
    strupr(outFilename);
    if (!SD.exists(outFilename)) break;
  }

  // Serial.print("Saving as: ");
  // Serial.println(outFilename);

  delay(50);
  File imgFile = SD.open(outFilename, FILE_WRITE);
  if (!imgFile) {
    // Serial.println("Failed to open file for writing.");
    return false;
  }

  while (jpglen > 0) {
    uint8_t bytesToRead = min(32, jpglen);
    uint8_t *buffer = camera.readPicture(bytesToRead);
    if (!buffer) {
      // Serial.println("Camera readPicture returned NULL.");
      break;
    }

    imgFile.write(buffer, bytesToRead);
    jpglen -= bytesToRead;
  }

  imgFile.close();
  Serial.println("Image saved successfully.");
  return true;
}

// ****************** SETUP *****************************

void setup(){

  // begin serial monitor
  Serial.begin(9600);

  // start the LCD screen
  screen.begin(16, 2);

  // attach servo to pin 5 and move to start position 0
  myServo.attach(5);
  myServo.write(originalPos);

  SD.begin(10);

  if (!SD.begin(10)) {
    Serial.println("SD init failed!");
    return;
  }

  // File testFile = SD.open("test.txt", FILE_WRITE);
  // if (testFile) {
  //   testFile.println("SD card is working!");
  //   testFile.close();
  //   Serial.println("Test file written.");
  // } 
  // else {
  //   Serial.println("Failed to write test file.");
  // }

  // start camera
  cameraConnection.begin(38400);
  camera.setImageSize(VC0706_320x240);
  camera.setMotionDetect(true);

  // start ir sensor receiver
  irsig.enableIRIn(); // Start the receiver
}

// ******************* LOOP *****************************

void loop(){
  // let user know looking for motion...
  screen.setCursor(0, 0);
  strcpy_P(buffer, msgLookingFor);
  screen.print(buffer);
  screen.setCursor(0, 1);
  strcpy_P(buffer, msgMotion);
  screen.print(buffer);

  if (camera.motionDetected()) {
    motionDetected();
  }
}
