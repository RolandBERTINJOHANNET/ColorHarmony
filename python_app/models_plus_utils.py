import torch
import torch.nn as nn
import os
import sys
import PIL.Image as Image
import numpy as np

#il faut faire pip install colormath


from colormath.color_objects import LCHabColor, sRGBColor
from colormath.color_conversions import convert_color


#noter que ce sont les opérations dans colorMath qui
				#prennent 10secondes par image.

#---------------------------------------------------

def lch_to_rgb(img,wasByte):#wasByte indicates the there was a 0,360 to 0,256 mapping
  new_img = np.zeros(img.shape)
  ctr=0
  if img.shape[2]!=3:
    print("warning : image is not in :,:,3 format !")
  for i in range(img.shape[0]):
    for j in range(img.shape[1]):
      pixel = img[i,j]
      lch = LCHabColor(pixel[0],pixel[1],pixel[2]/(256./360) if wasByte else pixel[2])
      rgb = convert_color(lch,sRGBColor)
      new_img[i,j]=np.array([rgb.rgb_r,rgb.rgb_g,rgb.rgb_b])
      ctr+=1
      if ctr%10000==0:
        print("converting image to rgb, did",ctr,"pixels")
  return new_img

def rgb_to_lch(img,forModel):#forModel equivalent to wasByte
  new_img = np.zeros(img.shape)
  ctr=0
  if img.shape[2]!=3:
    print("warning : image is not in :,:,3 format !")
  for i in range(img.shape[0]):
    for j in range(img.shape[1]):
      pixel = img[i,j]
      rgb = sRGBColor(pixel[0]/255.,pixel[1]/255.,pixel[2]/255.)
      lch = convert_color(rgb,LCHabColor)
      new_img[i,j]=np.array([lch.lch_l,lch.lch_c,lch.lch_h*(256./360) if forModel else lch.lch_h])
      ctr+=1
      if ctr%10000==0:
        print("converting image to lch, did",ctr,"pixels")
  return new_img

#---------------------------------------------------

#____________________________________MODELS__________
#(defining only the generator class)

class convBlock(nn.Module):
  def __init__(self,in_channels,out_channels,use_act=True,down=True,**kwargs):
    super().__init__()
    self.down=down
    if down:
      self.block=nn.Sequential(
        nn.Conv2d(in_channels,out_channels,**kwargs),
        nn.InstanceNorm2d(out_channels),
        nn.ReLU() if use_act else nn.Identity()
    )
    else:
      self.block=nn.Sequential(
        nn.Upsample(scale_factor = 2, mode='nearest'),
        nn.ReflectionPad2d(1),
        nn.Conv2d(in_channels, out_channels,
                  kernel_size=3, stride=1, padding=0),
        nn.InstanceNorm2d(out_channels),
        nn.ReLU() if use_act else nn.Identity()
      )

  def forward(self,x):
    if self.down:
      return self.block(x)
    else:
      x=x.reshape((-1,x.shape[0],x.shape[1],x.shape[2]))
      x = self.block(x)
      x = x.reshape((x.shape[1],x.shape[2],x.shape[3]))
      return x

class residualBlock(nn.Module):
  def __init__(self,n_channels):
    super().__init__()
    self.block = nn.Sequential(
        convBlock(n_channels,n_channels,kernel_size=3,stride=1,padding=1),
        convBlock(n_channels,n_channels,False,kernel_size=3,stride=1,padding=1)
    )
  def forward(self,x):
    return x + self.block(x)

class Generator(nn.Module):
  def __init__(self,n_residuals=9,in_channels=3,num_features=64):
    super().__init__()
    self.initial = nn.Sequential(
        nn.Conv2d(in_channels,num_features,7,1,3,padding_mode="reflect"),
        nn.ReLU()
        )
    self.down = nn.ModuleList(
        [
        convBlock(num_features,num_features*2,kernel_size=3,stride=2,padding=1),
        convBlock(num_features*2,num_features*4,kernel_size=3,stride=2,padding=1)
        ]
    )
    self.residual=nn.ModuleList(
        [*[residualBlock(num_features*4) for _ in range(n_residuals)]
        ]
    )
    self.up = nn.ModuleList(
        [
        convBlock(num_features*4,num_features*2,kernel_size=3,stride=2,padding=1,output_padding=1,down=False),
        convBlock(num_features*2,num_features*1,kernel_size=3,stride=2,padding=1,output_padding=1,down=False)
        ]
    )
    self.up.append(nn.Sequential(
        nn.Conv2d(num_features,3,7,1,3,padding_mode="reflect"),
        nn.Tanh()
        )
    )

  def forward(self,x):
    x = self.initial(x)
    for d in self.down:
      x = d(x)
    for r in self.residual:
      x = r(x)
    for u in self.up:
      x = u(x)
    return x

