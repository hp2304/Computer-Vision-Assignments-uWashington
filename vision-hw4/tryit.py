from uwimg import *
import time
# import cv2
# import matplotlib.pyplot as plt

im = load_image("data/canny.jpg")

save_image(cannyEdgeDetect(im, 3, 0.8, 0.5), "canny")

# canny = cannyEdgeDetect(im, 1, 0.7, 0.07)
# save_image(canny, "canny_board")
# save_image(findLines(im, canny, .25), "lines")

# canny = cannyEdgeDetect(im, 2, 0.7, 0.07)
# save_image(canny, "canny_coins")
# save_image(findCircles(im, canny, .5, 15, 80), "circles")

# findLines(im, 0.4)
# out = get_lbph_features(im, 1, 8, 8)
# save_image(out.lbp_im, "lbp")

# res = bilinear_resize(im, 300, 600)
# out = get_hog_features(res)
# print(out.n)
# for i in range(out.n):
#     print(out.data[i])

# img_path = "data/new_board.jpg"
# img = cv2.imread(img_path)
# img = cv2.resize(img, (1000, 720))
# cv2.imwrite("data/new_board.jpg", img)
# plt.imshow(img)
# plt.show()
# exit()
# img_path = "data/new_board.jpg"
# im = load_image(img_path)
# out = projective_resize(im, 5, 38, 930, 140, 25, 710, 890, 610) # new_board
# save_image(out, "proj_resized1")
# img_path = "data/new_book.jpg"
# im = load_image(img_path)
# out = projective_resize(im, 155, 108, 578, 121, 160, 375, 550, 370) # new_book
# save_image(out, "proj_resized2")
