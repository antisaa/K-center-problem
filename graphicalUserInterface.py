#!/usr/bin/env python

from Tkinter import *

import os
import time
from subprocess import call
import matplotlib.pyplot as plt
import numpy as np

class KCenterProblem(Frame):

    def __init__(self, master ):
        
        Frame.__init__(self, master)


        self.frame = Frame(master)  #Starts by creating a Frame widget. A frame is a simple container and is in this case only used to hold the other widgets
        self.frame.pack()   #After creating the widget, we immediately call the pack method to make the frame visible.
        master.minsize(width=600, height=400)
        master.maxsize(width=1000, height=650)
        master.title("K-center problem")
        #Creating button widgets, as a children to the frame
        #This time, we pass a number of options to the constructor, as keyword arguments.
        ###############################################################


        
        ###################BUTTONS#################################
        self.quit    = Button(self.frame, text="QUIT", fg="red", command=self.frame.quit, height=2, width=4)
        self.compile = Button(self.frame, text="COMPILE", fg="blue", command=self.compile, height=2, width=4)
        self.run     = Button(self.frame, text="RUN", fg="blue",command=self.run, height=2, width=4)
        self.resultt = Button(self.frame, text="RESULT", fg="blue", command=self.printResult, height=2, width=4) 
        
        ##################LABELS#####################################
        self.labelStartCompiling = Label (self.frame, text = "Compiling...", fg="blue")
        self.labelEndCompiling   = Label (self.frame, text = "Compiled!", fg="blue")
        self.labelStartRunning   = Label (self.frame, text = "Running...", fg="blue")
        self.labelEndRunning     = Label (self.frame, text = "Runed!", fg="blue")

        self.file  = Label(self.frame, text="File:")
        self.n     = Label(self.frame, text="N:")
        self.k     = Label(self.frame, text="k:")
        self.range = Label(self.frame, text="Uniform range:")
        
        ###################ENTRIES###################################
        #self.fileValue  = Entry(self.frame)
        self.nValue     = Entry(self.frame)
        self.kValue     = Entry(self.frame)
        self.rangeValue = Entry(self.frame)

        #self.fileValue.focus_set()
        #self.nValue.focus_set()
        #self.kValue.focus_set()
        #self.rangeValue.focus_set()

        #################################BOOLIANS############################################
        self.isCompiled = False
        self.isRunned = False
        self.isResulted = False
        self.isResultedBoth = False
        self.isResultedGen = False
        self.isResultedSim = False

        self.fileMod = False
        self.manMod = False

        ###############################VARIABLES#############################
        self.varSim = IntVar()
        self.varGen = IntVar()
        self.varGenSimSep = IntVar()
        self.varGenSimTo = IntVar()
        
        self.var = IntVar()

        ##############################RADIOBUTTONS##################################
        self.simCheckbutton = Radiobutton(self.frame, text="Run only simulated annealing", variable=self.var, value=0)
        self.genCheckbutton = Radiobutton(self.frame, text="Run only genetic algorithm",  variable=self.var, value=1)
        self.genSimToCheckbutton = Radiobutton(self.frame, text="Run simulated and genetic together", variable=self.var,value = 2)
        self.genSimSepCheckbutton = Radiobutton(self.frame, text="Run simulated and genetic separately", variable=self.var, value=3)
        
        self.genResult = -1
        self.simResult = -1

        self.OPTIONS = [
            "pmed1.txt",
            "pmed2.txt",
            "pmed3.txt",
            "pmed4.txt",
            "pmed5.txt",
            "pmed6.txt",
            "pmed7.txt",
            "pmed8.txt",
            "pmed9.txt",
            "pmed10.txt",
            "pmed11.txt",
            "pmed12.txt",
            "pmed13.txt",
            "pmed14.txt",
            "pmed15.txt",
            "pmed16.txt",
            "pmed17.txt",
            "pmed18.txt",
            "pmed19.txt",
            "pmed20.txt",
            "pmed21.txt",
            "pmed22.txt",
            "pmed23.txt",
            "pmed24.txt",
            "pmed25.txt",
            "pmed26.txt",
            "pmed27.txt",
            "pmed28.txt",
            "pmed29.txt",
            "pmed30.txt",
            "pmed31.txt",
            "pmed32.txt",
            "pmed33.txt",
            "pmed34.txt",
            "pmed35.txt",
            "pmed36.txt",
            "pmed37.txt",
            "pmed38.txt",
            "pmed39.txt",
            "pmed40.txt",

        ]           
        self.variable = StringVar(self.frame)
        self.variable.set(self.OPTIONS[0]) # default value

        self.w = apply(OptionMenu, (self.frame, self.variable) + tuple(self.OPTIONS))
        
        self.initialUI()

    def initialUI(self):

        self.question = Label (self.frame, text = "How do you want to input data?", fg="black")
        self.question.grid(row=0,column=0,columnspan=2)

        self.inputVar = IntVar()
        self.inputFile = Radiobutton(self.frame, text="From file", variable=self.inputVar, value=1, command=self.showFile)
        self.inputFile.grid(row=0, column=2, columnspan=1)

        self.inputMan = Radiobutton(self.frame, text="Manually", variable=self.inputVar, value=2, command=self.showMan)
        self.inputMan.grid(row=0, column=3, columnspan=1)

        #self.positioning = Label(self.frame, text="                                       ",fg="black")
        #self.positioning.grid(row=0,column=4,columnspan=10)


    def showFile(self):

        self.fileMod = True

        if self.manMod:
            #print "Destructor za man"
            self.n.grid_forget()
            self.k.grid_forget()
            self.range.grid_forget()
            self.nValue.grid_forget()
            self.kValue.grid_forget()
            self.rangeValue.grid_forget()

            self.compile.grid_forget()
            self.run.grid_forget()
            self.resultt.grid_forget()
            self.quit.grid_forget()

            self.simCheckbutton.grid_forget()
            self.genCheckbutton.grid_forget()
            self.genSimSepCheckbutton.grid_forget()
            self.genSimToCheckbutton.grid_forget()

            self.labelStartCompiling.grid_forget()
            self.labelEndCompiling.grid_forget()
            self.labelStartRunning.grid_forget()
            self.labelEndRunning.grid_forget()
            self.modMan = False

        self.file.grid(row=1,column=0)
        self.k.grid(row=2,column=0)

        self.w.grid(row=1, column=1, columnspan=3)
        #self.fileValue.grid(row=1, column=1, columnspan=3)
        self.kValue.grid(row=2, column=1, columnspan=3)
        
        self.compile.grid(row=5, column=1) 
        self.run.grid(row=5, column=2)
        self.resultt.grid(row=5, column=3)
        self.quit.grid(row=5, column=6)

        self.simCheckbutton.grid(row=4, column=0, columnspan=1)
        self.genCheckbutton.grid(row=3, column=0, columnspan=1)
        self.genSimSepCheckbutton.grid(row=3, column=1, columnspan=3)
        self.genSimToCheckbutton.grid(row=4, column=1, columnspan=3)

        

    def showMan(self):
        
        self.manMod = True
          
        if self.fileMod:
            #print "Destructor za file"
            self.file.grid_forget()
            self.k.grid_forget()
            #self.fileValue.grid_forget()
            self.kValue.grid_forget()

            self.compile.grid_forget() 
            self.run.grid_forget()
            self.resultt.grid_forget()
            self.quit.grid_forget()

            self.simCheckbutton.grid_forget()
            self.genCheckbutton.grid_forget()
            self.genSimSepCheckbutton.grid_forget()
            self.genSimToCheckbutton.grid_forget()

            self.labelStartCompiling.grid_forget()
            self.labelEndCompiling.grid_forget()
            self.labelStartRunning.grid_forget()
            self.labelEndRunning.grid_forget()

            self.w.grid_forget()

            self.fileMod = False
           
        self.n.grid(row=1,column=0)
        self.k.grid(row=2,column=0)
        self.range.grid(row=3,column=0)


        self.nValue.grid(row=1, column=1, columnspan=3)
        self.kValue.grid(row=2, column=1, columnspan=3)
        self.rangeValue.grid(row=3, column=1, columnspan=3)

        
        self.compile.grid(row=6, column=1) 
        self.run.grid(row=6, column=2)
        self.resultt.grid(row=6, column=3)
        self.quit.grid(row=6, column=6)

        self.simCheckbutton.grid(row=5, column=0, columnspan=1)
        self.genCheckbutton.grid(row=4, column=0, columnspan=1)
        self.genSimSepCheckbutton.grid(row=4, column=1, columnspan=3)
        self.genSimToCheckbutton.grid(row=5, column=1, columnspan=3)
    
    def getGraph(self):

        label =  ["Genetski","Simulirano"]
        no_alg =[self.genResult,self.simResult]
        index = np.arange(len(label))

        plt.bar(index, no_alg)
        plt.xlabel('Algoritam', fontsize=20)
        plt.ylabel('Rezultat', fontsize=20)
        plt.xticks(index, label, fontsize=20)
        plt.title('Stupcasti dijagram')
        plt.show()

    def compile(self):
        
        if(not self.isCompiled):

            if(self.isRunned):
                self.labelEndRunning.grid_forget()
                self.timeRun.grid_forget()
        
            if(self.isResulted):

                if(self.isResultedBoth):
                    self.labelSimulirano.grid_forget()
                    self.labelGenetski.grid_forget()

                elif(self.isResultedSim):
                    self.labelSimulirano.grid_forget()

                elif(self.isResultedGen):
                    self.labelGenetski.grid_forget()

            self.isCompiled = True

            self.labelStartCompiling.grid(row=7, column=1, columnspan=2)
            #call(["g++", "main1.cpp", "-o", "prog"])
            call(["g++", "-std=c++11", "main.cpp", "graph.cpp", "simulated_annealing.cpp", "geneticAlgorithm.cpp", "-o", "prog"])
            #commands.getoutput(["g++", "main.cpp", "-o", "prog"])
            self.labelStartCompiling.grid_forget()
            self.labelEndCompiling.grid(row=7, column=1, columnspan=2)

    def run(self):
        
        self.isRunned = True
        if(self.isCompiled):
            self.labelEndCompiling.grid_forget()

        if(self.isResulted):

            if(self.isResultedBoth):
                self.labelSimulirano.grid_forget()
                self.labelGenetski.grid_forget()

            elif(self.isResultedSim):
                self.labelSimulirano.grid_forget()

            elif(self.isResultedGen):
                self.labelGenetski.grid_forget()


        self.labelStartRunning.grid(row=7, column=1, columnspan=2)
             

        start_time= time.time()

        if(self.inputVar.get()==1):
            print (self.var.get() ,self.kValue.get(),"/tests/"+self.variable.get())
            call(["./prog",str(self.var.get()),self.kValue.get(),"tests/"+self.variable.get()])
        elif(self.inputVar.get()==2):
            print (self.var.get(),self.nValue.get(),self.kValue.get(),self.rangeValue.get())
            call(["./prog",str(self.var.get()),self.nValue.get(),self.kValue.get(),self.rangeValue.get()])

        #commands.getoutput(["./prog"])
        self.timeRun = Label(self.frame, text = "Time of executing: " + str(time.time() - start_time), fg="blue")
        
        self.labelStartRunning.grid_forget()        
        self.labelEndRunning.grid(row=7, column=1, columnspan=2)
        self.timeRun.grid(row=8, column=0, columnspan=4)
       


    def printResult(self):
        # if you want the button to disappear:
        # button.destroy() or button.pack_forget()

        self.isResulted = True
        filee = open("resultFile.txt","r")

        #if self.modeValue == 0:
        if self.var.get()==0:

            self.isResultedSim = True
            a = filee.readline()
            self.labelSimulirano = Label(self.frame, text = "Rezultat simuliranog kaljenja je: " + str(a),fg="blue")
            self.labelSimulirano.grid(row=9, column=0, columnspan=4)

        elif self.var.get()==1:

            self.isResultedGen = True
            a = filee.readline()
            self.labelGenetski = Label(self.frame, text = "Rezultat genetskog algoritma je: " + str(a),fg="blue")
            self.labelGenetski.grid(row=10, column=0,  columnspan=4)

        elif self.var.get()==2:

            self.isResultedBoth = True
            a = filee.readline()
            b = filee.readline()
            self.simResult = a
            self.genResult = b
            self.labelSimulirano = Label(self.frame, text = "Rezultat simuliranog kaljenja je: " + str(a),fg="blue")
            self.labelGenetski = Label(self.frame, text = "Rezultat genetskog algoritma je: " + str(b),fg="blue")
            self.labelSimulirano.grid(row=9, column=0, columnspan=4)
            self.labelGenetski.grid(row=10, column=0,  columnspan=4)

            self.getGraph()

        elif self.var.get()==3:

            self.isResultedBoth = True
            a = filee.readline()
            b = filee.readline()
            self.simResult = a
            self.genResult = b
            self.labelSimulirano = Label(self.frame, text = "Rezultat simuliranog kaljenja je: " + str(a),fg="blue")
            self.labelGenetski = Label(self.frame, text = "Rezultat genetskog algoritma je: " + str(b),fg="blue")
            self.labelSimulirano.grid(row=9, column=0, columnspan=4)
            self.labelGenetski.grid(row=10, column=0,  columnspan=4)

            self.getGraph()
        
        filee.close()

def main(args):

    root = Tk() #Creating Tk root widget

    app = KCenterProblem(root) #Creating instance of the App class using root widget as its parent

    #The mainloop call enters the Tk event loop, in which the application will stay until the quit method is called (just click the QUIT button), or the window is closed.
    root.mainloop() 

    #The destroy call is only required if you run this example under certain development environments; it explicitly destroys the main window when the event loop is terminated
    root.destroy() # optional; see description below
    
    

if __name__ == "__main__":
    main(sys.argv)
    


        
        

