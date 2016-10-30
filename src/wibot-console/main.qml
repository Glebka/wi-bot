import QtQuick 2.3
import CameraItem 1.0

CameraItem
{
        id: camera
        width: 800
        height: 600
        stream: streamObj
        focus: true

        Keys.onPressed: {
            if (event.isAutoRepeat)
                return;
            switch (event.key)
            {
            case Qt.Key_W:
                motoControl.upPressed()
                break;
            case Qt.Key_A:
                motoControl.leftPressed()
                break;
            case Qt.Key_D:
                motoControl.rightPressed()
                break;
            default:
                break;
            }
        }

        Keys.onReleased: {
            if (event.isAutoRepeat)
                return;
            switch (event.key)
            {
            case Qt.Key_W:
                motoControl.upReleased()
                break;
            case Qt.Key_A:
                motoControl.leftReleased()
                break;
            case Qt.Key_D:
                motoControl.rightReleased()
                break;
            default:
                break;
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton

            onPositionChanged: {
                if (pressedButtons & Qt.LeftButton) {
                    camControl.mouseMoved( mouse.x, mouse.y );
                }
            }
            onReleased: { camControl.mouseReleased(); }
        }
}


