// SPISettings MAX17841(4000000, MSBFIRST, SPI_MODE0);

// /************************************
// * Reads data from all slave devices *
// ************************************/
// void readAllSlaves(uint8_t dataRegister, boolean setupDone) // Read all slaves
// {
// 	uint8_t command = 0x03; // READALL
// 	uint8_t byteList[3] = {command, dataRegister, 0x00};
// 	uint8_t PEC = calculatePEC(byteList, 3);
// 	uint8_t readRegisterData[Message_length]; //change
// 	uint8_t errorByte = 0x00;
// 	static uint8_t resendCounter = 0;
// 	//static uint8_t failCounter = 0;
	
// 	/*if(failCounter >= 4)
// 	{
// 		//clearBuffers();
// 		failCounter = 0; // Reset counter
// 		Serial.println("Buffers cleared");
// 	}*/
 
// 	SPI.beginTransaction(MAX17841);
	
// 	// Load the READALL command sequence into the load queue
// 	digitalWrite(SPI_MAX_CS_PIN, LOW);
//   SPI.transfer(0xC0); // WR_LD_Q SPI command byte (write the load queue)
//   SPI.transfer(Message_length); // Message length (5 + 2 x n = 29) //change
//   SPI.transfer(command); // READALL command byte
//   SPI.transfer(dataRegister); // Register address
//   SPI.transfer(0x00); // Data-check byte (seed value = 0x00)
//   SPI.transfer(PEC); // PEC byte
//   SPI.transfer(0x00); // Alive-counter byte (seed value = 0x00)
//   digitalWrite(SPI_MAX_CS_PIN, HIGH); 
  
//   transmitQueue();
	
// 	// Read the receive buffer
// 	digitalWrite(SPI_MAX_CS_PIN, LOW);
// 	SPI.transfer(0x93); // RD_NXT_MSG SPI command byte
	
// 	for(int i=0; i<Message_length; i++) //change
//   {
// 		readRegisterData[i] = SPI.transfer(0x93);
// 	}
	
// 	errorByte |= receiveBufferError();
	
// 	SPI.endTransaction();
	
// 	// Verify that the device register data is received correctly during the READALL sequence
// 	if(!((readRegisterData[0] == command) && (readRegisterData[1] == dataRegister)))
//   {
// 		errorByte |= mismatchBefore; 
// 	}
// 	if(setupDone)
// 	{
// 		uint8_t checkPEC = calculatePEC(readRegisterData, (Message_length-2) ); //change
// 		// Check check-byte, PEC and alive-counter	
// 		if(!((readRegisterData[(Message_length-3)] == 0x00) && (readRegisterData[(Message_length-2)] == checkPEC) && (readRegisterData[(Message_length-1)] == numberOfModules))) //change
// 		{
// 			errorByte |= mismatchAfter;
// 			if(readRegisterData[(Message_length-3)] != 0x00) // change
// 			{
// 				readAllSlaves(0x02, false); // Read STATUS of all slaves with checks ignored
// 				writeAllSlaves(0x02, 0x0000, true); // Clear STATUS register
// 				Serial.println("STATUS cleared");
// 			}
// 		}
// 	}
	
// 	/*// Print data received
// 	for(int i=0; i<Message_length; i++)//change
//   {
// 		Serial.print(readRegisterData[i], HEX);
// 		Serial.print(" ");
// 	}
// 	Serial.println();*/

// 	if(errorByte) // Error
// 	{
//     resendCounter++;
//     Serial.println(errorByte, HEX);
//     errorByte &= 0x00; // Clear errors
//     Serial.println("errorByte cleared readAll");
    
//     if(resendCounter > 2)
//     {
// 			Serial.println("READALL fail");
// 			resendCounter = 0;
// 			//failCounter++;
// 			return;
// 		}
    
//     delay(1);
//     readAllSlaves(dataRegister, setupDone); // Resend READALL
//   }
//   else // No errors, clear counters and store data received
//   {
// 		resendCounter = 0; // Reset counter
// 		//failCounter = 0; // Reset counter
		
