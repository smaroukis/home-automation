# Editing: 

import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import csv
import os

class Plotter:

    def __init__(self, data_file, figure_path = None, frames = 100, interval = 1000):
        """ 
        REQUIRES: data_file path relative
        """
        __location__ = os.path.dirname(__file__)
        self.data_file = os.path.join(__location__, data_file)
        if not os.path.exists(self.data_file):
            raise FileNotFoundError(f"The file '{self.data_file}' does not exist")

        self.x = []
        self.y = []
        self.figure_path = figure_path
        self.frames = frames
        self.interval = interval

        self.fig, self.ax = plt.subplots()
        self.line, = self.ax.plot([], [], lw=2)
        # self.ax.set_xlim(0, 2*np.pi) 
        self.ax.set_ylim(-1, 1) # to avoid divide by zero when setting axis limits
        self.ani = FuncAnimation(self.fig, self.update, frames = self.frames, interval = self.interval) 

        # Setup callback to save figure on user closing window
        self.fig.canvas.mpl_connect('close_event', self.on_close)

    def __del__(self):
        # self.stop() # to disconnect callback and save figure
        pass

    def stop(self):
        # TODO - issues with event_source being None
        # self.ani.event_source.stop() # need to disconnect the update callback to stop the animation
        # self.ani.disconnect(self.update) # disconnects the update callback
        try: 
            self.ani._stop() # stops the animation
            print("(plotter.stop): disconnecting animation callback")
        except AttributeError:
            print("(plotter.stop): AttributeError, animation callback not connected")
        self.save()
        plt.close(self.fig)

    def on_close(self, event):
        """ Callback for when user closes window """
        # Note the mpl handler passes the event as an arg
        print("(plotter.on_close): user close event, calling self.stop()")
        self.save()
        self.stop()

    def show(self):
        # plt.show(self.fig)
        pass
        
    def save(self):
        """ Saves figure to the path stored in self.figure_path """
        if self.figure_path:
            self.fig.savefig("{}".format(self.figure_path))
            print(f"(plotter.save): saved figure to {self.figure_path}")

    def store_datum(self, x, y):
        """ Store data in a list
        Separates the ingestion of data from the plotting of data"""
        self.x.append(x)
        self.y.append(y)
        print("(Plotter.store_datum): appended x='{}'".format(x))
        print("(Plotter.store_datum): appended y='{}'".format(y))

    def read_data(self):
        try: 
            with open(self.data_file, 'r') as csv_file:
                csv_reader = csv.reader(csv_file)
                self.x.clear()
                self.y.clear()
                for row in csv_reader:
                    x = float(row[0])
                    y = float(row[1])
                    self.x.append(x)
                    self.y.append(y)
                    # print(f"(read_data): added x={x}, y={y} to plotter")
        except Exception as e:
            print(f"An error occurred: {e}")

    def update(self, frame):
        """ Update function that is called by FuncAnimation
        Requires: frame/i passed by FuncAnimation
        See store_datum(x,y) for adding data to members"""
        print(f"(plotter.update): call to plotter.update (frame {frame})")

        self.read_data()

        # Debugging: Print data and limits at each frame
        print(f"x: {self.x}")
        print(f"y: {self.y}")
        print(f"X-axis limits: {self.ax.get_xlim()}")
        print(f"Y-axis limits: {self.ax.get_ylim()}")
        
        # Update Plot
        self.ax.set_xlim([min(self.x), max(self.x)])
        self.ax.set_ylim([min(self.y), max(self.y)])

        self.line.set_data(self.x, self.y)
        # self.fig.canvas.flush_events()
        return self.line, 


if __name__ == "__main__":
    # Expected: to plot all 10 points seemingly at once and then keep plotting

    # Enable fake data in update() for testing

    datafile = "logs/test.csv"
    savefig = "logs/test_fig.png"
    try:
        plot = Plotter(datafile, savefig, interval=1000)
    except FileNotFoundError as e:
        print(e)
        exit(1)

    # logging.getLogger('matplotlib.font_manager').disabled = True # to silence matplotlib.font_manager debug output
    # logging.info("Created plotter → {}".format(plot_saveas))
    # plt.ion()
    # fig = plt.gcf()
    # ani = plot.ani

    try: 
        while True:
            plt.pause(0.1)  # Add a small pause to allow animation updates
    except KeyboardInterrupt:
        plot.stop()
        plt.ioff()
        
        print("Exiting")
        exit(0)
    