void displayState() {
    Serial.print("estado:");
    Serial.println(state);
}

void counter(bool show) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisDisplay >= 1000) {
        previousMillisDisplay = currentMillis;
        /*if (show) {
            for (int i = 0 ; i < 8 ; i++)
                digitalWrite(i + PIN_DISPLAY_INIT, !bitRead( Digit[lastDigit], i) );
        }*/
        lastDigit--;
    } 
}

unsigned int measure()
{
    digitalWrite(TRIGGER_PIN,LOW); 
    delayMicroseconds(5);
    digitalWrite(TRIGGER_PIN, HIGH); 
    delayMicroseconds(10);
    unsigned int distance= int(0.017*pulseIn(ECHO_PIN, HIGH));
    Serial.println(String("measure:") + distance);
    return constrain(distance, 1, 300);
}

unsigned int measureMedia()
{
    static int measurements[DISTANCE_MAX_READS];
    long mean = 0;
    for (int i = 0; i < DISTANCE_MAX_READS - 1; i++)
    {
        measurements[i] = measurements[i + 1];
        mean += measurements[i];
    }
    measurements[DISTANCE_MAX_READS - 1] = measure();
    mean += measurements[DISTANCE_MAX_READS - 1];
    Serial.println(String("measureMedia:") + mean);
    return mean /= DISTANCE_MAX_READS;
}


void redTrafficLight() {
    digitalWrite(PIN_RED_LED, HIGH);
    digitalWrite(PIN_GREEN_LED, LOW);
}

void greenTrafficLight(bool hurry) {
    if(hurry) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisGreenLight >= 500) {
            previousMillisGreenLight = currentMillis;
            lastGreenLight = !lastGreenLight;
            digitalWrite(PIN_GREEN_LED, lastGreenLight);
        }
    } else {
        digitalWrite(PIN_GREEN_LED, HIGH);
    }
    digitalWrite(PIN_RED_LED, LOW);
}

void clearDisplay() {
    /*for (int i = 0 ; i < 8 ; i++)
        digitalWrite(i + PIN_DISPLAY_INIT, HIGH );
    */
}

void beep(bool hurry) {

}

void closeBarrier(){
    Serial.println("closeBarrier");
	myservo.write(0); 
}

void openBarrier(){
    Serial.println("openBarrier");
	myservo.write(90); 
}