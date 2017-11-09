Images are read from a source dirctory and sent to Microsoft for analysis.  The result is written as a protein to a file or named pipe

Reading images from the webcam using gstreamer:
gst-launch-1.0 wrappercamerabinsrc mode=1  ! "video/x-raw,format=GRAY8,width=640,height=480,framerate=5/1" ! queue ! jpegenc ! multifilesink location="frames/image%d.jpeg"


On osx.  Make a named pipe "mkfifo /tmp/proteinPipe"
Use greenhouse's poke command to read from the pipe and write to a pool "poke -l t tcp://eixample.local/hackathon < proteinPipe"

Start the Emotion main class giving the images directory and the named pipe as args.
