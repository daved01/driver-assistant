# include<opencv2/core.hpp>
# include<opencv2/videoio.hpp>
# include<opencv2/highgui.hpp>
# include<opencv2/opencv.hpp>
# include<iostream>
# include<opencv2/imgproc.hpp>
# include<string>
# include<memory>
# include<torch/script.h>

using namespace cv;
using namespace std;


// Displays statistics in the video
Mat displayStats(Mat frame, double stats) {
    putText(frame, //target image
    "Loss: " + to_string(stats),
    Point(10, frame.rows / 2), //top-left position
    FONT_HERSHEY_DUPLEX,
    0.7,
    CV_RGB(255, 255, 255), //font color
    2);

    return frame;
}



int main()
{
    // Parameters
    bool showStats = true;
    bool showSigns = true;

    // Load symbols and make background transparent
    Mat stopsign = imread("/Users/David/Repositories/driver-assistant/stop.png", -1); 
    if (stopsign.empty() == true) {
        cout << "ERROR! Could not load sign symbols\n";
    }

    Mat mask;
    vector<Mat> rgbLayer;
    split(stopsign,rgbLayer);
    if(stopsign.channels() == 4) {
        split(stopsign, rgbLayer);         
        Mat cs[3] = { rgbLayer[0],rgbLayer[1],rgbLayer[2] };
        merge(cs,3,stopsign);
        mask = rgbLayer[3]; // png's alpha channel used as mask
        }

    Mat frame;
    VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    
    for (;;)
    {
        // Wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }


        // Add stuff to frame
        if (showStats == true) {
            frame = displayStats(frame, 0.999);
        }

        // Add symbols
        if (showSigns == true) {
            const int x = (frame.size().width - stopsign.size().width)/2;
            const int y = 20;
            stopsign.copyTo(frame(Rect(x,y,stopsign.cols, stopsign.rows)),mask);
        }

        // Show output
        imshow("Camera Demo", frame);
        if (waitKey(5) >= 0)
            break;
    }

    return 0;
}