// 		if((dataRegister >= 0x20) && (dataRegister <= 0x2B)) // Cell voltage measurements
// 		{
// 			storeCellVoltage(dataRegister, readRegisterData);
// 		}
  
// 		if(dataRegister == 0x2D) // Aux voltage measurements (external temperature sensors)
// 		{
// 			storeCellTemperature(readRegisterData);
// 		}
		
// 		if(dataRegister == 0x50) // Die temperature measurements
// 		{
// 			storeDieTemperature(readRegisterData);
// 		}
		
// 		if(dataRegister == 0x13) // Read SCANCTRL to check if data is ready to be read
// 		{
// 			checkDataReady(readRegisterData);
// 		}
		
// 		if(dataRegister == 0x02) // Read STATUS and print content
// 		{
// 			// Print data received
// 			for(int i=0; i<Message_length; i++)
// 			{
// 				Serial.print(readRegisterData[i], HEX);
// 				Serial.print(" ");
// 			}
// 			Serial.println();
// 		}
// 	}  
// }

// /*****************************************
// * Reads data from addressed slave device *
// *****************************************/
// void readAddressedSlave(uint8_t dataRegister, uint8_t address, boolean setupDone) // Read addressed slave
// {
// 	uint8_t command = 0x05; // READDEVICE
// 	command |= (address << 3);
// 	uint8_t byteList[3] = {command, dataRegister, 0x00};
// 	uint8_t PEC = calculatePEC(byteList, 3);
// 	uint8_t readRegisterData[7];
// 	uint8_t errorByte = 0x00;
// 	static uint8_t resendCounter = 0;
// 	//static uint8_t failCounter = 0;
	
// 	/*if(failCounter >= 4)
// 	{
// 		//clearBuffers();
// 		failCounter = 0; // Reset counter
// 		Serial.println("Buffers cleared");
// 	}*/
		
// 	SPI.beginTransaction(MAX17841);
	
// 	// 1, Load the READDEVICE command sequence into the load queue
// 	digitalWrite(SPI_MAX_CS_PIN, LOW);
//   SPI.transfer(0xC0); // WR_LD_Q SPI command byte (write the load queue)
//   SPI.transfer(0x07); // Message length (5 + 2 x n = 29)
//   SPI.transfer(command); // READALL command byte
//   SPI.transfer(dataRegister); // Register address
//   SPI.transfer(0x00); // Data-check byte (seed value = 0x00)
//   SPI.transfer(PEC); // PEC byte
//   SPI.transfer(0x00); // Alive-counter byte (seed value = 0x00)
//   digitalWrite(SPI_MAX_CS_PIN, HIGH); 
  
//   transmitQueue();
	
// 	// 4, Read the receive buffer
// 	digitalWrite(SPI_MAX_CS_PIN, LOW);
// 	SPI.transfer(0x93); // RD_NXT_MSG SPI command byte
	
// 	for(int i=0; i<7; i++)
//   {
// 		readRegisterData[i] = SPI.transfer(0x93);
// 	}
	
// 	errorByte |= receiveBufferError();
	
// 	SPI.endTransaction();
	
// 	// Verify that the device register data is received correctly during the READDEVICE sequence
// 	if(!((readRegisterData[0] == command) && (readRegisterData[1] == dataRegister)))
//   {
// 		errorByte |= mismatchBefore; 
// 	}
// 	if(setupDone)
// 	{
// 		uint8_t checkPEC = calculatePEC(readRegisterData, 5);
// 		// Check check-byte, PEC and alive-counter		
// 		if(!((readRegisterData[4] == 0x00) && (readRegisterData[5] == checkPEC) && (readRegisterData[6] == 0x01)))
// 		{
// 			errorByte |= mismatchAfter;
// 			if(readRegisterData[4] != 0x00)
// 			{
// 				readAddressedSlave(0x02, address, false); // Read STATUS of addressed slave with checks ignored
// 				writeAddressedSlave(0x02, 0x0000, address, true); // Clear STATUS register
// 				Serial.println("STATUS cleared");
// 			}
// 		}
// 	}
	
