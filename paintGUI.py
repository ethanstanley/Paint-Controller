from random import random
from kivy.app import App
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.clock import Clock
from kivy.graphics import Color, Ellipse, Line
import serial

class MyPaintWidget(Widget):
    def on_touch_down(self, touch):
        color = (random(), 1, 1)
        with self.canvas:
            Color(*color, mode='hsv')
            d = 30.
            self.painter = MyPaintWidget()

def colorGen(stringC):
    if stringC == "red":
        return (1., 0., 0.)
    elif stringC == "orange":
        return (1., .5, 0.)
    elif stringC == "yellow":
        return (1., 1., 0.)
    elif stringC == "green":
        return (0., 1., 0.)
    elif stringC == "cyan":
        return (0., 1., 1.)
    elif stringC == "blue":
        return (0., 0., 1.)
    elif stringC == "purple":
        return (1., 0., 1.)
    else:
        return (1., 1., 1.) 

class MyPaintApp(App):
    upper_bound = 1200
    right_bound = 1600
    most_recent_x = 800
    most_recent_y = 600
    def my_callback(self, dt):
      with serial.Serial('/dev/tty.usbmodemSDA4150DE621',115200) as ser:
          readIn =ser.readline().decode("utf-8")
          next = readIn[:readIn.index("\r")]
          xY = next.split(", ")
          x1 = int(xY[0])
          y2 = int(xY[1])
          col = str(xY[2])
          drawActive = int(xY[3])
          clear = int(xY[4])
          if clear == 1:
            self.clear_canvas()
          x_n, y_n = self.draw_new(int(x1), int(y2), col, drawActive)
          self.most_recent_x = x_n
          self.most_recent_y = y_n
          #print(x_n, ", ", y_n, ", ", col)
          #print('My callback is called', dt)

    def build(self):
        parent = Widget()
        self.painter = MyPaintWidget()
        parent.add_widget(self.painter)
        Clock.schedule_interval(self.my_callback, .001)
        return parent

    def clear_canvas(self):
        self.most_recent_x = 800
        self.most_recent_y = 600
        self.painter.canvas.clear()
    
    def draw_new(self, x, y, col, enabled):
        color = colorGen(col)
        with self.painter.canvas:
          Color(*color)
          d = 8.
          if enabled == 1:
            joe = Line(points=(self.most_recent_x, self.most_recent_y), width = 3)
          else:
            Color(*(0., 0., 0.))
            Ellipse(pos=(self.most_recent_x, self.most_recent_y), size=(d, d))
            Color(*color)
          x_n, y_n = self.next_spot(self.most_recent_x, self.most_recent_y, x, y)
          if enabled == 1:
            joe.points += [x_n, y_n]
          else:
            Ellipse(pos=(x_n, y_n), size=(d, d))

        return x_n, y_n
 

    def next_spot(self, x_real, y_real, x2_pre, y2_pre):
        vm = 20
        xf = x2_pre - 196
        yf = y2_pre - 196
        vx = vy = 0
        if(xf > -3 and xf < 3):
          vx = 0
        elif xf <= -3:
          vx = (vm/191) * xf
        else:
          vx = (vm/64) * xf
        if (yf > -3 and yf < 3):
          vy = 0
        elif yf <= -3: 
          vy = (vm/196) * yf
        else:
          vy = (vm/59) * yf
        x_new_adj = x_real + int(vx)
        y_new_adj = y_real + int(vy)

        #bound it
        if x_new_adj > self.right_bound:
          x_new_adj = self.right_bound
        if x_new_adj < 0:
          x_new_adj = 0
        if y_new_adj > self.upper_bound:
          y_new_adj = self.upper_bound
        if y_new_adj < 0:
          y_new_adj = 0 

        return x_new_adj, y_new_adj

if __name__ == '__main__':
    app = MyPaintApp()
    app.run()








