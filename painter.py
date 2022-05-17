import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
import os


class Painter:
    def __init__(self, load_csv, load_dir=None):
        if not load_csv:
            self.data = pd.DataFrame(
                columns=['episode reward', 'episode', 'Method'])
        else:
            self.load_dir = load_dir
            if os.path.exists(self.load_dir):
                print("==Reading {}.".format(self.load_dir))
                # The first column of the csv file is index, so you don't need to take it.
                self.data = pd.read_csv(self.load_dir).iloc[:, 1:]
                print("==Reading complete")
            else:
                print("==There is no file in {}, Painter has automatically created the csv.".format(
                    self.load_dir))
                self.data = pd.DataFrame(
                    columns=['episode reward', 'episode', 'Method'])
        self.xlabel = None
        self.ylabel = None
        self.title = None
        self.hue_order = None

    def setXlabel(self, label): self.xlabel = label

    def setYlabel(self, label): self.ylabel = label

    def setTitle(self, label): self.title = label

    def setHueOrder(self, order):
        """Set to ['name1', 'name2'...] form"""
        self.hue_order = order

    def addData(self, dataSeries, method, x=None, smooth=True):
        if smooth:
            dataSeries = self.smooth(dataSeries)
        size = len(dataSeries)
        if x is not None:
            if len(x) != size:
                print("Please enter x of the same dimension!")
                return
        for i in range(size):
            if x is not None:
                dataToAppend = {
                    'episode reward': dataSeries[i], 'episode': x[i], 'Method': method}
            else:
                dataToAppend = {
                    'episode reward': dataSeries[i], 'episode': i+1, 'Method': method}
            self.data = self.data.append(dataToAppend, ignore_index=True)

    def drawFigure(self, style="darkgrid"):
        """
        style: darkgrid, whitegrid, dark, white, ticks
        """
        sns.set_theme(style=style)
        sns.set_style(rc={"linewidth": 1})
        print("==Drawing...")
        sns.relplot(data=self.data, kind="line", x="episode", y="episode reward",
                    hue="Method", hue_order=None)
        plt.title(self.title, fontsize=12)
        plt.xlabel(self.xlabel)
        plt.ylabel(self.ylabel)
        print("==Finished drawing!")
        plt.show()

    def saveData(self, save_dir):
        self.data.to_csv(save_dir)
        print("==Data has been saved to path {}!".format(save_dir))

    def addCsv(self, add_load_dir):
        """Merge another csv file into load_dir's csv file."""
        add_csv = pd.read_csv(add_load_dir).iloc[:, 1:]
        self.data = pd.concat([self.data, add_csv], axis=0, ignore_index=True)

    def deleteData(self, delete_data_name):
        """To delete the data of a method, it needs to be saved manually after deletion, it will not be saved automatically."""
        self.data = self.data[~self.data['Method'].isin([delete_data_name])]
        print("==The corresponding data under {} has been deleted!".format(
            delete_data_name))

    def smoothData(self, smooth_method_name, N):
        """Perform MA filtering on the reward under a certain method, where N is the MA filtering order."""
        begin_index = -1
        # A mode of -1 means that the initial index has not been searched, and a mode of 1 means that the end index is being searched.
        mode = -1
        for i in range(len(self.data)):
            if self.data.iloc[i]['Method'] == smooth_method_name and mode == -1:
                begin_index = i
                mode = 1
                continue
            if mode == 1 and self.data.iloc[i]['episode'] == 1:
                self.data.iloc[begin_index:i, 0] = self.smooth(
                    self.data.iloc[begin_index:i, 0], N=N
                )
                begin_index = -1
                mode = -1
                if self.data.iloc[i]['Method'] == smooth_method_name:
                    begin_index = i
                    mode = 1
            if mode == 1 and i == len(self.data) - 1:
                self.data.iloc[begin_index:, 0] = self.smooth(
                    self.data.iloc[begin_index:, 0], N=N
                )
        print("==Finished smoothing {} data {} times!".format(
            smooth_method_name, N))

    @staticmethod
    def smooth(data, N=11):
        n = (N - 1) // 2
        res = np.zeros(len(data))
        for i in range(len(data)):
            if i <= n - 1:
                res[i] = sum(data[0:2 * i+1]) / (2 * i + 1)
            elif i < len(data) - n:
                res[i] = sum(data[i - n:i + n + 1]) / (2 * n + 1)
            else:
                temp = len(data) - i
                res[i] = sum(data[-temp * 2 + 1:]) / (2 * temp - 1)
        return res