// 	/*// Print data received
// 	for(int i=0; i<7; i++)
//   {
// 		Serial.print(readRegisterData[i], HEX);
// 		Serial.print(" ");
// 	}
// 	Serial.println();*/
	
// 	if(errorByte) // Error
// 	{
//     resendCounter++;
//     Serial.println(errorByte, HEX);
//     errorByte &= 0x00; // Clear errors
//     Serial.println("errorByte cleared readAddr");
    
//     if(resendCounter > 2)
//     {
// 			Serial.println("READEVICE fail");
// 			resendCounter = 0;
// 			//failCounter++;
// 			return;
// 		}
    
//     delay(1);
//     readAddressedSlave(dataRegister, address, setupDone); // Resend READDEVICE
//   }
//   else // No errors, clear counters and store data received
//   {
// 		resendCounter = 0; // Reset counter
// 		//failCounter = 0; // Reset counter
		
// 		if(dataRegister == 0x02) // Read STATUS and print content
// 		{
// 			// Print data received
// 			for(int i=0; i<7; i++)
// 			{
// 				Serial.print(readRegisterData[i], HEX);
// 				Serial.print(" ");
// 			}
// 			Serial.println();
// 		}
// 	} 
// }

// /***********************************
// * Writes data to all slave devices *
// ***********************************/
// void writeAllSlaves(uint8_t dataRegister, uint16_t data, boolean setupDone)
// {
// 	uint8_t command = 0x02; // WRITEALL
// 	uint8_t byteList[4] = {command, dataRegister, lowByte(data), highByte(data)};
// 	uint8_t PEC = calculatePEC(byteList, 4);
// 	uint8_t readRegisterData[6];
// 	uint8_t errorByte = 0x00;
// 	static uint8_t resendCounter = 0;
// 	//static uint8_t failCounter = 0;
	
// 	/*if(failCounter >= 4)
// 	{
// 		//clearBuffers();
// 		failCounter = 0; // Reset counter
// 		Serial.println("Buffers cleared");
// 	}*/
	
// 	SPI.beginTransaction(MAX17841);
	
// 	// 1, Load the WRITEALL command sequence into the load queue
// 	digitalWrite(SPI_MAX_CS_PIN, LOW);
//   SPI.transfer(0xC0); // WR_LD_Q SPI command byte (write the load queue)
//   SPI.transfer(0x06); // Message length
//   SPI.transfer(command); // WRITEALL command byte
//   SPI.transfer(dataRegister); // Register address
//   SPI.transfer(lowByte(data)); // LS byte of register data to be written
//   SPI.transfer(highByte(data)); // MS byte of register data to be written
//   SPI.transfer(PEC); // PEC byte
//   SPI.transfer(0x00); // Alive-counter byte (seed value = 0x00)
//   digitalWrite(SPI_MAX_CS_PIN, HIGH); 
  
//   transmitQueue();
	
// 	// 4, Read the receive buffer
// 	digitalWrite(SPI_MAX_CS_PIN, LOW);
// 	SPI.transfer(0x93); // RD_NXT_MSG SPI command byte
	
// 	for(int i=0; i<6; i++)
//   {
// 		readRegisterData[i] = SPI.transfer(0x93);
// 	}
//   digitalWrite(SPI_MAX_CS_PIN, HIGH);
	
// 	errorByte |= receiveBufferError();
  
//   SPI.endTransaction();
  
//   // Verify that the device register data is what was written during the WRITEALL sequence
//   if(!((readRegisterData[0] == command) && (readRegisterData[1] == dataRegister) && (readRegisterData[2] == lowByte(data)) && (readRegisterData[3] == highByte(data)) && (readRegisterData[4] == PEC)))
//   {
// 		errorByte |= mismatchBefore; 
// 	}
// 	if(setupDone)
// 	{
// 		// Check alive-counter
// 		if(!(readRegisterData[5] == numberOfModules)) // change
// 		{
// 			errorByte |= mismatchAfter;
// 		}
// 	}

