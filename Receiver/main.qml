import QtQuick 2.12
import QtQuick.Controls 2.14
import QtWebSockets 1.14
import QtQml 2.14
import Receiver 1.0
import "."

ApplicationWindow {
	visible: true
	width: 640
	height: 480
	title: qsTr("Remote subtitle receiver")

	background: Rectangle {
		color: "black"
	}

	Receiver {
		id: receiver
	}

	WebSocket {
		id: socket

		onStatusChanged: {
			if (status === WebSocket.Open) {
				console.debug("send init")
				sendTextMessage("SOCKET:RECEIVER")
			}
		}

		onTextMessageReceived: {
			var p = receiver.parseMessage(message)
			labelText.text = p
		}

	}

	Timer {
		interval: 5000
		running: true
		repeat: true
		triggeredOnStart: true
		onTriggered: {
			if (socket.status !== WebSocket.Open) {
				socket.active = false
				socket.active = true
			}
		}
	}


	Rectangle {
		id: setupRect
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		color: switch (socket.status) {
			   case WebSocket.Connecting: "blue"; break
			   case WebSocket.Open: "green"; break
			   case WebSocket.Closing: "yellow"; break
			   case WebSocket.Closed: "orange"; break
			   case WebSocket.Error: "red"; break
			   default: "brown";
			   }

		opacity: 0

		Behavior on opacity {  NumberAnimation { duration: 175 }  }

		height: col.height

		MouseArea {
			anchors.fill: parent
			acceptedButtons: Qt.LeftButton
			onClicked: setupRect.opacity = 1
		}

		Column {
			id: col
			width: parent.width

			Row {
				Label {
					text: "Socket:"
				}

				TextField {
					id: textSocket
					text: socket.url
					enabled: setupRect.opacity == 1
				}

				Button {
					text: "Connect"
					enabled: setupRect.opacity == 1
					onClicked: {
						socket.url = textSocket.text
						socket.active = true
					}
				}


				Button {
					text: "+"
					enabled: setupRect.opacity == 1
					onClicked: {
						labelText.font.pointSize += 2
					}
				}

				Button {
					text: "-"
					enabled: setupRect.opacity == 1
					onClicked: {
						labelText.font.pointSize -= 2
					}
				}
			}

			Row {
				Label {
					text: "File:"
				}

				TextField {
					id: textFile
					text: receiver.fileName
					enabled: setupRect.opacity == 1
				}

				Button {
					text: "Save"
					enabled: setupRect.opacity == 1
					onClicked: {
						receiver.fileName = textFile.text
					}
				}
			}
		}
	}

	Label {
		id: labelText
		anchors.fill: parent
		anchors.margins: 50
		anchors.topMargin: setupRect.height+50
		text: "MAIN RECEIVER WINDOW"
		color: "white"

		horizontalAlignment: Text.AlignLeft
		verticalAlignment: Text.AlignTop
		wrapMode: Text.WordWrap
	}

	Rectangle {
		id: connectedRect
		width: 10
		height: 10
		radius: 5
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		color: socket.status===WebSocket.Open ? "green" : "red"
	}

	MouseArea {
		anchors.top: setupRect.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		acceptedButtons: Qt.LeftButton
		onClicked: setupRect.opacity = 0
	}


	Component.onCompleted: {
		socket.url = receiver.getUrl()
		labelText.font.pointSize = receiver.getSize()
	}

	onClosing: {
		receiver.setUrl(socket.url)
		receiver.saveFile()
		receiver.setSize(labelText.font.pointSize)
	}
}
