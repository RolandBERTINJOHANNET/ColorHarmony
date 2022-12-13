from models_plus_utils import *

#image must have shape (_,_,3)
def run_model(model_name,img):
	#load the model
	the_model = Generator(n_residuals=3,num_features=64)
	the_model.load_state_dict(torch.load(model_name))
	the_model = the_model.to(torch.device("cuda:0"))

	#normalize the image
	img = img/255.
	
	#get lch of original image
	orig_lch = rgb_to_lch(img,False)

	#get the generated image
	img = torch.Tensor(np.moveaxis(img,-1,0)).to(torch.device("cuda:0"))
	img_treated = np.moveaxis(np.array(the_model(img).cpu().detach()),0,-1)


	#set its luminance to the orig luminance
	img_treated_lch = rgb_to_lch(img_treated,False)
	img_treated_lch[:,:,0]=orig_lch[:,:,0]

	#forbid increase in saturation (to limit artefacts)
	for i in range(256):
	  for j in range(256):
	    if img_treated_lch[i,j,1] - orig_lch[i,j,1] >0:
	      img_treated_lch[i,j,1] = orig_lch[i,j,1]


	#turn back to lch
	img_treated = lch_to_rgb(img_treated_lch,False)

	#perform clipping to avoid modulos
	img_treated = img_treated*255.#conversion divides by 255
	img_treated = np.clip(img_treated,0,1)

	return img_treated


#demo -- pour voir le résultat, faire "python runModel.py"
if __name__=="__main__":
	image_name = "testImage.jpg"
	model_name = "rect"
	out_file_name = "result.jpg"

	image = np.array(Image.open(image_name))
	image = run_model(model_name,image)

	print(np.min(image),np.max(image))

	#save it
	Image.fromarray((image*255.).astype(np.uint8)).save(out_file_name)
