#pragma once
#include <string>


class MotionDetector
{
#pragma once

    public:
    MotionDetector(const std::string& streamURL, int sensitivity);
    ~MotionDetector();

    void Start();
    void Stop();

private:
    std::string streamURL;
    int sensitivity;
    bool isDetecting;

};

