#!/usr/bin/env python3

# NOTE: this example requires PyAudio because it uses the Microphone class

import serial
import speech_recognition as sr
import subprocess
import time

ser = serial.Serial()
ser.baudrate = 115200
ser.port = "/dev/cu.usbserial-A603UY7D"

r = sr.Recognizer()
r.operation_timeout = 30

def connect():

    while not ser.is_open:
        ser.open()
        ser.write(b"0")
        # serin = ser.read()
        # if ser.read() == b'2':
        print("connected")
        # connected = True

def recognize():
    # obtain audio from the microphone
    with sr.Microphone() as source:
        print("adjusting threshold based on ambient noise...")
        r.adjust_for_ambient_noise(source)
        print("Say something!")
        try:
            audio = r.listen(source, timeout=15)
        except sr.WaitTimeoutError:
            print("didn't hear anything")
            subprocess.run(["say", "-v", "Vicki", "{}".format("I did not hear anything")], stdout=subprocess.DEVNULL)
            audio = None

    if audio:
        try:
            # for testing purposes, we're just using the default API key
            # to use another API key, use `r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")`
            # instead of `r.recognize_google(audio)`
            recognized = r.recognize_sphinx(audio)
            print("Sphinx thinks you said: {}".format(recognized))
            # os.system("say '{}'".format(recognized))
            subprocess.run(["say", "-v", "Vicki", "{}".format(recognized)], stdout=subprocess.DEVNULL)
        except sr.UnknownValueError:
            subprocess.run(["say", "-v", "Vicki", "{}".format("Sorry, I did not understand")], stdout=subprocess.DEVNULL)
            print("Sphinx could not understand audio")
        except sr.RequestError as e:
            print("Sphinx error; {0}".format(e))

if __name__ == '__main__':
    connect()

    incoming = ser.read()
    if incoming == b'1':
        subprocess.run(["say", "-v", "Vicki", "{}".format("grate triggered")], stdout=subprocess.DEVNULL)
        recognize()
        time.sleep(2)
        ser.write(b"0")
    else:
        subprocess.run(["say", "-v", "Vicki", "{}".format("what is that")], stdout=subprocess.DEVNULL)
