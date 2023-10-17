import QtQuick 2.0

// Základem každého prvku je čtverec
Rectangle {
     id: btn;
     width: 100
     height: 100

     // Proměnné, které mají parametr property jsou přístupné zvenčí
     property alias text: txt.text;
     property color btnColor: "#777777";
     property color btnColorClicked: "#777777";
     property color textColor: mouse.pressed ? "#0066FF" : "black"

     border.color: "#bbbbbb";
     border.width: 3;

     // TODO: Upravte nastavení hodnoty color tak,
     // aby při stisknutí myši se změnila barva tlačítka
     color: mouse.pressed ? btnColorClicked : btnColor;

     // Definování signálu
     signal clicked();

     // Samotná třída Rectangle nijak nezachytává signály
     // ze vstupních zařízení. Pro jejich zachytávání a zpracování
     // je potřeba vložit třídy k tomu určené.
     // Oblast zachytávající události myši
     // Lze použít oblushu události (onClicked, atd.)
     // uvnitř i mimo tento prvek, pak je zapotřebí použít
     // referenci přes id prvku. Lze i přistupovat k vlastnostem
     // objektu (vnitřní hodnoty), např. mouse.pressed (viz výše).
     MouseArea {
          id: mouse;
          anchors.fill: parent;
          onClicked: {
               btn.clicked();
          }
     }

     // Textový prvek pro zobrazení textu
     Text {
          id: txt;
          
          // TODO: Nastavte parametry tohoto prvku tak,
          // aby výsledný text byl zarovnán na střed tlačítka
          // a měl vhodné formátování
          anchors.centerIn: parent;
          color: textColor;
          font.pointSize: 18;
          font.bold: true;
     }
}
