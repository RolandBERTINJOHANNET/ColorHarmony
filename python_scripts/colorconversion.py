# -*- coding: utf-8 -*-
"""
Created on Mon Oct 31 15:32:53 2022

@author: Orlando
"""
import math


def gamma_expand(value):
	return (value/12.92) if value<=0.04045 else pow((value+0.055)/1.055, 2.4)

def gamma_compress(value):
	return (value*12.92) if value<=0.0031308 else pow(value*1.055, 1./2.4)-0.055

def expand_to_byte(value):
	oct = 255*value
	oct = 255 if oct>255 else oct
	oct = 0 if oct<0 else oct
	return oct
	

def LABfun(value):
	delta = 6./29.

	t = value;

	return pow(t, 1./3.) if t>pow(delta, 3) else (t/(3*pow(delta,2)))+(4./29.)


def LABfun_inv(value):
	delta = 6./29.

	t = value

	return pow(t, 3) if t>delta else 3.*pow(delta,2)*(t-(4./29.))





def RGB_to_LAB(r, g, b):
	#first convert rgb to XYZ
	#bring to 0,1 range
	r = float(r)/255.
	g = float(g)/255.
	b = float(b)/255.
	
	#gamma expansion
	r = gamma_expand(r)
	g = gamma_expand(g)
	b = gamma_expand(b)

	#matrix multiplication
	X = 0.4124564*(r) +0.3575761*(g) +0.1804375*(b)
	Y = 0.2126729*(r)  +0.7151522*(g) +0.0721750*(b)
	Z = 0.0193339*(r) +0.1191920*(g) +0.9503041*(b)

	#then convert to LAB
	r = 116.*LABfun(Y/100.) - 16.
	g = 500.*(LABfun(X/95.0489) - LABfun(Y/100.))
	b = 200.*(LABfun(Y/100.) - LABfun(Z/108.8840))
	
	return r,g,b

def LAB_to_LCH(L, a, b):
	#convert to LCH

	var_H = (math.pi+math.atan2(b,a))*(360./(2.*math.pi))  #Quadrant by signs, turned into degrees


	a = math.sqrt((a)*(a)+(b)*(b))
	b = var_H
	
	return L,a,b

