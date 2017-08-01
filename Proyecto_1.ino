#include <Servo.h>

#define PIN_BARRIER_SERVO 3
#define PIN_PEDESTRIAN_SENSOR A5
#define PIN_GREEN_LED 5
#define PIN_RED_LED 6
#define TRIGGER_PIN 2
#define ECHO_PIN 4

#define STATE_VEHICULES 0
#define STATE_TRAIN 1
#define STATE_PEDESTRIANS 2
#define STATE_VEHICULES_TO_TRAIN 3
#define STATE_VEHICULES_TO_PEDESTRIANS 4
#define STATE_PEDESTRIANS_TO_TRAIN 5
#define STATE_PEDESTRIANS_TO_VEHICULES 6
#define STATE_TRAIN_TO_VEHICULES 7
#define STATE_TRAIN_TO_PEDESTRIANS 8

#define DISTANCE_MAX_READS 10


Servo myservo; 
byte state = STATE_VEHICULES;
byte lastState = STATE_VEHICULES;
int lastDigit = 9;
unsigned long previousMillisGreenLight = 0; 
unsigned long previousMillisPedestrians = 0; 
unsigned long previousMillisDisplay = 0; 
bool lastGreenLight = false;
bool pedestriansCooldown = false;
bool pedestrialsSignalWhileTrain = false;


void setup(){
    Serial.begin(9600);
    pinMode(PIN_PEDESTRIAN_SENSOR, INPUT);
    myservo.attach(PIN_BARRIER_SERVO);
    pinMode(PIN_RED_LED, OUTPUT); 
    pinMode(PIN_GREEN_LED, OUTPUT); 
    pinMode(TRIGGER_PIN, OUTPUT); 
    pinMode(ECHO_PIN, INPUT); 

    openBarrier();
    redTrafficLight();
}

void loop(){
    if (lastState != state) {
        displayState();
        lastState = state;
    }
    bool isTrain = false;
    switch (state)
    {
        case STATE_VEHICULES:
            isTrain = checkTrain();
            if (isTrain)
                state = STATE_VEHICULES_TO_TRAIN;
            else if (checkPedestrians())
                state = STATE_VEHICULES_TO_PEDESTRIANS;
            break;

        case STATE_PEDESTRIANS:
            isTrain = checkTrain();
            if (isTrain) 
                state = STATE_PEDESTRIANS_TO_TRAIN;
            else {
                doPedestriansSignals();
                if (endPedestriansTime()) 
                    state = STATE_PEDESTRIANS_TO_VEHICULES;
            }
            break;
        
        case STATE_TRAIN:
            isTrain = checkTrain();
            if (isTrain && checkPedestrians())
                pedestrialsSignalWhileTrain = true;
            else if (!isTrain) {    
                if (checkPedestrians() || pedestrialsSignalWhileTrain) {
                    state = STATE_TRAIN_TO_PEDESTRIANS;
                    pedestrialsSignalWhileTrain = false;
                } else 
                    state = STATE_TRAIN_TO_VEHICULES;
            } 
            break;

        case STATE_VEHICULES_TO_PEDESTRIANS:
            outVehiculesState();
            inPedestriansState();
            state = STATE_PEDESTRIANS;
            break;
        
        case STATE_PEDESTRIANS_TO_VEHICULES:
            outPedestriansState();
            inVehiculesState();
            state = STATE_VEHICULES;
            break;
            
        case STATE_VEHICULES_TO_TRAIN:
            outVehiculesState();
            inTrainState();
            state = STATE_TRAIN;
            break;

        case STATE_TRAIN_TO_PEDESTRIANS:
            outTrainState();
            inPedestriansState();
            state = STATE_PEDESTRIANS;
            break;

        case STATE_TRAIN_TO_VEHICULES:
            outTrainState();
            inVehiculesState();
            state = STATE_VEHICULES;
            break;

        case STATE_PEDESTRIANS_TO_TRAIN:
            outPedestriansState();
            inTrainState();
            state = STATE_TRAIN;
            break;

    }
    //delay(500);
}

void outVehiculesState() {
}
void inVehiculesState() {
    openBarrier();
    redTrafficLight();
}
void inPedestriansState() {
    closeBarrier();
    greenTrafficLight(false);
    beep(false);
}
void outPedestriansState() {
    clearDisplay();
}
void inTrainState() {
    closeBarrier();
    redTrafficLight();
}
void outTrainState() {
}

void doPedestriansSignals() {
    counter(false);
    if (lastDigit < 5 ) {
        greenTrafficLight(true);
        beep(true);
    }
}

bool endPedestriansTime() {
    if (lastDigit < 0) {
        lastDigit = 9;
        pedestriansCooldown = true;
        return true;
    }
    return false;
}

bool checkPedestrians() {
    //checks the pedestrians button
    bool pressed = digitalRead(PIN_PEDESTRIAN_SENSOR) == HIGH;
    if (pressed) {
        long elapsed = (millis() - previousMillisPedestrians);
        Serial.println(String("pressed")  + " elapsed:" + elapsed);
        if (elapsed > 18000 || previousMillisPedestrians == 0) {
            previousMillisPedestrians = millis();
            return true;
        } else 
            return false;
    }
    return false;
}

bool checkTrain() {
    //checks the train sensor
    if (measureMedia() < 100) {
        Serial.println("Train happens");
        return true;
    } 
    return false;
}
