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
  for (int i=0; i < n-1; i++) { // Función de interpolación lineal para 
                                //calcular pesos intermedios entre los ptos declarados.
    if (x >= lectura[i] && x <= lectura[i+1]) {
      float m=(peso[i+1]-peso[i]) / (lectura[i+1]-lectura[i]);
      return  m*(x-lectura[i])+peso[i];
      }
    }
    return -1.0;// Fuera de rango
}
void setup() {
  pinMode(pulgar,INPUT);
  pinMode(indice,INPUT);
  pinMode(corazon,INPUT);
  pinMode(anular,INPUT);
  pinMode(menique,INPUT);
  pinMode(pinBoton,INPUT);
  pinMode(luz,OUTPUT);
  Serial.begin(9600);
}
void loop() {
  if (digitalRead(pinBoton) == HIGH) {
    Serial.println("Empezando a medir");
    digitalWrite(luz, HIGH);
    int datosPulgar[numDatos];
    int datosIndice[numDatos];
    int datosCorazon[numDatos];
    int datosAnular[numDatos];
    int datosMenique[numDatos];
    for (int i = 0; i < numDatos; i++) {
      datosPulgar[i] = analogRead(pulgar);
      datosIndice[i] = analogRead(indice);
      datosCorazon[i] = analogRead(corazon);
      datosAnular[i] = analogRead(anular);
      datosMenique[i] = analogRead(menique);
      delay(500);
    }
    digitalWrite(luz, LOW);

    filtrado("Pulgar",datosPulgar,numDatos);
    filtrado("Indice",datosIndice,numDatos);
    filtrado("Corazon",datosCorazon,numDatos);
    filtrado("Anular",datosAnular,numDatos);
    filtrado("Menique",datosMenique,numDatos);
    delay(2000);
  } 
}

void filtrado(String nombre, int datos[], int total){
  float suma = 0;
  for (int i = 0; i < total; i++){
    suma += datos[i];
  }
  float media = suma/total;
  // Calcula desviación estandar para eliminar datos atipicos
  float sumaDesviacion = 0;
  for (int i = 0; i < total; i++){
    sumaDesviacion += pow(datos[i] - media, 2);
  }
  float desviacion = sqrt(sumaDesviacion/total);

  float limiteInferior = media - desviacion;
  float limiteSuperior = media + desviacion;

  float sumaFiltrada = 0;
  int contadorFiltrados = 0;

  for (int i = 0; i < total; i++){
    if (datos[i] >= limiteInferior && datos[i] <= limiteSuperior){
      sumaFiltrada += datos[i];
      contadorFiltrados++;
    }
  }
  float mediaFiltrada = (sumaFiltrada/contadorFiltrados);
  float kg = deLecturaAPeso(mediaFiltrada);

  Serial.print(nombre);
  Serial.print(": ");
  Serial.print(kg);
  Serial.print(" Kg | ");
}