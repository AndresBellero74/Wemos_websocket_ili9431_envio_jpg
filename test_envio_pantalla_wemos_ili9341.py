#import cv2
import time
from PIL import Image
import pyautogui
import asyncio
import websockets
from pynput import keyboard as kb

  
async def test():
 
    async with websockets.connect('ws://192.168.0.201:8888') as websocket:
        count = 0
        while count < 5000:
            screenshot = pyautogui.screenshot()
            # Guardar imagen.
            screenshot.save("foto.png")
            #cv2.imwrite("foto.png", frame)
            im = Image.open('foto.png')
            size = 320, 240
            im.thumbnail(size)
            new_im = im.resize(size)
            #new_im.save('foto.bmp')
            new_im.save('foto.jpg',"JPEG",dpi=[300,300],quality=40)
            print("Foto tomada correctamente")
            # Write character 'A' to serial port
            archivo = open("foto.jpg", "rb")
            indice = 0
            byteArchivo = archivo.read()
            longitud = len(byteArchivo)
            archivo.close()
            veces = int(longitud / 10000 + 1)
            # reabro para poder leerlo de a bloques
            archivo = open("foto.jpg", "rb")
            vueltas = 1
            while vueltas <= veces:
                data = archivo.read(10000)
                await websocket.send(data)
                lon = len(data)
                print(lon)
                vueltas = vueltas + 1
            archivo.close()
            count += 1  
#cam = cv2.VideoCapture(0,cv2.CAP_DSHOW)

#cam.set(cv2.CAP_PROP_FRAME_WIDTH, 240)
#cam.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)

#def pulsa(key):
#    print(str(key))
#    check, frame = cam.read()

#    cv2.imshow('video', frame)

#    key = cv2.waitKey(1)
#if key == kb.KeyCode.from_char('x'):
#        exit
#    if key == kb.KeyCode.from_char('c'):
#       leido, frame = cam.read()
#        # Capturar pantalla.
asyncio.get_event_loop().run_until_complete(test())
    
        
#with kb.Listener(pulsa) as escuchador:
#    escuchador.join()
