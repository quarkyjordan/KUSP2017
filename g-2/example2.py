#!/usr/bin/python        ###need this line pinned to the top                                                                                           
from ROOT import *

gSystem.Load("RooJordan2_cxx.so")

t = RooRealVar("t", "Time/[s]", 0., 0.3e-6)
wa = RooRealVar("wa", "wa", 2.975e+8)
A = RooRealVar("A", "A",0.5)
phi = RooRealVar("phi", "phi", 0.1)
N = RooRealVar("N", "N", 100.e+8)
tau = RooRealVar("tau", "tau", 2.2e-6)

fwa = RooRealVar("fwa", "fwa",2.975e+8, 0., 10.e+8)
fA = RooRealVar("fA","fA",0.5, 0., 10.0)
fphi = RooRealVar("fphi","fphi",0.1,0.,10.0)
fN = RooRealVar("fN","fphi",100.e+8,0.,200.e+8)
ftau = RooRealVar("ftau","ftau",2.2e-6,0.,10.e-6)

pdf = RooJordan2("pdf","pdf",t, wa, A, tau, phi, N)
fpdf = RooJordan2("fpdf","fpdf",t,fwa,fA,ftau,fphi,fN)
data = pdf.generate(RooArgSet(t), 1000000)

tframe = t.frame()

data.plotOn(tframe)
fpdf.plotOn(tframe)
tframe.Draw()


raw_input("please press enter")