// 	/*// Print data received
//   for(int i=0; i<6; i++)
//   {
//     Serial.print(readRegisterData[i], HEX);
//     Serial.print(" ");
//   }
//   Serial.println();*/
  
//   if(errorByte) // Error
// 	{
//     resendCounter++;
//     Serial.println(errorByte, HEX);
//     errorByte &= 0x00; // Clear errors
//     Serial.println("errorByte cleared writeAll");
    
//     if(resendCounter > 2)
//     {
// 			Serial.println("WRITEALL fail");
// 			resendCounter = 0;
// 			//failCounter++;
// 			return;
// 		}
    
//     delay(1);
//     writeAllSlaves(dataRegister, data, setupDone); // Resend WRITEALL
//   }
//   else // No errors, clear counters
//   {
// 		resendCounter = 0; // Reset counter
// 		//failCounter = 0; // Reset counter
// 	}
// }

// /****************************************
// * Writes data to addressed slave device *
// ****************************************/
// void writeAddressedSlave(uint8_t dataRegister, uint16_t data, uint8_t address, boolean setupDone) // Write addressed slave
// {
// 	uint8_t command = 0x04; // WRITEDEVICE
// 	command |= (address << 3);
// 	uint8_t byteList[4] = {command, dataRegister, lowByte(data), highByte(data)};
// 	uint8_t PEC = calculatePEC(byteList, 4);
// 	uint8_t readRegisterData[6];
// 	uint8_t errorByte = 0x00;
// 	static uint8_t resendCounter = 0;
// 	//static uint8_t failCounter = 0;
	
// 	/*if(failCounter >= 4)
// 	{
// 		//clearBuffers();
// 		failCounter = 0; // Reset counter
// 		Serial.println("Buffers cleared");
// 	}*/
		
// 	SPI.beginTransaction(MAX17841);
	
// 	// 1, Load the WRITEDEVICE command sequence into the load queue
// 	digitalWrite(SPI_MAX_CS_PIN, LOW);
//   SPI.transfer(0xC0); // WR_LD_Q SPI command byte (write the load queue)
//   SPI.transfer(0x06); // Message length
//   SPI.transfer(command); // WRITEDEVICE command byte
//   SPI.transfer(dataRegister); // Register address
//   SPI.transfer(lowByte(data)); // LS byte of register data to be written
//   SPI.transfer(highByte(data)); // MS byte of register data to be written
//   SPI.transfer(PEC); // PEC byte
//   SPI.transfer(0x00); // Alive-counter byte (seed value = 0x00)
//   digitalWrite(SPI_MAX_CS_PIN, HIGH); 
  
//   transmitQueue();
	
// 	// 4, Read the receive buffer
// 	digitalWrite(SPI_MAX_CS_PIN, LOW);
// 	SPI.transfer(0x93); // RD_NXT_MSG SPI command byte
	
// 	for(int i=0; i<6; i++)
//   {
// 		readRegisterData[i] = SPI.transfer(0x93);
// 	}
//   digitalWrite(SPI_MAX_CS_PIN, HIGH);
	
// 	errorByte |= receiveBufferError();
  
//   SPI.endTransaction();
  
// 	// Verify that the device register data is what was written during the WRITEDEVICE sequence
// 	if(!((readRegisterData[0] == command) && (readRegisterData[1] == dataRegister) && (readRegisterData[2] == lowByte(data)) && (readRegisterData[3] == highByte(data)) && (readRegisterData[4] == PEC)))
//   {
// 		errorByte |= mismatchBefore; 
// 	}
// 	if(setupDone)
// 	{
// 		// Check alive-counter
// 		if(!(readRegisterData[5] == 0x01))
// 		{
// 			errorByte |= mismatchAfter;
// 		}
// 	}

// 	/*// Print data received
//   for(int i=0; i<6; i++)
//   {
//     Serial.print(readRegisterData[i], HEX);
//     Serial.print(" ");
//   }
//   Serial.println();*/
  
