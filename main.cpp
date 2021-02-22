#include<opencv2/core.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<opencv2/imgproc.hpp>
#include<string>

#include<memory>
#include<torch/script.h>
//#include <stdio.h>

using namespace cv;
using namespace std;


// Displays statistics in the video
Mat displayStats(Mat frame, int counter) {
    putText(frame, //target image
    "Loss: " + to_string(counter),
    Point(10, frame.rows / 2), //top-left position
    FONT_HERSHEY_DUPLEX,
    0.8,
    CV_RGB(255, 255, 255), //font color
    2);

    return frame;
}




int main()
{
    // Parameters
    bool showStats = true;

    // Load symbols
    Mat stopsign = imread("/Users/David/Repositories/driver-assistant/stop.png", -1);
    //src1 = imread( samples::findFile("LinuxLogo.jpg") );
    assert(stopsign.empty() == false);

    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    cap.open(0);
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    
    int count = 0;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }


        // Add stuff to frame
        if (showStats==true) {
            frame = displayStats(frame, count);
        }

        cout << frame.size().width << " " << frame.size().height << endl;
        const int x = (frame.size().width - stopsign.size().width)/2;
        const int y = 20;
        Mat destRoi;
        try {
            destRoi = frame(Rect(x, y, stopsign.cols, stopsign.rows));
        }
        catch (...) {
            cout << "Roi is out of bounds!" << endl;
            return -1;
        }
        stopsign.copyTo(destRoi);
        
        

        // Trying to make the image background transparent 
        Mat mask;
        vector<Mat> rgbLayer;
        split(stopsign,rgbLayer);
        cout << stopsign.channels() << endl;
        if(stopsign.channels() == 4)
        {
            split(stopsign, rgbLayer);         // seperate channels
            Mat cs[3] = { rgbLayer[0],rgbLayer[1],rgbLayer[2] };
            merge(cs,3,stopsign);  // glue together again
            mask = rgbLayer[3];       // png's alpha channel used as mask
        }
        //stopsign.copyTo(frame(Rect(0,0,stopsign.cols, stopsign.rows)),mask);
        


        // show live and wait for a key with timeout long enough to show images
        imshow("Camera Demo", frame);
        if (waitKey(5) >= 0)
            break;
        count++;
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}