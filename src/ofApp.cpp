#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    recalcScreenVariables(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_WIDTH);
    
    mouse_coords.set(0.0f, 0.0f);
    ofSetLineWidth(3.0f);
    
    ofLog(OF_LOG_NOTICE, "Setting up communication Arduino");
    
    // Get a lit of connected serial devices
    // https://github.com/bakercp/ofxSerial/blob/8086059d4de58620b7bc45d67a7388b32a7c4627/example_packet_serial/src/ofApp.cpp
    std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();
    ofLogNotice("ofApp::setup") << "Connected Devices: ";
    for (std::size_t i = 0; i < devicesInfo.size(); ++i)
    {
        ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
    }
    
    if (!devicesInfo.empty())
    {
        // Connect to the first matching device.
        bool success = packetSerialDevice.setup(devicesInfo[0], BAUDRATE);
        
        if(success){
            packetSerialDevice.registerAllEvents(this);
            ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[0];
        }else{
            ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[0];
        }
    }else{
        ofLogNotice("ofApp::setup") << "No devices connected.";
    }
}

// Send data to this arduino
bool ofApp::sendData(uint8_t* buffer, int length){
    if (packetSerialDevice.isOpen()){
        if (length>0 && buffer != NULL){
            ofx::IO::ByteBuffer outBuffer(buffer, (size_t)length);
            try{
                if (packetSerialDevice.isClearToSend()){
                    packetSerialDevice.send(outBuffer);
                    return true;
                }else{
                    ofLog(OF_LOG_WARNING, "\tWarning: Packet buffer not clear to send.");
                    return false;
                }
            }catch(const std::exception& exc){
                ofLogError("\tError sending packet buffer: ") << exc.what();
                return false;
            }
        }else{
            ofLogWarning("\tWarning: Sending no data.");
            return false;
        }
    }else{
        ofLogWarning("\tWarning: Serial port closed!");
        return false;
    }
}

// Decoded serial packets will show up here.
void ofApp::onSerialBuffer(const ofx::IO::SerialBufferEventArgs& args){
    // ofLog(OF_LOG_NOTICE, "Received packet: " + args.getBuffer().toString());
    
    ofx::IO::ByteBuffer buffer = args.getBuffer();
    string debugMessageString = "DEBUG: " +string((char*)(&((buffer.getPtr())[1])), (buffer.size())-1);
    ofLog(OF_LOG_NOTICE,debugMessageString);
}

void ofApp::onSerialError(const ofx::IO::SerialBufferErrorEventArgs& args){
    // Errors and their corresponding buffer (if any) will show up here.
    ofLog(OF_LOG_ERROR, "Serial Error: " + args.getBuffer().toString() + "\t" + args.getException().displayText());
}


//--------------------------------------------------------------
void ofApp::update(){
    if (mouse_pressed){
        ofVec2f drive_vector = mouse_coords - screen_center;
        float driveVectorLength = drive_vector.length();
        DriveSpeed drive_speed = 0;
        if (driveVectorLength > stop_radius){
            if (driveVectorLength < screen_radius){
                drive_speed = MAX_DRIVE_SPEED * (driveVectorLength/screen_radius);
            }else{
                drive_speed = MAX_DRIVE_SPEED;
            }
        }
        
        SteerAngle steer_angle = MAX_STEER_ANGLE * (atan2(drive_vector.x,-drive_vector.y)/(PI/2));
        if (steer_angle > MAX_STEER_ANGLE){
            steer_angle = 2 * MAX_STEER_ANGLE - steer_angle;
            drive_speed = -drive_speed;
        }else if (steer_angle < -MAX_STEER_ANGLE){
            steer_angle = -2*MAX_STEER_ANGLE - steer_angle;
            drive_speed = -drive_speed;
        }
        
        //ofLog(OF_LOG_NOTICE, "D: " + ofToString(drive_speed) + "\tS" + ofToString(steer_angle));
        
        // Send data
        memcpy(&(cmdBuffer[0]), &steer_angle, sizeof(steer_angle));
        memcpy(&(cmdBuffer[sizeof(steer_angle)]), &drive_speed, sizeof(drive_speed));
        sendData(cmdBuffer, CMD_MESSAGE_SIZE);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::black);
    
    ofSetColor(ofColor::white);
    ofFill();
    ofDrawCircle(screen_center.x, screen_center.y, screen_radius);
    
    ofSetColor(ofColor::lightGray);
    ofDrawCircle(screen_center.x, screen_center.y, stop_radius);
    
    if (mouse_pressed){
        ofSetColor(ofColor::red);
        ofDrawLine(screen_center.x, screen_center.y, mouse_coords.x, mouse_coords.y);
    }
}

void ofApp::recalcScreenVariables(int w, int h){
    screen_width = MIN(w,h);
    screen_center.set(w/2.0f, h/2.0f);
    screen_radius = FULLSPEED_CLAMP_RADIUS_RATIO * (screen_width/2.0f);
    stop_radius = STOP_CLAMP_RADIUS_RATIO * screen_radius;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    mouse_coords.x = x;
    mouse_coords.y = y;
    mouse_pressed = true;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    mouse_coords.x = x;
    mouse_coords.y = y;
    mouse_pressed = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    mouse_pressed = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    recalcScreenVariables(w,h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
