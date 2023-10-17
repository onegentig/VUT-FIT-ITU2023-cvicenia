import QtQuick 2.0

Rectangle {
     id: lutdisp;
     width: 400;
     height: 25;

     property int pos: 0.0;
     property real value: 0.0;
     property color bgColor: "#999";
     property color textColor: "black";

     color: bgColor;

     Text {
          id: txt;
          font.pointSize: 10;
          anchors.verticalCenter: parent.verticalCenter;

          text: "LUT Value: " + value.toFixed(6);
          anchors.centerIn: parent;

          /*x: Math.min(
               Math.max(0, (pos / 200) * lutdisp.width - txt.width / 2),
               lutdisp.width - txt.width
          );*/
     }
}
