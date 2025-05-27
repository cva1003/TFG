int pulgar=A0;
int indice=A1;
int corazon=A2;
int anular=A3;
int menique=A4;
int pinBoton=2;
int luz=4;
float deLecturaAPeso(int x){
  int lectura[]={};
  float peso[]={0,0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5};
  int n=10;
  if (x < lectura[0] || x > lectura[n-1]) return -1.0;  // Fuera de rango
  for (int i=0; i < n-1; i++) { // Función de interpolación lineal para calcular pesos intermedios entre los ptos declarados
    if (x >= lectura[i] && x <= lectura[i+1]) {
      float m=(peso[i+1]-peso[i]) / (lectura[i+1]-lectura[i]);
      return  m*(x-lectura[i])+peso[i];
        }
    }
    return -1.0; // Fuera de rango
}
void setup() {
  pinMode(pulgar,INPUT);
  pinMode(indice,INPUT);
  pinMode(corazon,INPUT);
  pinMode(anular,INPUT);
  pinMode(menique,INPUT);
  pinMode(pinBoton,INPUT);
  pinMode(luz, OUTPUT); 
  Serial.begin(9600);
}
void loop() {
  if (digitalRead(pinBoton)==HIGH){
    Serial.println("Empezando a medir");
    digitalWrite(4, HIGH);
    int maxPulgar=0;
    int maxIndice=0;
    int maxCorazon=0;
    int maxAnular=0;
    int maxMenique=0;
    
    unsigned long empiece=millis();
    while (millis()-empiece <10000){
      maxPulgar=max(maxPulgar,analogRead(pulgar));
      maxIndice=max(maxIndice,analogRead(indice));
      maxCorazon=max(maxCorazon,analogRead(corazon));
      maxAnular=max(maxAnular,analogRead(anular));
      maxMenique=max(maxMenique,analogRead(menique));
      delay(50);
    }
    // Transforma el dato máximo recibido a Kg
        Serial.print("Pulgar: ");
        Serial.print(deLecturaAPeso(maxPulgar));
        Serial.print(" Kg | ");

        Serial.print("Indice: ");
        Serial.print(deLecturaAPeso(maxIndice));
        Serial.print(" Kg | ");

        Serial.print("Corazon: ");
        Serial.print(deLecturaAPeso(maxCorazon));
        Serial.print(" Kg | ");

        Serial.print("Anular: ");
        Serial.print(deLecturaAPeso(maxAnular));
        Serial.print(" Kg | ");

        Serial.print("Menique: ");
        Serial.print(deLecturaAPeso(maxMenique));
        Serial.println(" Kg");

        delay(2000);
    }
    else {
        digitalWrite(4, LOW);
  		}
  }