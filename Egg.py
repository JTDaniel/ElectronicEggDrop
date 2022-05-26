from machine import ADC, Pin
import utime

X = 26
Y = 27
Z = 29
cutoff = 40000

Rpin = 0
Gpin = 1

RLED=Pin(Rpin,Pin.OUT)
GLED=Pin(Gpin,Pin.OUT)

GLED.toggle()

Xref = ADC(Pin(X))
Yref = ADC(Pin(Y))
Zref = ADC(Pin(Z))
    


while (True): 
    Xval = Xref.read_u16()
    Yval = Yref.read_u16()
    Zval = Zref.read_u16()

    utime.sleep(0.05)


    Xval2 = Xref.read_u16()
    Yval2 = Yref.read_u16()
    Zval2 = Zref.read_u16()
    
    if (Xval - Xval2 > cutoff or Yval - Yval2 > cutoff or Zval - Zval2 > cutoff):
        GLED.toggle()
        RLED.toggle()
        utime.sleep(3)
        GLED.toggle()
        RLED.toggle()
    print(Xval)

    
          
