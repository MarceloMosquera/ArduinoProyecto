static int measurements[DISTANCE_MAX_READS];
unsigned int measure()
{
    digitalWrite(TRIGGER_PIN,LOW); 
    delayMicroseconds(5);
    digitalWrite(TRIGGER_PIN, HIGH); 
    delayMicroseconds(10);
    unsigned int distance= int(0.017*pulseIn(ECHO_PIN, HIGH));
    //Serial.println(String("measure:") + distance);
    return constrain(distance, 1, 300);
}

void measurementsInit() {
    measurements[0] = measure();
    for (int i = 1; i < DISTANCE_MAX_READS; i++)
    {
        measurements[i] = measurements[0];
    }
}

unsigned int measureMedia()
{
    long mean = 0;
    for (int i = 0; i < DISTANCE_MAX_READS - 1; i++)
    {
        measurements[i] = measurements[i + 1];
        mean += measurements[i];
    }
    measurements[DISTANCE_MAX_READS - 1] = measure();
    mean += measurements[DISTANCE_MAX_READS - 1];
    //Serial.println(String("measureMedia:") + mean);
    return mean /= DISTANCE_MAX_READS;
}

