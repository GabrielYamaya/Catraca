#include <SoftwareSerial.h>
#include <RDM6300.h>

#define usuarios 3 //Número de usuários cadastrados

boolean check();

void aciona_motor();

//Inicializa a serial nos pinos 12 (RX) e 13 (TX) 
SoftwareSerial RFID(12, 13);


int Led = 4; //Configura o led na porta digital D2
int MotorA = 16; //Configura o motor na porta digital so
int MotorB = 5; //Configura o motor na porta digital so
int Sensor = 15; //Configura o sensor na porta digital sc

uint8_t Payload[6]; // used for read comparisons

uint8_t cartao_valido[usuarios][6]; // usado como um usuário válido

boolean confere = true;

RDM6300 RDM6300(Payload);

void setup()
{
  pinMode(Led, OUTPUT); //Define o pino do led como saida
  pinMode(MotorA, OUTPUT); //Define o pino do motor como saída
  pinMode(MotorB, OUTPUT); //Define o pino do motor como saída
  pinMode(Sensor, INPUT); //Define o pino do sensor como entrada
  
  //Inicializa a serial para o leitor RDM6300
  RFID.begin(9600);
  
  //Inicializa a serial para comunicacao com o PC
  Serial.begin(9600);
  
  //Informacoes iniciais
  Serial.println("Leitor RFID RDM6300");

  //usuários válidos
  cartao_valido[0][0] = 0x3;
  cartao_valido[0][1] = 0x25;
  cartao_valido[0][2] = 0x4A;
  cartao_valido[0][3] = 0x6B;
  cartao_valido[0][4] = 0xD9;
  
  cartao_valido[1][0] = 0x2;
  cartao_valido[1][1] = 0x6;
  cartao_valido[1][2] = 0x6C;
  cartao_valido[1][3] = 0x4D;
  cartao_valido[1][4] = 0x13;

  cartao_valido[2][0] = 0x43;
  cartao_valido[2][1] = 0x0;
  cartao_valido[2][2] = 0x4A;
  cartao_valido[2][3] = 0x6B;
  cartao_valido[2][4] = 0xD8;
}

void loop()
{
  //Apaga o led
  digitalWrite(Led, HIGH);

  Serial.println("Aproxime um cartão: ");
  //Aguarda a aproximacao da tag RFID
  while (RFID.available() > 0)
  {
    
    uint8_t c = RFID.read();
    if (RDM6300.decode(c))
    {
      if(check()){
        Serial.print("ID TAG: ");
        //Mostra os dados no serial monitor
        for (int i = 0; i < 5; i++) {
          Serial.print(Payload[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
        digitalWrite(Led, LOW);
        Serial.println("Acesso permitido. ");
        Serial.println("");
        aciona_motor();
      } else {
        Serial.print("Acesso negado. ");
        Serial.println("");
        for (int i = 0; i < 5; i++) {
          Serial.print(Payload[i], HEX);
          Serial.print(" ");
        }
      }
    }
  }

  //Aguarda 2 segundos e reinicia o processo
  delay(500);
}

boolean check() {
  for (int k = 0; k < usuarios; k++) {
    boolean confere = true; // Inicialize confere como true para cada novo usuário
    for (int j = 0; j < 5; j++) {
      if (Payload[j] != cartao_valido[k][j]) {
        confere = false;
        break; // Não é necessário continuar verificando se um byte não corresponde
      }
      delay(100);
    }
    if (confere) {
      return true; // Se todos os bytes correspondem, retorna true
    }
  }
  return false; // Se nenhum usuário corresponder, retorna false
}

void aciona_motor(){
  digitalWrite(MotorA, HIGH);
  digitalWrite(MotorB, LOW);
  delay(2000);
  while(true){
    Serial.println("Motor rodando ");
    if(digitalRead(Sensor) != true){
      Serial.println("Motor parado ");
      digitalWrite(MotorA, LOW);
      digitalWrite(MotorB, LOW);
      break;
    }
    delay(500);
  }  
}
