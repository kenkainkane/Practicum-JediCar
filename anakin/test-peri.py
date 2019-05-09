import usb
from practicum import find_mcu_boards, McuBoard, PeriBoard
from time import sleep
import serial

ser = serial.Serial('/dev/tty.Obi-Wan-SPPDev', 38400)
devs = find_mcu_boards()

if len(devs) == 0:
    print("*** No practicum board found.")
    exit(1)

mcu = McuBoard(devs[0])
print("*** Practicum board found")
print("*** Manufacturer: %s" % \
        mcu.handle.getString(mcu.device.iManufacturer, 256))
print("*** Product: %s" % \
        mcu.handle.getString(mcu.device.iProduct, 256))
peri = PeriBoard(mcu)

sleep(2)
print('ready')

while True:
    try:
        val = peri.get_cmd()
        ser.write(str(val))
        print(str(val))
        sleep(2)
    except usb.core.USBError as e:
        print(type(e),str(e))
        sleep(2)
        peri = PeriBoard(McuBoard(find_mcu_boards()[0]))
    except serial.serialutil.SerialException:
        ser = serial.Serial('/dev/tty.Obi-Wan-SPPDev', 38400)


ser.close()
