import cv2
import sys
import time

#cascPath = haarcascade_frontalface_default.xml
cascPath = sys.argv[1]
faceCascade = cv2.CascadeClassifier(cascPath)

#videoPath = sys.argv[2]
#video_capture = cv2.VideoCapture(videoPath)
video_capture = cv2.VideoCapture(0)
ratio = video_capture.get(cv2.CAP_PROP_FRAME_WIDTH) / video_capture.get(cv2.CAP_PROP_FRAME_HEIGHT)
WIDTH = 600
HEIGHT = int(WIDTH / ratio)

while True:
    # Capture frame-by-frame
    ret, frame = video_capture.read()
    frame = cv2.resize(frame, (WIDTH, HEIGHT))
    frame = cv2.flip(frame, 1)
    begin_time = time.time()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(gray,scaleFactor=1.1,minNeighbors=5,minSize=(30, 30),flags=cv2.CASCADE_SCALE_IMAGE)

    # Draw a rectangle around the faces
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    print('diff time = {:5.2f}'.format(time.time()-begin_time))
    #cv2.namedWindow('Video',cv2.WINDOW_NORMAL)
    # Display the resulting frame
    cv2.imshow('Video', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything is done, release the capture
video_capture.release()
cv2.destroyAllWindows()
