import cv2
import numpy as np
from time import sleep
import math

cap = cv2.VideoCapture(0)

_, prev = cap.read()
prev_gray = cv2.cvtColor(prev, cv2.COLOR_BGR2GRAY)/255.
prev = prev/255.

h, w = prev_gray.shape
print(w, h)
stride = 13
while(True):
    ret, im = cap.read()
    im_gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)/255.
    im = im/255.
    sx = cv2.Sobel(im_gray, cv2.CV_64F, 1, 0, ksize=3)
    sy = cv2.Sobel(im_gray, cv2.CV_64F, 0, 1, ksize=3)
    it = prev_gray - im_gray

    Ixx = cv2.blur(sx**2,(15, 15))
    Iyy = cv2.blur(sy**2,(15, 15))
    Ixy = cv2.blur(sx*sy,(15, 15))
    Ixt = cv2.blur(sx*it,(15, 15))
    Iyt = cv2.blur(sy*it,(15, 15))
    vs = np.zeros((round(h/stride), round(w/stride), 2))

    for j in range(int((stride-1)/2), w, stride):
        for i in range(int((stride-1)/2), h, stride):
            m = np.array([[Ixx[i, j], Ixy[i, j]], [Ixy[i, j], Iyy[i, j]]])
            v = np.array([[Ixt[i, j]], [Iyt[i, j]]])
            try:
                m = np.linalg.inv(m)
            except np.linalg.LinAlgError:
                continue
            else:
                delP = m@v
            vs[int(i/stride), int(j/stride), 0] = delP[0][0]
            vs[int(i/stride), int(j/stride), 1] = delP[1][0]

    vs = 60*cv2.GaussianBlur(np.clip(vs, a_min = -6, a_max = 6) , (13, 13), 2)
    vs[vs[:, :, 0]>w] = 0
    vs[vs[:, :, 1]>h] = 0

    for j in range(int((stride-1)/2), w, stride):
        for i in range(int((stride-1)/2), h, stride):
            dx = vs[int(i/stride), int(j/stride), 0]
            dy = vs[int(i/stride), int(j/stride), 1]
            angle = 6*(math.atan2(dy, dx) / (2*math.pi) + .5);
            index = math.floor(angle);
            f = angle - index;
            if(index == 0):
                r = 1
                g = f
                b = 0
            elif(index == 1):
                r = 1-f
                g = 1
                b = 0
            elif(index == 2):
                r = 0
                g = 1
                b = f
            elif(index == 3):
                r = 0
                g = 1-f
                b = 1
            elif(index == 4):
                r = f
                g = 0
                b = 1
            else:
                r = 1
                g = 0
                b = 1-f
            d = int(math.sqrt(dx*dx + dy*dy));
            for val in range(d):
                xi = j + int((dx*val)/d);
                yi = i + int((dy*val)/d);
                if(xi < w and yi < h):
                    im[yi, xi, 0] = b
                    im[yi, xi, 1] = g
                    im[yi, xi, 2] = r
    cv2.imshow('im',im)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    prev_gray = im_gray
    prev = im

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()