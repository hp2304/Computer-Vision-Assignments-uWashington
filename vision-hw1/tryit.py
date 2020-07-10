from uwimg import *


# im = load_image("data/dogsmall.jpg")
# a = nn_resize(im, im.w*4, im.h*4)
# save_image(a, "dog4x-nn")



# a = bilinear_resize(im, im.w*4, im.h*4)
# save_image(a, "dog4x-bl")



# im = load_image("data/dog.jpg")
# a = nn_resize(im, im.w//7, im.h//7)
# save_image(a, "dog7th-bl")



# im = load_image("data/dog.jpg")
# f = make_box_filter(7)
# blur = convolve_image(im, f, 1)
# save_image(blur, "dog-box7")



# im = load_image("data/dog.jpg")
# f = make_box_filter(7)
# blur = convolve_image(im, f, 1)
# thumb = nn_resize(blur, blur.w//7, blur.h//7)
# save_image(thumb, "dogthumb")



# im = load_image("data/dog.jpg")
# f = make_highpass_filter()
# edge = convolve_image(im, f, 1)
# clamp_image(edge)
# save_image(edge, "dog-highpass")



# im = load_image("data/dog.jpg")
# f = make_sharpen_filter()
# sharp = convolve_image(im, f, 1)
# clamp_image(sharp)
# save_image(sharp, "dog-sharpen")



# im = load_image("data/dog.jpg")
# f = make_emboss_filter()
# emb = convolve_image(im, f, 1)
# clamp_image(emb)
# save_image(emb, "dog-emboss")



# im = load_image("data/dog.jpg")
# f = make_gaussian_filter(2)
# blur = convolve_image(im, f, 1)
# save_image(blur, "dog-gauss2")



# im = load_image("data/dog.jpg")
# f = make_gaussian_filter(2)
# lfreq = convolve_image(im, f, 1)
# hfreq = im - lfreq
# reconstruct = lfreq + hfreq
# save_image(lfreq, "low-frequency")
# save_image(hfreq, "high-frequency")
# save_image(reconstruct, "reconstruct")



# im1 = load_image("data/ron.jpg")
# f = make_gaussian_filter(3)
# lf1 = convolve_image(im1, f, 1)
# hf1 = im1 - lf1
# clamp_image(hf1)
# im2 = load_image("data/dumbledore.jpg")
# f = make_gaussian_filter(3)
# lf2 = convolve_image(im2, f, 1)
# final = hf1 + lf2
# clamp_image(final)
# save_image(final, "cool")



# im = load_image("data/dog.jpg")
# res = sobel_image(im)
# mag = res[0]
# feature_normalize(mag)
# save_image(mag, "magnitude")



im = load_image("data/dog.jpg")
f = make_gaussian_filter(3)
res = convolve_image(im, f, 1)
res = colorize_sobel(res)
clamp_image(res)
save_image(res, "sobel_colorized")
