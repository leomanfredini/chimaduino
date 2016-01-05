// CHIMADUINO
// Termômetro Eletrônico para Água do Chimarrão
//
// Leonardo Manfredini
// Adaptado do código original disponível em http://blog.filipeflop.com/sensores/sensor-de-temperatura-ds18b20-arduino.html


#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
 
// Porta do pino de sinal do DS18B20
#define ONE_WIRE_BUS 3
 
// Define uma instancia do oneWire para comunicacao com o sensor
OneWire oneWire(ONE_WIRE_BUS);
 
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;
 
// Inicializa o LCD
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
 
void setup(void)
{
	Serial.begin(9600);
	sensors.begin();
	// Localiza e mostra enderecos dos sensores
	Serial.println("Localizando sensores DS18B20...");
	Serial.print("Foram encontrados ");
	Serial.print(sensors.getDeviceCount(), DEC);
	Serial.println(" sensores.");
	if (!sensors.getAddress(sensor1, 0)) 
		Serial.println("Sensores nao encontrados !"); 
	// Mostra o endereco do sensor encontrado no barramento
	Serial.print("Endereco sensor: ");
	mostra_endereco_sensor(sensor1);
	Serial.println();
	Serial.println();
	lcd.begin(16, 2);
	//OPCIONAL
	//Define o pino 2 para alimentação da luz de fundo do LCD (pino 15 do LCD)
	pinMode(2, OUTPUT);
	digitalWrite(2, HIGH);
   
}
 
void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// Adiciona zeros se necessário
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}
 
void loop()
{
	// Le a informacao do sensor
	sensors.requestTemperatures();
	float tempC = sensors.getTempC(sensor1);

	// Mostra dados no serial monitor
	Serial.print("Temp C: ");
	Serial.println(tempC);

	// Mostra dados no LCD  
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Temp.:       ");
	//Simbolo grau
	lcd.write(223);
	lcd.print("C");
	lcd.setCursor(7,0);
	lcd.print(tempC);

	// Exibe as mensagens no LCD conforme as faixas de temperatura
	if (tempC < 15) {
		lcd.setCursor(3,1);
		lcd.print("GELADA!!!");
	} else if ((tempC >= 15) && (tempC < 35)){
		lcd.setCursor(1,1);
		lcd.print("AINDA FRIA!!!");        
	} else if ((tempC >= 35) && (tempC < 50)){
		lcd.setCursor(2,1);
		lcd.print("AQUECENDO...");
	} else if ((tempC >= 50) && (tempC < 60)){
		lcd.setCursor(0,1);
		lcd.print("SUBINDO MAIS...");
	} else if ((tempC >= 60) && (tempC < 70)){
		lcd.setCursor(1,1);
		lcd.print("QUASE NO PONTO");
	} else if ((tempC >= 70) && (tempC < 76)){
		lcd.setCursor(0,1);
		lcd.print("BAGUAL P/ CHIMA");
		//Pisca a luz de fundo do display LCD
		digitalWrite(2, LOW);
		delay(200);
		digitalWrite(2, HIGH);
		delay(200);
		digitalWrite(2, LOW);
		delay(200);
		digitalWrite(2, HIGH);
		delay(200);
	} else if ((tempC >= 76) && (tempC < 80)){
		lcd.setCursor(0,1);
		lcd.print("PASSOU DO PONTO!");
	} else if (tempC >= 80){
		lcd.setCursor(0,1);
		lcd.print("AGUA PELA PORCO!");
	}  
	delay(3000);
}
