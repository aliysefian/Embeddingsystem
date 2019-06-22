/*
14CORE NRF24L01 SLAVE/RECIEVER
*/

#include <EEPROM.h>
#include  <SPI.h> //Include SPI Code Library which can be downloaded below
#include "EEPROMAnything.h"
struct MyStruct
{
	char name[10];
	char family[10];
	char phone[10];

};
MyStruct user;
MyStruct user123;
MyStruct user4;
void setup(void) {
	Serial.begin(9600);
	String s = "ali";
	String family = "yousefian";
	String phone = "9137266252";
	 s.toCharArray(user.name,10);
	 family.toCharArray(user.family,10);
	 phone.toCharArray(user.phone,10);
	EEPROM.put(0, user); 

	phone = "123456789";
	s.toCharArray(user.name,10);
	 family.toCharArray(user.family,10);
	 phone.toCharArray(user.phone,10);
	EEPROM.put(sizeof(MyStruct)+1, user);

	
}

void loop(void) {

	EEPROM.get(0, user4);
	Serial.println(String(user4.name)+"family="+user4.family+"phone="+user4.phone);
	EEPROM.get(sizeof(MyStruct) + 1, user4);
	Serial.println(String(user4.name)+"family="+user4.family+"phone="+user4.phone);
	
	
	delay(30000);
}
void SaveScheduleEEporomInFirstState(char name,String family,int id) {
	int eeAddress = 0;
	
	eeAddress += id * sizeof(MyStruct);
	if (eeAddress != EEPROM.length()) {
	//	EEPROM.put(eeAddress, customVar);

	}
	for (int i = 0; i < 20; i++) {
	//	Serial.println("Written custom= " + customVar.name[i]);

	}
}
void getAllUser(int i) {
	int eeAddress = 0;
	MyStruct dor;
	
		eeAddress = i * sizeof(MyStruct);
		EEPROM.get(eeAddress, dor);
		Serial.println("fromeeprom="+String(dor.name));
		//Serial.println(dor.family);
		

		//Serial.println("Read custom object from EEPROM: ");
	
}