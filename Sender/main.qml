import QtQuick 2.12
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtWebSockets 1.14
import Sender 1.0
import "."

ApplicationWindow {
	visible: true
	width: 640
	height: 480
	title: qsTr("Remote subtitle sender")

	background: Rectangle {
		color: "black"
	}

	Sender {
		id: sender
	}

	WebSocket {
		id: socket

		url: "ws://vjpvm.piarista.hu:10101"

		onStatusChanged: {
			if (status === WebSocket.Open) {
				console.debug("send init")
				sendTextMessage("SOCKET:SENDER")
			}
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

	ListView {
		id: view
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.bottom: parent.bottom
		anchors.right: col.left

		snapMode: ListView.SnapToItem

		model: ListModel { }

		delegate: Rectangle {
			id: item
			color: ListView.isCurrentItem ? "blue" : "transparent"
			border.color: "blue"
			border.width: 1

			height: 96
			width: view.width

			Label {
				color: "white"
				anchors.fill: parent
				text: model.text
			}

			MouseArea {
				anchors.fill: parent
				acceptedButtons: Qt.LeftButton
				onClicked: view.currentIndex = index
			}
		}

		onCurrentIndexChanged: {
			if (currentIndex === -1) {
				rectSend.isHided = true
				labelSend.text = ""
			} else {
				labelSend.text = model.get(currentIndex).text
			}
		}
	}

	Column {
		id: col
		anchors.top: parent.top
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		width: parent.width/3

		Rectangle {
			id: rectSend
			width: parent.width
			height: parent.height/2

			property bool isHided: true

			color: isHided ? "black" : "white"

			Label {
				id: labelSend
				anchors.fill: parent

				color: rectSend.isHided ? "grey" : "black"

				wrapMode: Text.WordWrap

				onTextChanged: sendText()
			}

			MouseArea {
				anchors.fill: parent
				acceptedButtons: Qt.LeftButton
				onClicked: rectSend.isHided = !rectSend.isHided
			}

			onIsHidedChanged: sendText()
		}

		Rectangle {
			width: parent.width
			height: parent.height/2
			color: "yellow"

			Label {
				anchors.centerIn: parent
				text: "TOVÁBB"

			}

			MouseArea {
				anchors.fill: parent
				acceptedButtons: Qt.LeftButton
				onClicked: view.incrementCurrentIndex()
			}
		}
	}

	function sendText() {
		if (rectSend.isHided || labelSend.text === "#" )
			socket.sendTextMessage("")
		else
			socket.sendTextMessage(labelSend.text)
	}

	Component.onCompleted: {
		var l = sender.loadData()

		for (var i=0; i<l.length; ++i) {
			view.model.append({text: l[i]})
		}
	}
}
