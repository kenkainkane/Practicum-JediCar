int cmd = 5;
int state = 5;
int count = 0;
void setup()
{
  Serial.begin(38400);
  Serial.println("connected");
  pinMode(PIN_PC0, OUTPUT); // in1 //LB
  pinMode(PIN_PC1, OUTPUT); // in2 //LF
  pinMode(PIN_PC2, OUTPUT); // in3 //RF
  pinMode(PIN_PC3, OUTPUT); // in4 //RB
  pinMode(PIN_PB1, OUTPUT); // enA
  pinMode(PIN_PB2, OUTPUT); // enB
}

void forward()
{
  digitalWrite(PIN_PC2, HIGH);
  digitalWrite(PIN_PC3, LOW);
  analogWrite(PIN_PB2, 200);

  digitalWrite(PIN_PC0, LOW);
  digitalWrite(PIN_PC1, HIGH);
  analogWrite(PIN_PB1, 200);
}

void backward()
{
  digitalWrite(PIN_PC2, LOW);
  digitalWrite(PIN_PC3, HIGH);
  analogWrite(PIN_PB2, 200);

  digitalWrite(PIN_PC0, HIGH);
  digitalWrite(PIN_PC1, LOW);
  analogWrite(PIN_PB1, 200);
}

void turnright()
{
  digitalWrite(PIN_PC2, LOW);
  digitalWrite(PIN_PC3, LOW);
  analogWrite(PIN_PB2, 200);

  digitalWrite(PIN_PC0, LOW);
  digitalWrite(PIN_PC1, HIGH);
  analogWrite(PIN_PB1, 200);
}

void turnleft()
{
  digitalWrite(PIN_PC2, HIGH);
  digitalWrite(PIN_PC3, LOW);
  analogWrite(PIN_PB2, 200);

  digitalWrite(PIN_PC0, LOW);
  digitalWrite(PIN_PC1, LOW);
  analogWrite(PIN_PB1, 200);
}


void freeze()
{
  digitalWrite(PIN_PC2, LOW);
  digitalWrite(PIN_PC3, LOW);
  analogWrite(PIN_PB2, 200);

  digitalWrite(PIN_PC0, LOW);
  digitalWrite(PIN_PC1, LOW);
  analogWrite(PIN_PB1, 200);
}


void loop()
{
  int cmd = Serial.read();
  if (count < 3){
    if(cmd == '1'){
      forward();
      state = '1';
    }
    else if(cmd == '2'){
      backward();
      state = '2';
    }
    else if(cmd == '3'){
      turnleft();
      state = '3';
    }
    else if(cmd == '4'){
      turnright();
      state = '4';
    }
    else if(cmd == '5' && state != '5'){
      if (state == '1'){
        forward();    
      }
      else if(state == '2'){
        backward();
      }
      else if(state == '3'){
        turnleft();
      }
      else if(state == '4'){
        turnright();
      }
    }
    count++;
  }
  else{
    freeze();
    count = 0;
  }
  delay(1000);
}