//   if(errorByte) // Error
// 	{
//     resendCounter++;
//     Serial.println(errorByte, HEX);
//     errorByte &= 0x00; // Clear errors
//     Serial.println("errorByte cleared writeAddr");
    
//     if(resendCounter > 2)
//     {
// 			Serial.println("WRITEDEVICE fail");
// 			resendCounter = 0;
// 			//failCounter++;
// 			return;
// 		}
    
//     delay(1);
//     writeAddressedSlave(dataRegister, data, address, setupDone); // Resend WRITEDEVICE
//   }
//   else // No errors, clear counters
//   {
// 		resendCounter = 0; // Reset counter
// 		//failCounter = 0; // Reset counter
// 	}
// }

/****************************************
* Writes data to addressed slave device *
****************************************/
void writeAddressedSlave(uint8_t dataRegister, uint16_t data, uint8_t address, boolean setupDone) // Write addressed slave
{
	uint8_t command = 0x04; // WRITEDEVICE
	command |= (address << 3);
	uint8_t byteList[4] = {command, dataRegister, lowByte(data), highByte(data)};
	uint8_t PEC = calculatePEC(byteList, 4);
	uint8_t readRegisterData[6];
	uint8_t errorByte = 0x00;
	static uint8_t resendCounter = 0;
	//static uint8_t failCounter = 0;
	
	/*if(failCounter >= 4)
	{
		//clearBuffers();
		failCounter = 0; // Reset counter
		Serial.println("Buffers cleared");
	}*/
		
	SPI.beginTransaction(MAX17841);
	
	// 1, Load the WRITEDEVICE command sequence into the load queue
	digitalWrite(SPI_MAX_CS_PIN, LOW);
    SPI.transfer(0xC0); // WR_LD_Q SPI command byte (write the load queue)
    SPI.transfer(0x06); // Message length
    SPI.transfer(command); // WRITEDEVICE command byte
    SPI.transfer(dataRegister); // Register address
    SPI.transfer(lowByte(data)); // LS byte of register data to be written
    SPI.transfer(highByte(data)); // MS byte of register data to be written
    SPI.transfer(PEC); // PEC byte
    SPI.transfer(0x00); // Alive-counter byte (seed value = 0x00)
    digitalWrite(SPI_MAX_CS_PIN, HIGH); 
  
    transmitQueue();
	
	// 4, Read the receive buffer
	digitalWrite(SPI_MAX_CS_PIN, LOW);
	SPI.transfer(0x93); // RD_NXT_MSG SPI command byte
	
	for(int i=0; i<6; i++)
    {
		readRegisterData[i] = SPI.transfer(0x93);
	}
    digitalWrite(SPI_MAX_CS_PIN, HIGH);
	
	errorByte |= receiveBufferError();
  
    SPI.endTransaction();
  
	// Verify that the device register data is what was written during the WRITEDEVICE sequence
	if(!((readRegisterData[0] == command) && (readRegisterData[1] == dataRegister) && (readRegisterData[2] == lowByte(data)) && (readRegisterData[3] == highByte(data)) && (readRegisterData[4] == PEC)))
    {
		errorByte |= mismatchBefore; 
	}
	if(setupDone)
	{
		// Check alive-counter
		if(!(readRegisterData[5] == 0x01))
		{
			errorByte |= mismatchAfter;
		}
	}

	/*// Print data received
    for(int i=0; i<6; i++)
    {
        Serial.print(readRegisterData[i], HEX);
        Serial.print(" ");
    }
    Serial.println();*/
  
    if(errorByte) // Error
	{
        resendCounter++;
        Serial.println(errorByte, HEX);
        errorByte &= 0x00; // Clear errors
        Serial.println("errorByte cleared writeAddr");

        if(resendCounter > 2)
        {
			Serial.println("WRITEDEVICE fail");
			resendCounter = 0;
			//failCounter++;
			return;
		}
    
        delay(1);
        writeAddressedSlave(dataRegister, data, address, setupDone); // Resend WRITEDEVICE
    }
    else // No errors, clear counters
    {
		resendCounter = 0; // Reset counter
		//failCounter = 0; // Reset counter
	}
}