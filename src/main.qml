import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs

Window {
    id: window
    visible: true
    width: 720
    height: 480

    title: qsTr("ComicBookReader")

    Item {
        anchors.fill: parent
        anchors.centerIn: parent
        Keys.onRightPressed: buttonPageUp.pageUp()
        Keys.onLeftPressed: buttonPageDown.pageDown()
        Keys.onUpPressed: buttonPageUp.pageUp()
        Keys.onDownPressed: buttonPageDown.pageDown()
        MouseArea {
            id: mouseArea
            width: parent.width
            anchors.top:parent.top
            anchors.bottom: slider.top
            anchors.left: parent.left
            anchors.right: parent.right
            Layout.fillHeight: true
            drag.target: showWindow
            enabled: true
            property double factor: 1.25
            property double oldImgX: 0.0
            property double oldImgY: 0.0
            hoverEnabled: true
            onWheel:{
                oldImgX = showWindow.x
                oldImgY = showWindow.y
                if(wheel.modifiers & Qt.ControlModifier){
                    if( wheel.angleDelta.y > 0 )  // zoom in
                    {
                        showWindow.width *= factor
                        showWindow.height *= factor
                        showWindow.x = oldImgX - (factor - 1)*(mouseX - oldImgX)
                        showWindow.y = oldImgY - (factor -1)*(mouseY - oldImgY)

                    }
                    else if( wheel.angleDelta.y < 0 )    // zoom out
                    {
                        showWindow.width /= factor
                        showWindow.height /= factor
                        showWindow.x = oldImgX + (1 - 1/factor)*(mouseX - oldImgX)
                        showWindow.y = oldImgY + (1 - 1/factor)*(mouseY - oldImgY)

                    }

                }
            }
            Item{

                id: showWindow
                width: parent.width
                height: parent.height
                anchors.horizontalCenter : parent.horizontalCenter
                anchors.verticalCenter : parent.verticalCenter

                Image {
                    id: showImage
                    objectName:"ShowImage"
                    width: (showImage2.status === Image.Error||showImage2.status == Image.Null || showImage2.opacity == 0.0) ? parent.width : parent.width*0.48
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 0
                    property int pageNum: 0
                    property int bookmark: 0
                    source: "image://myimg/"+pageNum.toString()
                    cache: false
                    fillMode: Image.PreserveAspectFit
                    mipmap: true
                    onPageNumChanged: {
                        if(pageNum > slider.to){

                            pageNum = slider.to;
                        }else if(pageNum < 1){
                            pageNum = 0 ;
                        }

                        showWindow.anchors.horizontalCenter = showWindow.parent.horizontalCenter
                        showWindow.anchors.verticalCenter = showWindow.parent.verticalCenter
                        if(showImage2.isShow == true){
                            if(pageNum === slider.to) pageNum -= 1;
                        }
                    }

                }

                Image{
                     id: showImage2
                     objectName: "ShowImage2"
                     property bool isShow: false
                     width: parent.width *0.48
                     anchors.right: parent.right
                     anchors.top: parent.top
                     anchors.bottom: parent.bottom
                     fillMode: Image.PreserveAspectFit
                     source: "image://myimg/"+(showImage.pageNum + 1).toString()
                     opacity: 0.0
                     cache: false
                     mipmap: true

                }

                Drag.active: mouseArea.drag.active

                states: State {
                        when: mouseArea.drag.active

                        AnchorChanges { target: showWindow; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
                }

                function zoom(p){
                    var oldImgX;
                    var oldImgY;
                    oldImgX = showWindow.x;
                    oldImgY = showWindow.y;
                    showWindow.width *= p;
                    showWindow.height *= p;
                    showWindow.x = showWindow.parent.width/2.0 - p*(showWindow.parent.width/2.0 - oldImgX);
                    showWindow.y = showWindow.parent.height/2.0 - p*(showWindow.parent.height/2.0 - oldImgY);
                }


                Connections {
                    target: buttonZoomIn
                    onClicked: showWindow.zoom(1.25)
                }
                Connections{
                    target: buttonZoomOut
                    onClicked: showWindow.zoom(0.8)
                }

                Connections{
                    target: buttonTwoPageid
                    onClicked:{
                        if( showImage2.opacity === 0){
                            // basculer pour afficher la deuxième image
                            showImage2.opacity = 1.0
                            showImage2.isShow = true
                            source: "image://myimg/"+(showImage.pageNum + 1).toString()
                            }
                        else{
                            //basculer pour cacher la deuxième image
                            showImage2.opacity = 0.0
                            showImage2.isShow = false
                            source: undefined
                        }
                    }
                }

            }


            onWidthChanged: {
                showWindow.anchors.horizontalCenter = mouseArea.horizontalCenter
                showWindow.anchors.verticalCenter = mouseArea.verticalCenter
            }

        }
        CustomSlider {
            id:slider
            anchors.bottom:  bottomBar.top
            width:parent.width
            objectName: "SlideBar"
            anchors.bottomMargin: 0
            rightPadding: 3
            bottomPadding: 0
            leftPadding: 3
            topPadding: 0
            from : 0
            value: 1
            to :1
            snapMode: Slider.SnapOnRelease
            stepSize: 1
            Connections {
                target: showImage

                onPageNumChanged: {
                    slider.value = showImage.pageNum
                }
            }
                onValueChanged: {
                showImage.pageNum = slider.value;
            }

        }
        ColumnLayout {
            CustomButton {

                id: bookmark
                contentItem: Image {
                    id: book
                    source: "icons/bookmark.png"
                    width: 20
                    height: 20

            }
                function saveBookmark() {
                    showImage.bookmark = showImage.pageNum // la page courante à sauvegarder
                }
                onClicked: saveBookmark()


        }
            CustomButton {
                id: bookmark2
                contentItem: Image {
                    id: book2
                    source: "icons/next.jpg"
                    width: 20
                    height: 20

                }
                function restoreBookmark() {
                    showImage.pageNum = showImage.bookmark
                }
                onClicked: restoreBookmark()
            }
        }
        ToolBar {
            id: bottomBar
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 40
            background: Rectangle {
                        color: "#ffffff"
                    }
            RowLayout {
                anchors.rightMargin: 0
                anchors.fill: parent

                CustomButton {
                    contentItem: Image {
                                    id: ouvrir
                                    source: "icons/dossier.png"
                                    width: 20
                                    height: 20
                                    anchors.top: parent.top
                                  }
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                        FileDialog {
                            id: fileDialog
                            objectName: "FileDialog"
                            title: "Choisir un fichier"
                            nameFilters: [ "cbr cbz files (*.cbr *.cbz)", "Image files (*.png *.jpg *.bmp)","All files (*)",  ]
                            signal setFilePathSignal(string msg)
                            onAccepted: {
                                setFilePathSignal(fileDialog.currentFile)
                            }

                        }
                        onClicked: fileDialog.open()
                    }

                    CustomButton {
                        id: buttonPageDown
                        function pageDown(){
                            if(showImage2.isShow == false)   showImage.pageNum -=1
                            else showImage.pageNum -=2
                        }

                        onClicked: pageDown()
                        contentItem: Image {
                            id: pagedown
                            source: "icons/back.png"
                            width: 20
                            height: 20
                            anchors.top: parent.top
                        }
                    }

                    CustomButton {
                        id: buttonPageUp
                        function pageUp(){
                            if(showImage2.isShow == false)   showImage.pageNum +=1
                            else showImage.pageNum +=2
                        }

                        onClicked: {
                            pageUp()
                        }
                        contentItem: Image {
                            id: pageup
                            source: "icons/forward.png"
                            width: 20
                            height: 20
                            anchors.top: parent.top
                        }
                }

                CustomButton {
                    id: buttonZoomIn
                    contentItem: Image {
                        id: zoomin
                        source: "icons/zoomiconin.png"
                        width: 20
                        height: 20
                        anchors.top: parent.top
                    }
                }

                CustomButton {
                    id: buttonZoomOut
                    contentItem: Image {
                        id: zoomout
                        source: "icons/zoomiconout.png"
                        width: 20
                        height: 20
                        anchors.top: parent.top
                    }
                }
                CustomButton {
                    id: buttonTwoPageid
                    contentItem: Image {
                        id: two
                        source: "icons/two.png"
                        width: 20
                        height: 20
                        anchors.top: parent.top
                    }
                }
                TextField {
                        id: pageInput
                        placeholderText: "Page number"
                        width: 100
                        height: 30
                        font.pointSize: 14
                        validator: IntValidator {
                            bottom: 1
                        }
                    }
                CustomButton {
                         contentItem: Image {
                             id: goIcon
                             source: "icons/go.png"
                             width: 20
                             height: 20
                             anchors.top: parent.top
                         }
                         function navigateToPage(){
                             showImage.pageNum = pageInput.text;
                         }
                         onClicked: navigateToPage()
                     }

                }
            }
        }
    }

