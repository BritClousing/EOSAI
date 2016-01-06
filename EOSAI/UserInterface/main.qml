
import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0

ApplicationWindow {
    visible: true
    title: "EOSAI User Interface" + MyController
    property int margin: 11
    width: 800 //mainLayout.implicitWidth + 2 * margin
    height: 600 //mainLayout.implicitHeight + 2 * margin
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin
	
	property int iRowHeight: 23

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: margin

        RowLayout {
			Layout.preferredHeight: iRowHeight
			Rectangle
			{
				width: 60
				height: iRowHeight
				color: "#00FFFFFF"
				Label {
					id: eosaiCurrentTurn
					text: "Turn: 5"
					anchors.fill: parent
					verticalAlignment: Text.AlignVCenter
				}
			}
			Label {
				id: eosaiState
				text: "Process Turns:"
			}
			RowLayout {
				ExclusiveGroup { id: turnProcessingGroup }
				RadioButton {
					text: "Process All Turns Automatically"
					exclusiveGroup: turnProcessingGroup
					checked: false
				}
				RadioButton {
					text: "Process Turn 5 For All Players"
					exclusiveGroup: turnProcessingGroup
					checked: false
				}
				RadioButton {
					text: "No Turn Processing (Except When Explicitly Commanded)"
					exclusiveGroup: turnProcessingGroup
					checked: true
				}
			}
			/*
			Button{
				implicitWidth: 200
				text: "Process Turn 5 for All Players"
			}
			*/
		}
		RowLayout {
			Layout.preferredHeight: iRowHeight //20
			Label {
				text: "Processing State:"
			}
			Rectangle
			{
				id: eosaiProcessingState
				height: 20 //parent.height
				width: 20 //parent.height
				color: "#FAA" // Red
				//color: "#FFA" // Yellow
				//color: "#AFA" // Green
				radius: 50
				border.width: 1
				border.color: "black"
			}
			Label {
				text: "Idle"
			}
		}
		RowLayout {
			Layout.preferredHeight: iRowHeight //20
			//Layout.preferredWidth: 500
			//Layout.fillWidth: true
			//Layout.preferredHeight: 20
			width: 500

			Label {
				text: "View:"
			}
			ComboBox {
				implicitWidth: 100
				currentIndex: 1
				model: [ "EOSAI Data", "Player 1", "Player 2", "Player 3" ]
			}
			Label {
				text: "State of AI Players:"
			}
			Rectangle
			{
				id: player1StateB
				//anchors.left: aiPlayersLabel.right
				//anchors.leftMargin: 1
				height: 20 //parent.height
				width: 20 //parent.height
				//color: "#FAA" // Red
				//color: "#FFA" // Yellow
				color: "#AFA" // Green
				radius: 50
				border.width: 1
				border.color: "black"
				Label {
					text: "1"
					anchors.fill: parent
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
				}
			}
			Rectangle
			{
				id: player2StateA
				//anchors.left: player1State.right
				//anchors.leftMargin: 1
				height: 20 //parent.height
				width: 20 //parent.height
				color: "#FAA"
				radius: 50
				border.width: 1
				border.color: "black"
				Label {
					text: "2"
					anchors.fill: parent
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
				}
			}
			Rectangle
			{
				id: player3StateA
				height: 20 //parent.height
				width: 20 //parent.height
				color: "#FAA"
				radius: 50
				border.width: 1
				border.color: "black"
				Label {
					text: "3"
					anchors.fill: parent
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
				}
			}
		}
		
		Rectangle {
			// Anchored to 20px off the top right corner of the parent
			anchors.right: parent.right
			anchors.left: parent.left
			height: 1
			color: "gray"
		}
		
        RowLayout {
			height: 20
			Label {
				id: actions
				//implicitWidth: 100
				text: "Actions:"
			}
			Button{
				implicitWidth: 200
				//text: "Process Player 1's Turn"
				text: "Process Turn (Player 1 Only)"
			}
			Label {
				id: status
				text: "Status: Turn Processed"
			}
		}

		TabView {
			//width: parent.width
			//width: 500
            Layout.fillWidth: true
            Layout.fillHeight: true
			Tab {
				title: "Details"
				ColumnLayout {
					id: detailsLayout
					anchors.fill: parent
					anchors.margins: margin
					Label {
						text: "Player 1 (Rommel)"
					}
					Label {
						id: personalityLabel
						text: "Personality:"
					}
					Label {
						anchors.left: parent.left
						anchors.leftMargin: 15
						text: "Holds Grudges"
					}
					Label {
						anchors.left: parent.left
						anchors.leftMargin: 15
						text: "30% Economy, 20% Technology, 50% Military"
					}
					Label {
						anchors.left: parent.left
						anchors.leftMargin: 15
						text: "Talkative (0/5)"
					}
					Label {
						text: ""
						Layout.fillHeight: true
					}
				}
			}
			Tab {
				title: "National State"
				ColumnLayout {
					anchors.fill: parent
					anchors.margins: margin

					Label {
						text: "Production Power: 470 Total"
					}
					Rectangle
					{
						anchors.left: parent.left
						anchors.leftMargin: 15
						color: "#FBB"
						border.width: 1
						border.color: "gray"
						height: 25
						width: 250
						Label {
							//anchors.left: parent.left
							//anchors.leftMargin: 15
							anchors.fill: parent
							verticalAlignment: Text.AlignVCenter
							anchors.left: parent.left
							anchors.leftMargin: 10
							text: "Player 1 (Rommel): 250"
						}
					}
					Rectangle
					{
						anchors.left: parent.left
						anchors.leftMargin: 15
						color: "#BFB" // light green
						border.width: 1
						height: 25
						width: 220
						Label {
							//anchors.left: parent.left
							//anchors.leftMargin: 15
							anchors.fill: parent
							verticalAlignment: Text.AlignVCenter
							anchors.left: parent.left
							anchors.leftMargin: 10
							text: "Player 2 (Montgomery): 220"
						}
					}
					Label {
						text: "Military Power: 210 Total"
					}
					Rectangle
					{
						anchors.left: parent.left
						anchors.leftMargin: 15
						color: "#FBB"
						border.width: 1
						border.color: "gray"
						height: 25
						width: 120
						Label {
							//anchors.left: parent.left
							//anchors.leftMargin: 15
							anchors.fill: parent
							verticalAlignment: Text.AlignVCenter
							anchors.left: parent.left
							anchors.leftMargin: 10
							text: "Player 1 (Rommel): 120"
						}
					}
					Rectangle
					{
						anchors.left: parent.left
						anchors.leftMargin: 15
						color: "#BFB" // light green
						border.width: 1
						height: 25
						width: 90
						Label {
							//anchors.left: parent.left
							//anchors.leftMargin: 15
							anchors.fill: parent
							verticalAlignment: Text.AlignVCenter
							anchors.left: parent.left
							anchors.leftMargin: 10
							text: "Player 2 (Montgomery): 90"
						}
					}
					Label {
						text: "Resources"
					}
					Label {
						text: "Money: 200 (+2/turn)"
						anchors.left: parent.left
						anchors.leftMargin: 15
					}
					Label {
						text: "Iron: 100 (+1/turn)"
						anchors.left: parent.left
						anchors.leftMargin: 15
					}
					Label {
						text: "Oil: 100 (+1/turn)"
						anchors.left: parent.left
						anchors.leftMargin: 15
					}
					Label {
						text: "Food: 20 (-2/turn) - Scarce"
						anchors.left: parent.left
						anchors.leftMargin: 15
					}

					Label {
						text: ""
						Layout.fillHeight: true
					}
				}
			}
			Tab {
				title: "Foreign Relations"
				//width: 300
				//Rectangle { color: "red" }
			}
			Tab {
				title: "Map"
				//Rectangle { color: "blue" }
			}
			Tab {
				title: "Desires"
				//Rectangle { color: "blue" }
			}
			Tab {
				title: "Tactical Projects"
				//Rectangle { color: "green" }
			}
			Tab {
				title: "Event Log"
				//Rectangle { color: "green" }
			}
		}

		/*
        GroupBox {
            id: rowBox
            title: "Row layout"
            Layout.fillWidth: true

            RowLayout {
                id: rowLayout
                anchors.fill: parent
                TextField {
                    placeholderText: "This wants to grow horizontally"
                    Layout.fillWidth: true
                }
                Button {
                    text: "Button"
                }
            }
        }

        GroupBox {
            id: gridBox
            title: "Grid layout"
            Layout.fillWidth: true

            GridLayout {
                id: gridLayout
                rows: 3
                flow: GridLayout.TopToBottom
                anchors.fill: parent

                Label { text: "Line 1" }
                Label { text: "Line 2" }
                Label { text: "Line 3" }

                TextField { }
                TextField { }
                TextField { }

                TextArea {
                    text: "This widget spans over three rows in the GridLayout.\n"
                        + "All items in the GridLayout are implicitly positioned from top to bottom."
                    Layout.rowSpan: 3
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
            }
        }
        TextArea {
            id: t3
            text: "This fills the whole cell"
            Layout.minimumHeight: 30
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
		*/
    }
}
