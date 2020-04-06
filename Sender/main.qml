import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import "."

ApplicationWindow {
	visible: true
	width: 640
	height: 480
	title: qsTr("Remote subtitle sender")

	background: Rectangle {
		color: "black"
	}

	Label {
		anchors.centerIn: parent
		text: "MAIN WINDOW"
		color: "white"
	}
}
