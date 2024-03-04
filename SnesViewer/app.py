from tkinter import *
import tkinter as tk
root = Tk()
import serial.tools.list_ports
import threading
from PIL import Image, ImageTk
picoSer = serial.Serial
picoSerISopen = False
onExit = False
stop_scan = threading.Event()
stop_serial = threading.Event()

#windowe
root.title("SneS Controller")
root.geometry("600x295")
root.resizable(False, False)
#bg image
canvas = Canvas(root, bg="#211a21")
canvas.pack(side=TOP, fill=BOTH, expand=True)
img = ImageTk.PhotoImage(Image.open('assets/snes.png'))
canvas.create_image(0, 0, anchor="nw", image=img)

#serial connection
def connect(port):
    global picoSer
    global picoSerISopen
    global buttonsClick
    try:
        picoSer = serial.Serial(port=port, baudrate=9600)
        picoSerISopen = True
        buttonsClick = threading.Thread(target=showButtons, args=(picoSer,stop_scan,))
        buttonsClick.start()
    except serial.SerialException:
        nuhuh = Toplevel(root)
        nuhuh.geometry("300x326")
        nuhuh.title("Port Busy or Broken")
        nuhuh.resizable(False, False)
        nuhuhanvas = Canvas(nuhuh, width=300, height=326, bg="#211a21")
        nuhuhanvas.pack()
        nuhuhimg = PhotoImage(file='assets/SerialError.png')
        nuhuhanvas.create_image(150, 163, image=nuhuhimg)
        nuhuh.mainloop()


#Drop menu
menubar = Menu(root)
root.config(menu=menubar)
#Port selection
port_menu = Menu(menubar)

#find ports
def reload_ports(end):
    portsOLD = []
    global onExit
    while True:
        ports = serial.tools.list_ports.comports()
        if ports != portsOLD:
            if portsOLD != []:
                port_menu.delete(0, len(portsOLD) -1)
            portsOLD = ports
            for port, desc, hwid in sorted(ports):
                port_menu.add_radiobutton(
                    label="{}: {}".format(port, desc),
                    command= lambda port = port: connect(port),
                )
        if end.is_set():
            onExit = True
            break
port_finder = threading.Thread(target=reload_ports, args=(stop_serial,))
port_finder.start()

menubar.add_cascade(
    label="Port",
    menu=port_menu,
)

def showButtons(picoSer:serial.Serial, end):
    def _create_circle(self, x, y, r, **kwargs):
        return self.create_oval(x-r, y-r, x+r, y+r, **kwargs)
    tk.Canvas.create_circle = _create_circle

    nintendoColor = "#E4000F"
    canvas.create_polygon([121, 127, 143, 127, 132, 107], fill=nintendoColor, tags="UP", state="hidden")
    canvas.create_polygon([121, 171, 143, 171, 132, 191], fill=nintendoColor, tags="DOWN", state="hidden")
    canvas.create_polygon([110, 138, 110, 160, 90, 149], fill=nintendoColor, tags="LEFT", state="hidden")
    canvas.create_polygon([154, 138, 154, 160, 174, 149], fill=nintendoColor, tags="RIGHT", state="hidden")

    canvas.create_polygon([195, 19, 116, 19, 110, 20, 104, 21, 100, 22, 96, 23, 93, 24, 90, 25, 87, 26, 84, 27, 82, 28, 79, 29, 77, 30, 75, 31, 73,
                            32, 72, 33, 71, 34, 71, 45, 72, 44, 73, 43, 75, 42, 77, 41, 79, 40, 81, 39, 83, 38, 85, 37, 88, 36, 91, 35, 94, 34, 97,
                            33, 100, 32, 103, 31, 107, 30, 112, 29, 119, 28, 131, 27, 197, 27, 197, 19], fill=nintendoColor, tags="LB", state="hidden")
    canvas.create_polygon([403, 19, 482, 19, 489, 20, 495, 21, 499, 22, 503, 23, 506, 24, 509, 25, 512, 26, 515, 27, 517, 28, 519, 29, 521, 30, 523,
                            31, 525, 32, 528, 34, 529, 35, 529, 45, 525, 42, 523, 41, 521, 40, 519, 39, 517, 38, 515, 37, 512, 36, 509, 35, 506, 34,
                              503, 33, 499, 32, 495, 31, 489, 30, 482, 29, 468, 27, 402, 28, 402, 20], fill=nintendoColor, tags="RB", state="hidden")
    canvas.create_polygon([259, 152, 232, 173, 231, 175, 231, 178, 233, 182, 237, 184, 240, 184, 242, 184, 269, 163, 270, 160, 270, 157, 268, 153, 264, 151, 261, 151], fill=nintendoColor, tags="SELECT", state="hidden")
    canvas.create_polygon([320, 152, 293, 173, 292, 175, 292, 178, 294, 182, 298, 184, 301, 184, 303, 184, 330, 163, 331, 160, 331, 157, 329, 153, 325, 151, 322, 151], fill=nintendoColor, tags="START", state="hidden")
    canvas.create_circle(410, 147, 21, fill=nintendoColor, tags="Y", state="hidden", outline=nintendoColor)
    canvas.create_circle(467, 192.5, 21, fill=nintendoColor, tags="B", state="hidden", outline=nintendoColor)
    canvas.create_circle(465, 105.5, 21, fill=nintendoColor, tags="X", state="hidden", outline=nintendoColor)
    canvas.create_circle(521.5, 150.5, 21, fill=nintendoColor, tags="A", state="hidden", outline=nintendoColor)
    while True:
        if end.is_set():
            break
        if picoSer.in_waiting != 0:
            btnData = picoSer.readline().decode().replace("\n", "")
            if btnData[0] == "0":
                canvas.itemconfigure("B", state="normal")
            else:
                canvas.itemconfigure("B", state="hidden")
            if btnData[1] == "0":
                canvas.itemconfigure("Y", state="normal")
            else:
                canvas.itemconfigure("Y", state="hidden")
            if btnData[2] == "0":
                canvas.itemconfigure("SELECT", state="normal")
            else:
                canvas.itemconfigure("SELECT", state="hidden")
            if btnData[3] == "0":
                canvas.itemconfigure("START", state="normal")
            else:
                canvas.itemconfigure("START", state="hidden")
            if btnData[4] == "0":
                canvas.itemconfigure("UP", state="normal")
            else:
                canvas.itemconfigure("UP", state="hidden")
            if btnData[5] == "0":
                canvas.itemconfigure("DOWN", state="normal")
            else:
                canvas.itemconfigure("DOWN", state="hidden")
            if btnData[6] == "0":
                canvas.itemconfigure("LEFT", state="normal")
            else:
                canvas.itemconfigure("LEFT", state="hidden")
            if btnData[7] == "0":
                canvas.itemconfigure("RIGHT", state="normal")
            else:
                canvas.itemconfigure("RIGHT", state="hidden")
            if btnData[8] == "0":
                canvas.itemconfigure("A", state="normal")
            else:
                canvas.itemconfigure("A", state="hidden")
            if btnData[9] == "0":
                canvas.itemconfigure("X", state="normal")
            else:
                canvas.itemconfigure("X", state="hidden")
            if btnData[10] == "0":
                canvas.itemconfigure("LB", state="normal")
            else:
                canvas.itemconfigure("LB", state="hidden")
            if btnData[11] == "0":
                canvas.itemconfigure("RB", state="normal")
            else:
                canvas.itemconfigure("RB", state="hidden")
    picoSer.close()

root.mainloop()
stop_scan.set()
stop_serial.set()