#pragma once

#include "ofMain.h"

#include "ofxSerial.h"
#include "config.h"

class ofApp : public ofBaseApp{
    private:
        ofVec2f mouse_coords;
        bool mouse_pressed = false;
    
        float screen_width;
        ofVec2f screen_center;
        float screen_radius;
        float stop_radius;
        void recalcScreenVariables(int w, int h);
    
        ofx::IO::COBSPacketSerialDevice packetSerialDevice;
        uint8_t cmdBuffer[CMD_MESSAGE_SIZE];
    
	public:
		void setup();
		void update();
		void draw();

        bool sendData(uint8_t* buffer, int length);
        void onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args);
        void onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args);
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
