import QtQuick 2.1
import QtQuick.Window 2.0
import "../js/Theme.js" as Theme
import cz.vutbr.fit 1.0

Window {
     visible: true
     width: 400
     height: 315 + lutdisp.height

     maximumWidth: 400
     maximumHeight: 315 + lutdisp.height
     minimumWidth: 400
     minimumHeight: 315 + lutdisp.height

     title: "ITU - Qt 5 / QML kalkulačka"

     // Definování datového modelu s operátory
     // 'op' - zkratka pro operaci
     // 'tog' - zkratka pro toggled, označení, která operace je vybrána
     ListModel {
          id: operations;
          ListElement { op: "+"; tog: true; }
          ListElement { op: "-"; tog: false; }
          // TODO: Rozšiřte model o další dvě základní matematické operace
          ListElement { op: "×"; tog: false; }
          ListElement { op: "÷"; tog: false; }
     }

     // Prvek pro rozvržení prvků do sloupce
     // http://en.wikipedia.org/wiki/Layout_%28computing%29
     // https://qmlbook.github.io/ch04-qmlstart/qmlstart.html#positioning-elements
     Column {
          // Vstupní hodnota - první operand výpočtu
          Rectangle {
               id: input;
               height: 35;
               width: 400;
               border.color: "#bbb";
               border.width: 3;
               anchors.margins: 2;
               color: Theme.in_colour;

               TextInput {
                    anchors.fill: parent;
                    anchors.margins: 2;
                    horizontalAlignment: TextInput.AlignLeft;
                    verticalAlignment: TextInput.AlignVCenter;
                    id: textA;
                    font.pointSize: 16;
                    text: "0";

                    onTextChanged: {
                         input.color = Theme.in_colour;
                    }
               }
          }

          // Prvek pro rozvržení prvků do řádku
          // Více jak prvek Column (výše)
          Row {
               // Obdoba ListView (ale více obecný) nebo funkce foreach()
               // obsahuje _model_ a _delegate_
               Repeater {
                    // Definování modelu, data pro zobrazení
                    model: operations;

                    // Delegování vzhledu v MVC
                    // Jak má vypadat jeden prvek
                    // @disable-check M301
                    delegate: MyButton {
                         btnColor: Theme.btn_colour
                         text: model.op
                         toggled: model.tog;

                         onClicked: {
                              for (var i = 0; i < operations.count; i++) {
                                   // @disable-check M325
                                   operations.setProperty( i, "tog", (i == index) );
                              }
                         }
                    }
               }
          }

          // "Vlastní" třída pro posuvník. Definice v MySlider.qml
          // @disable-check M301
          MySlider {
               id: slider
               color: Qt.darker(Theme.slider_color)
               rectColor: Theme.slider_color

               onValueChanged: {
                    lutdisp.pos = slider.value;

                    if (slider.rectColor === Qt.color("red")) {
                         slider.rectColor = Theme.slider_color;
                         slider.color = Qt.darker(Theme.slider_color);
                    }
               }
          }

          // TODO: vložte nový textový prvek, který bude bude vizuálně 'zapadat'
          // do výsledné aplikace a bude zobrazoval vertikálně vycentrovaný
          // text 'LUT value: ' a hodnotu aktuálně vybrané položky z LUT
          // @disable-check M301
          MyLUTDisplay {
               id: lutdisp;
               value: lut.getValue(slider.value);
          }

          // Vlastní klikací tlačítko. Definice v MyClickButton.qml
          // @disable-check M301
          MyClickButton {
               width: 400;
               btnColor: Theme.btn_colour
               btnColorClicked: Theme.btn_colour_clicked
               text: qsTr( "Compute" )

               function getOperation() {
                    for (var i = 0; i < operations.count; i++) {
                         var item = operations.get(i);
                         if (item.tog) {
                              return item.op;
                         }
                    }
                    return "+";
               }

               // Obsluha události při zachycení signálu clicked
               onClicked: {
                    var a = Number(textA.text, 10);
                    var op = getOperation();

                    // TODO: Upravte načtení hodnoty b tak, aby získal hodnotu b
                    // z LUT (Look Up Table) podle vybrané hodnoty na 'slider'
                    var b = lut.getValue(slider.value);

                    function errorInput() {
                         input.color = Qt.darker("red");
                         result.color = "red";
                         result.text = "SyntaxError";
                         console.log(NaN + " " + op + " " + b + " = NaN")
                    }

                    function errorSlider() {
                         slider.rectColor = "red";
                         slider.color = Qt.darker("red");
                         result.color = "red";
                         result.text = "MathError";
                         console.log(a + " " + op + " " + b + " = ∞")
                    }

                    // TODO: Zkontrolujte jestli funkce parseFloat vrátila
                    // korektní výsledek (tj. ne NaN, ale číslo). Pokud
                    // je hodnota a NaN, změňte barvu vstupního pole
                    // na červenou a vypište chybu do pole pro výsledek
                    if (isNaN(a))
                         return errorInput();

                    // TODO: Pokud se uživatel pokouší dělit nulou, změňte barvu
                    // posuvníku na slideru na červenou a vypište chybu
                    // do pole pro výsledek
                    if (b === 0 && getOperation() === "÷")
                         return errorSlider();

                    // TODO: Vypočítejte výslednou hodnotu danou operandy a, b
                    // a operátorem op, výsledek uložte do prvku result
                    result.color = "#0066FF";
                    switch (op) {
                    case "+":
                         result.text = a + b;
                         break;
                    case "-":
                         result.text = a - b;
                         break;
                    case "×":
                         result.text = a * b;
                         break;
                    case "÷":
                         result.text = a / b;
                         break;
                    }

                    console.log(a + " " + op + " " + b + " = " + result.text)
               }
          }

          // Prvek pro zobrazení výsledné hodnoty
          Rectangle {
               height: 45;
               width: 400;
               border.color: "#bbb";
               border.width: 3;
               anchors.margins: 2
               color: "#777"

               Text {
                    id: result;
                    anchors.centerIn: parent
                    height: 35;
                    font.pointSize: 22
                    color: "#0066FF"
               }
          }
     }

     // Vytvoření objektu LUT, který je definován v C++
     // K danému se přistupuje pomocí jeho id
     LUT {
          id: lut
     }
}

