#ifndef RECTANGLEWATCHER
#define RECTANGLEWATCHER
#include "cv.hpp"

class RectangleWatcher {

public:
    virtual void updateRect(cv::Rect newRect) = 0;

private:
    cv::Rect rectangle;

};


#endif // RECTANGLEWATCHER

