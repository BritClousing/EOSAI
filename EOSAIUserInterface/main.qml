
import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
//import QtQuick 2.0
//import "TextBalloonPlugin" 1.0
import GameMapLib 1.0
//import "TextBalloon.h" 1.0

ApplicationWindow {
    visible: true
    title: m_strApplicationHeader
    property int margin: 11
    width: 900 //mainLayout.implicitWidth + 2 * margin
    height: 600 //mainLayout.implicitHeight + 2 * margin
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin

    //margin: 30
    //anchors.bottomMargin: 50
    //anchors.bottom: parent.bottom

	property int iRowHeight: 23

    //signal playerComboBoxChanged(int iPlayer);
    signal playerComboBoxChanged(int iPlayer);
/*
    ListModel {
        id: testModel
        ListElement { name: "Error: No Poi Found"; icon: "E" }
        ListElement { name: "Warning: No Technologies"; icon: "W" }
        ListElement { name: "Loaded 30 Poi";   icon: "" }
        ListElement { name: "2Bill Smith"; icon: "E" }
        ListElement { name: "2John Brown"; icon: "W" }
        ListElement { name: "2Sam Wise";   icon: "" }
        ListElement { name: "3Bill Smith"; icon: "E" }
        ListElement { name: "3John Brown"; icon: "W" }
        ListElement { name: "3Sam Wise"  ; icon: "" }
        ListElement { name: "4Bill Smith"; icon: "E" }
        ListElement { name: "4John Brown"; icon: "W" }
        ListElement { name: "4Sam Wise"  ; icon: "" }
        ListElement { name: "5Bill Smith"; icon: "E" }
        ListElement { name: "5John Brown"; icon: "W" }
        ListElement { name: "5Sam Wise"  ; icon: "" }
        ListElement { name: "B1Bill Smith"; icon: "E" }
        ListElement { name: "B1John Brown"; icon: "W" }
        ListElement { name: "B1Sam Wise"  ; icon: "" }
        ListElement { name: "B2Bill Smith"; icon: "E" }
        ListElement { name: "B2John Brown"; icon: "W" }
        ListElement { name: "B2Sam Wise"  ; icon: "" }
        ListElement { name: "B3Bill Smith"; icon: "E" }
        ListElement { name: "B3John Brown"; icon: "W" }
        ListElement { name: "B3Sam Wise"  ; icon: "" }
        ListElement { name: "B4Bill Smith"; icon: "E" }
        ListElement { name: "B4John Brown"; icon: "W" }
        ListElement { name: "B4Sam Wise"  ; icon: "" }
        ListElement { name: "B5Bill Smith"; icon: "E" }
        ListElement { name: "B5John Brown"; icon: "W" }
        ListElement { name: "B5Sam Wise"  ; icon: "" }
    }
*/
    Rectangle
    {
        id: connectionStateRect
        width: parent.width
        //width: 60
        height: iRowHeight+10
        color: "#FFFFFFFF"
        anchors.left: parent.left
        anchors.top: parent.top

        //anchors.bottomMargin: 50
        //anchors.bottom: parent.bottom

        //anchors.margins: margin
        Rectangle
        {
            id: eosaiCurrentConnectionStateIcon
            //anchors.left: player1State.right
            //anchors.leftMargin: 1
            anchors.left: parent.left
            anchors.leftMargin: margin
            anchors.top: parent.top
            anchors.topMargin: 6
            //anchors.bottom: parent.bottom
            //horizontalCenter: parent.horizontalCenter
            //anchors.verticalCenter: parent.verticalCenter
            //anchors.verticalCenter

            height: 20 //parent.height
            width: 20 //parent.height
            //color: "#FAA" // Red
            //color: "#FF6" // Yellow
            color: m_strConnectionStatusColor
            //color: m_bEOSAISharedMemoryFileExists ? "#FF6" : "#FAA"
            radius: 50
            border.width: 1
            border.color: "black"
        }
        Label {
            id: eosaiCurrentConnectionState
            text: m_bEOSAISharedMemoryFileExists ?
                      "State: Detected EOSAI (version " + m_strEOSAIVersion + " / " + m_strEOSAIBuildDate + "). " + m_strConnectionState :
                      "Error: EOSAI not found.";
            anchors.left: eosaiCurrentConnectionStateIcon.right
            anchors.leftMargin: 8
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            verticalAlignment: Text.AlignVCenter
        }
    }

    ColumnLayout {
        id: mainLayout
        anchors.top: connectionStateRect.bottom
        anchors.topMargin: 2 //margin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        RowLayout {
			Layout.preferredHeight: iRowHeight
            visible: m_bEOSAISharedMemoryFileExists

            Label {
                text: "Processing State:"
            }
            Rectangle
            {
                id: eosaiProcessingState
                height: 20 //parent.height
                width: 20 //parent.height
                color: m_strOverallProcessingStateColor //"#FAA" // Red
                //color: "#FFA" // Yellow
                //color: "#AFA" // Green
                radius: 50
                border.width: 1
                border.color: "black"
            }
            Rectangle
            {
                width: 80
                height: iRowHeight
                color: "#00FFFFFF"
                Label {
                    text: m_strOverallProcessingStateText //"Idle"
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Label {
                text: "Players:"
            }
            Rectangle
            {
                id: player1State
                color: m_strPlayer1ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "1"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 1
            }
            Rectangle
            {
                id: player2State
                color: m_strPlayer2ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "2"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 2
            }
            Rectangle
            {
                id: player3State
                color: m_strPlayer3ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "3"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 3
            }
            Rectangle
            {
                id: player4State
                color: m_strPlayer4ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "4"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 4
            }
            Rectangle
            {
                id: player5State
                color: m_strPlayer5ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "5"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 5
            }
            Rectangle
            {
                id: player6State
                color: m_strPlayer6ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "6"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 6
            }
            Rectangle
            {
                id: player7State
                color: m_strPlayer7ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "7"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 7
            }
            Rectangle
            {
                id: player8State
                color: m_strPlayer8ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "8"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 8
            }
            Rectangle
            {
                id: player9State
                color: m_strPlayer9ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "9"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 9
            }
            Rectangle
            {
                id: player10State
                color: m_strPlayer10ProcessingColor; height: 20; width: 20; radius: 50; border.width: 1; border.color: "black";
                Label { text: "10"; anchors.fill: parent; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                visible: m_iNumberOfPlayers >= 10
            }
        }
/*
		RowLayout {
			Layout.preferredHeight: iRowHeight //20
			width: 500
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
                    text: "Process Turn For All Players"
                    exclusiveGroup: turnProcessingGroup
                    checked: false
                }
                RadioButton {
                    text: "No Turn Processing (Except When Explicitly Commanded)"
                    exclusiveGroup: turnProcessingGroup
                    checked: true
                }
            }
        }
        */
		Rectangle {
			// Anchored to 20px off the top right corner of the parent
			anchors.right: parent.right
			anchors.left: parent.left
			height: 1
			color: "gray"
            visible: m_bEOSAISharedMemoryFileExists
        }
		
        // Common Data
        TabView {
            visible: m_bEOSAISharedMemoryFileExists
            Layout.fillWidth: true
            Layout.fillHeight: true
            Tab {
                id: statusTab
                title: "Status"
                TableView
                {
                    id: statusTable

                    headerVisible: false
                    //anchors.margins: 0
                    anchors.top: parent.top
                    //anchors.topMargin: 5
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    //anchors.rightMargin: 5

                    //anchors.fill: parent
                    alternatingRowColors: false
                    //model: theModel
                    //model: poiListModel
                    model: statusListModel

                    // Row Delegate
                    rowDelegate: Item
                    {
                        height: 20
                        Rectangle {
                            anchors {
                                left: parent.left
                                right: parent.right
                                verticalCenter: parent.verticalCenter
                            }
                            height: parent.height
                            color: styleData.selected ? 'lightsteelblue' : 'white'
                        }
                    }
                    // Column 0
                    TableViewColumn {
                        title: "Icon";
                        role: "icon";
                        width: 25;
                        resizable: false;
                        delegate:
                            Image
                            {
                                fillMode: Image.Pad
                                //sourceSize.width: 5
                                //paintedWidth: 5
                                source: styleData.value == "E" ? "qrc:/Images/Error.png" : styleData.value == "W" ? "qrc:/Images/Warning.png" : ""
                                Component.onCompleted:
                                {
                                    //anchors.right = parent.right;
                                    //anchors.rightMargin = 5;
                                    //anchors.leftMargin = 5;
                                    anchors.verticalCenter = parent.verticalCenter;
                                    //anchors.horizontalCenter = parent.horizontalCenter;
                                    /*
                                    width = parent.width
                                    //anchors.verticalCenter = parent.verticalCenter
                                    anchors.left = parent.left
                                    anchors.leftMargin = 10
                                    */
                                }
                            }
                            /*
                            Label
                            {
//                                width: parent ? parent.width : 0
                                text: styleData.value
                                height: 30
//                                anchors.left: parent ? parent.left : 0
//                                anchors.leftMargin: 15
                                //anchors.verticalCenter: parent.verticalCenter
                                verticalAlignment: Text.AlignVCenter
                                Component.onCompleted:
                                {
                                    width = parent.width
                                    //anchors.verticalCenter = parent.verticalCenter
                                    anchors.left = parent.left
                                    anchors.leftMargin = 10
                                }
                            }
                            */
                    }
                    // Column 1
                    TableViewColumn {
                        title: "Status";
                        role: "statusText";
                        width: 400
                        delegate:
                            Label
                            {
                                //width: parent ? parent.width : 0
                                text: styleData.value
                                //anchors.left: parent ? parent.left : 0
                                //anchors.leftMargin: 15
                                verticalAlignment: Text.AlignVCenter
                                Component.onCompleted:
                                {
                                    width = parent.width
                                    anchors.left = parent.left
                                    anchors.leftMargin = 10
                                }
                            }
                    }
                }

                /*
                Rectangle
                {
                    anchors.fill: parent
                    border.width: 2
                    ListModel {
                        id: libraryModel2
                        ListElement { title: "1A Masterpiece"; statusText: "Gabriel"; }
                        ListElement { title: "1Brilliance"; statusText: "Jens"; }
                        ListElement { title: "1Outstanding"; statusText: "Frederik"; }
                        ListElement { title: "2A Masterpiece"; statusText: "Gabriel"; }
                        ListElement { title: "2Brilliance"; statusText: "Jens"; }
                        ListElement { title: "2Outstanding"; statusText: "Frederik"; }
                        ListElement { title: "3A Masterpiece"; statusText: "Gabriel"; }
                        ListElement { title: "3Brilliance"; statusText: "Jens"; }
                        ListElement { title: "A-3Outstanding"; statusText: "Frederik"; }
                    }
                    ListView {
                        id: statusList
                        anchors.fill: parent
                        //width: 180;
                        //height: 3000;
                        //implicitHeight: 3000;
                        //anchors.topMargin: 10
                        //anchors.bottomMargin: 100

                        model: statusLineModel
                        //model: libraryModel2
                        //model: testModel //ContactModel {}
                        //highlight: Rectangle { color: "lightsteelblue"; } //radius: 5 }
                        highlightMoveDuration: 0
                        //highlightMoveVelocity: 1000
                        highlightResizeDuration: 0
                        //highlightFollowsCurrentItem: true
                        delegate:
                            Component
                            {
                                //Item
                                //{
                                //    width: parent.width
                                //    height: 18
                                    Rectangle {
                                        id: wrapper
                                        width: parent.width
                                        height: 18
                                        //width: 180
                                        //height: contactInfo.height
                                        //height: parent.height
                                        color: ListView.isCurrentItem ? "lightsteelblue" : "#00FFFFFF"
                                        //color: isCurrentItem ? "black" : "red"

                                        Image
                                        {
                                            anchors.left: parent.left; anchors.leftMargin: 10;
                                            anchors.verticalCenter: parent.verticalCenter;
                                            source: icon == "E" ? "qrc:/Images/Error.png" : icon == "W" ? "qrc:/Images/Warning.png" : ""
                                        }
                                        Text
                                        {
                                            anchors.left: parent.left; anchors.leftMargin: 30;
                                            anchors.verticalCenter: parent.verticalCenter;
                                            text: statusText;
                                        }
                                    //}
                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: statusList.currentIndex = index
                                        }
                                    }
                                //}
                            }
                        focus: true
                        //onCurrentItemChanged: console.log(model.get(statusList.currentIndex).name + ' selected')
                    }
                    ScrollBar {
                        flk: statusList;
                        //flickable: statusList;
                    }
                }
                */
                /*
                ScrollView
                {
                    verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
                    //anchors.topMargin: 10
                    ListView {
                        id: statusList
                        width: 180; height: 200
                        //anchors.topMargin: 10
                        //anchors.bottomMargin: 100

                        model: statusLineModel
                        //model: testModel //ContactModel {}
                        highlight: Rectangle { color: "lightsteelblue"; } //radius: 5 }
                        highlightMoveDuration: 0
                        //highlightMoveVelocity: 1000
                        highlightResizeDuration: 0
                        delegate: Component {
                            Item {
                                width: parent.width
                                height: 18
                                Image
                                {
                                    anchors.left: parent.left; anchors.leftMargin: 10;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    source: icon == "E" ? "qrc:/Images/Error.png" : icon == "W" ? "qrc:/Images/Warning.png" : ""
                                }
                                Text
                                {
                                    anchors.left: parent.left; anchors.leftMargin: 30;
                                    anchors.verticalCenter: parent.verticalCenter;
                                    text: statusText;
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: statusList.currentIndex = index
                                }
                            }
                        }
                        focus: true
                        //onCurrentItemChanged: console.log(model.get(statusList.currentIndex).name + ' selected')
                    }
                }
                */

                /*
                ScrollView
                {
                    verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
                    Label
                    {
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        text: m_szCommonData_Status
                    }
                }
                */
            }
            /*
            Tab {
                title: "Warnings and Errors " + m_strCommonWarningsAndErrorsCount
                ScrollView
                {
                    verticalScrollBarPolicy: Qt.ScrollBarAsNeeded //ScrollBarAlwaysOn //ScrollBarAsNeeded
                    //contentItem: Label
                    Label
                    {
                        //anchors.fill: parent
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        //anchors.bottom: parent.bottom
                        //anchors.bottomMargin: 10
                        text: m_strCommonWarningsAndErrors
                    }
                }
            }
            */
            Tab {
                title: "Players" //National State"
                // Player Personalities, Power, Resource Values

                ColumnLayout
                {
                    anchors.fill: parent
                    anchors.margins: margin

                    Label
                    {
                        //anchors.fill: parent
                        //text: m_strPlayerXDetails
                        text: m_strPlayer2Details
                    }
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
                // Each players attitudes towards other players

/*
                TableView {
                    //headerVisible: false
                    alternatingRowColors: false
                    TableViewColumn {
                        role: "desc"
                        //role: "desc"
                        //role: "title"
                        title: "Title"
                        width: 100
                        movable: false
                        resizable: false
                        delegate:
                        TextField {
                            anchors.fill: parent
                            text: styleData.value
                        }
                    }
                    //model: libraryModel
                    //model: statusLineModel
                    model: poiListModel //libraryModel

                    ListModel {
                        id: libraryModel
                        ListElement { desc: "Gabriel"; }
                        ListElement { desc: "Jens"; }
                        ListElement { desc: "Frederik"; }
                        ListElement { desc: "Gabriel"; }
                        ListElement { desc: "Jens"; }
                        ListElement { desc: "Frederik"; }
                        ListElement { desc: "Gabriel"; }
                        ListElement { desc: "Jens"; }
                    }
                }
*/
                /*
                TableView
                {
                    TableViewColumn {
                        role: "desc"
                        title: "Desc"
                        width: 100
                    }
                    TableViewColumn {
                        role: "icon"
                        title: "Icon"
                        width: 100
                    }
                    model: poiListModel
                }
                */

                /*
                ScrollView
                {
                    anchors.fill: parent
                    verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
                    frameVisible: true
                    //border.width: 1
                    contentItem: statusList
                    //viewport: statusList

                    Rectangle
                    {
                        width: 800
                        height: 700
                        color: "red"

                        Rectangle
                        {
                            anchors.top: parent.top
                            anchors.topMargin: 200
                            width: 200
                            height: 200
                            color: "gray"
                        }
                    }
                }
                */
            }
            Tab {
                title: "Map" //" (" + m_iNumberOfPoi + " Poi)"
                Rectangle
                {
                    Layout.fillWidth: true
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 2 //0
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 10

                    GameMapImage
                    {
                        id: poiGameMapImage
                        //Layout.fillWidth: true
                        anchors.left: parent.left
                        //anchors.leftMargin: 10
                        anchors.top: parent.top //mapSizeText.bottom
                        //anchors.topMargin: 10
                        //anchors.right: parent.right
                        //anchors.rightMargin: 10
                        anchors.bottom: parent.bottom
                        //anchors.bottomMargin: 10

                        //height: 100
                        //width: 200
                        width: parent.width - 300

                        //anchors.right: true //index % 2 == 0 ? undefined : parent.right
                        //height: 60
                        //rightAligned: true //index % 2 == 0 ? false : true
                        //width: balloonWidth
                    }
                    Rectangle
                    {
                        id: rightSidebar
                        anchors.left: poiGameMapImage.right
                        anchors.leftMargin: 10
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        width: 300
                        //border.color: "#888"
                        //border.width: 1

                        Label
                        {
                            id: viewAsPlayer
                            text: "View As:"
                            anchors.top: parent.top
                            anchors.left: parent.left
                            //height: 100 //viewAsCombo2.height
                            height: viewAsPlayerCombo.height
                            verticalAlignment: Text.AlignVCenter
                            //Text
                            //anchors.right: parent.right
                        }
                        ComboBox {
                            id: viewAsPlayerCombo
                            anchors.top: viewAsPlayer.top
                            anchors.left: viewAsPlayer.right
                            anchors.leftMargin: 5

                            implicitWidth: 150
                            currentIndex: 1
                            model: m_PlayerDropdownList
                            //onCurrentIndexChanged: playerComboBoxChanged(currentIndex);
                        }
                        Label
                        {
                            id: viewData
                            text: "View:"
                            anchors.top: viewAsPlayer.bottom
                            anchors.topMargin: 5
                            anchors.left: parent.left
                            //height: 100 //viewAsCombo2.height
                            height: viewDataCombo.height
                            verticalAlignment: Text.AlignVCenter
                            //Text
                            //anchors.right: parent.right
                        }
                        ComboBox {
                            id: viewDataCombo
                            anchors.top: viewData.top
                            //anchors.left: viewData.right
                            anchors.left: viewAsPlayerCombo.left
                            //anchors.leftMargin: 5

                            implicitWidth: 150
                            currentIndex: 0
                            model: [ "Poi", "National Pathways", "Desires", "Tactical Projects" ]
                            //model: m_PlayerDropdownList
                            //onCurrentIndexChanged: playerComboBoxChanged(currentIndex);
                        }
                        TableView
                        {
                            id: poiTable

                            headerVisible: false
                            anchors.top: viewData.bottom
                            anchors.topMargin: 5
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.rightMargin: 5

                            //anchors.fill: parent
                            alternatingRowColors: false
                            //model: theModel
                            model: poiListModel

                            // Row Delegate
                            rowDelegate: Item
                            {
                                height: 20
                                Rectangle {
                                    anchors {
                                        left: parent.left
                                        right: parent.right
                                        verticalCenter: parent.verticalCenter
                                    }
                                    height: parent.height
                                    color: styleData.selected ? 'lightsteelblue' : 'white'
                                }
                            }
                            // Column 0
                            TableViewColumn {
                                title: "Name";
                                role: "name";
                                width: 100;
                                resizable: false;
                                delegate:
                                    Label
                                    {
        //                                width: parent ? parent.width : 0
                                        text: styleData.value
                                        height: 30
        //                                anchors.left: parent ? parent.left : 0
        //                                anchors.leftMargin: 15
                                        //anchors.verticalCenter: parent.verticalCenter
                                        verticalAlignment: Text.AlignVCenter
                                        Component.onCompleted:
                                        {
                                            width = parent.width
                                            //anchors.verticalCenter = parent.verticalCenter
                                            anchors.left = parent.left
                                            anchors.leftMargin = 10
                                        }
                                    }
                            }
                            // Column 1
                            TableViewColumn {
                                title: "Desc";
                                role: "desc";
                                width: 100
                                delegate:
                                    Label
                                    {
                                        //width: parent ? parent.width : 0
                                        text: styleData.value
                                        //anchors.left: parent ? parent.left : 0
                                        //anchors.leftMargin: 15
                                        verticalAlignment: Text.AlignVCenter
                                        Component.onCompleted:
                                        {
                                            width = parent.width
                                            anchors.left = parent.left
                                            anchors.leftMargin = 10
                                        }
                                    }
                            }
                            // Column 2
                            /*
                            TableViewColumn {
                                title: "Owner";
                                role: "owner";
                                width: 40
                                delegate:
                                    Label
                                    {
                                        //width: parent ? parent.width : 0
                                        text: styleData.value
                                        //anchors.left: parent ? parent.left : 0
                                        //anchors.leftMargin: 15
                                        verticalAlignment: Text.AlignVCenter
                                        Component.onCompleted:
                                        {
                                            width = parent.width
                                            anchors.left = parent.left
                                            anchors.leftMargin = 10
                                        }
                                    }
                            }
                            */
                        }

                        /*
                        Label
                        {
                            id: viewAsCombo
                            text: "sfkljsdf"
                            anchors.top: parent.top
                            anchors.left: viewAs.right
                        }
                        */
                        /*
                        ScrollView
                        {
                            verticalScrollBarPolicy: Qt.ScrollBarAlwaysOn
                            anchors.left: parent.left
                            //anchors.left: poiGameMapImage.right
                            //anchors.leftMargin: 10
                            anchors.top: viewData.bottom
                            anchors.topMargin: 5
                            //anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.right: parent.right
                            //width: 300
                            Label
                            {
                                //anchors.fill: parent
                                //anchors.left: parent.left
                                //anchors.leftMargin: 15
                                //anchors.top: parent.top
                                //anchors.bottom: parent.bottom
                                //anchors.right: parent.right
                                //width: 200
                                //anchors.topMargin: 10
                                //width: 200
                                text: m_szCommonData_Poi
                            }
                        }
                        */
                        /*
                        ScrollView
                        {
                            verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: viewData.bottom
                            anchors.topMargin: 5
                            anchors.bottom: parent.bottom
                            //anchors.topMargin: 10
                            ListView {
                                id: poiList
                                width: 180; height: 200
                                //anchors.topMargin: 10
                                //anchors.bottomMargin: 100

                                model: poiListModel
                                //model: testModel //ContactModel {}
                                highlight: Rectangle { color: "lightsteelblue"; } //radius: 5 }
                                highlightMoveDuration: 0
                                //highlightMoveVelocity: 1000
                                highlightResizeDuration: 0
                                delegate: Component {
                                    Item {
                                        width: parent.width
                                        height: 18
                                        /-*
                                        Image
                                        {
                                            anchors.left: parent.left; anchors.leftMargin: 10;
                                            anchors.verticalCenter: parent.verticalCenter;
                                            source: icon == "E" ? "qrc:/Images/Error.png" : icon == "W" ? "qrc:/Images/Warning.png" : ""
                                        }
                                        *-/
                                        Text
                                        {
                                            anchors.left: parent.left; anchors.leftMargin: 30;
                                            anchors.verticalCenter: parent.verticalCenter;
                                            text: desc;// + "/" + owner;
                                        }
                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: poiList.currentIndex = index
                                        }
                                    }
                                }
                                focus: true
                                //onCurrentItemChanged: console.log(model.get(statusList.currentIndex).name + ' selected')
                            }
                        }
                        */
                    }
                }
            }
            Tab {
                title: "Unit Templates (" + m_iCommonData_NumberOfUnitTemplates + ")"
                //width: 400
                ScrollView
                {
                    verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
                    Label
                    {
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        //text: m_strCommonUnitTemplates
                        text: m_szCommonData_UnitTemplates
                    }
                }
            }
            Tab {
                title: "Improvements (-)"
                ScrollView
                {
                    verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
                    Label
                    {
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        //text: m_strCommonUnitTemplates
                        text: "Not implemented yet" //m_szCommonData_UnitTemplates
                    }
                }
            }
            Tab {
                title: "Technologies (" + m_iCommonData_NumberOfTechnologies + ")"
                ScrollView
                {
                    verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
                    Label
                    {
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        //text: "Biplane\nDestroyer (Class 1)"
                        text: m_szCommonData_Technologies
                    }
                }
            }
            Tab {
                title: "Tech Tree (-)"
                ScrollView
                {
                    verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
                    Label
                    {
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        text: "Not implemented yet"
                        //text: m_szCommonData_Technologies
                    }
                }
            }
            Tab {
                title: "Event Log"
                //Rectangle { color: "green" }
            }
        }
    }
}
