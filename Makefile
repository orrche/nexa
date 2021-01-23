
BOARDNAME="arduino:avr:mynano:cpu=atmega328"

TARGET="$(CURDIR)/output"

nexa.ino.hex: nexa.ino
	test -n "$(ARDUINOPATH)"  # $$ARDUINOPATH
	mkdir -p $(TARGET)
	
	$(ARDUINOPATH)/arduino-builder \
		-build-path $(TARGET) \
		-hardware hardware \
		-hardware $(ARDUINOPATH)/hardware \
		-tools $(ARDUINOPATH)/hardware/tools/avr/ \
		-tools $(ARDUINOPATH)/tools-builder/ \
		-libraries $(ARDUINOPATH)/libraries \
		-libraries libraries \
		-fqbn $(BOARDNAME) \
		$<
	
	cp $(TARGET)/$<.hex .

install: nexa.ino.hex
	 avrdude -B 5 -v -c arduino -p m328p -P /dev/ttyUSB0 -e -U flash:w:nexa.ino.hex

.PHONY: clean

clean:
	rm -rf $(TARGET)
	rm nexa.ino.hex
