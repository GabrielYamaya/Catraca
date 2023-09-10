#include <SoftwareSerial.h>
#include <RDM6300.h>

boolean check();

//Inicializa a serial nos pinos 12 (RX) e 13 (TX) 
SoftwareSerial RFID(12, 13);

//Configura o led na porta digital D2
int Led = 4;

uint8_t Payload[6]; // used for read comparisons

uint8_t cartao_valido[6][1]; // usado como um usuário válido

RDM6300 RDM6300(Payload);

void setup()
{
  //Define o pino do led como saida
  pinMode(Led, OUTPUT);
  
  //Inicializa a serial para o leitor RDM6300
  RFID.begin(9600);
  
  //Inicializa a serial para comunicacao com o PC
  Serial.begin(9600);
  
  //Informacoes iniciais
  Serial.println("Leitor RFID RDM6300");

  //usuários válidos
  cartao_valido[][] = {
    {0x3, 0x25, 0x4A, 0x6B, 0xD9},
    {0x43, 0x0, 0x4A, 0x6B, 0xD8},
    {0x2, 0x6, 0x6C, 0x4D, 0x11}
  };
}

void loop()
{
  //Apaga o led
  digitalWrite(Led, HIGH);
  
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
        //Aciona o led
        digitalWrite(Led, LOW);
        Serial.println("Acesso permitido. ");
        Serial.println("");
      } else {
        Serial.print("Acesso negado. ");
        Serial.println("");
      }
    }
  }

  //Aguarda 2 segundos e reinicia o processo
  delay(2000);
}

boolean check(){
  for(int j = 0; j < 5; j++){
    if(Payload[j] != cartao_valido[j]){
      return false;
      break;
    }
  }
  return true;
}
