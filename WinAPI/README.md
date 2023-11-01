# WinAPI #

--------------------

> ℹ️ *Linux kompilácia sa dá cez [Winelib](https://wiki.winehq.org/Winelib_User's_Guide#Getting_Started) – nainštalujte si `wine-devel` (alebo ekvivalentný package) a pretvorte Makefile cez `winemaker`:*
> 
> ```sh
> winemaker --lower-uppercase .
> ```

--------------------

## Prezentace ##

[Prezentace](ZADANI.md)

## Spuštění šablony ##

- Nabootujte do Windows
- Stáhněte si [šablonu](https://moodle.vut.cz/pluginfile.php/597571/mod_page/content/33/WinAPI.zip), rozbalte a otevřete WinApi.vcxproj pomocí Visual Studio.
- V souboru **`main.c`** je připravena šablona. Po překladu a spuštění se otevře prázdné okno.

## Princip aplikace ve WinAPI ##

Systém Windows je založen na principu zasílání zpráv. Zprávy slouží např. k informaci aplikace o stisku klávesy, posuny myši, změně velikosti okna atd. Každá zpráva se skládá z identifikátoru a dvou parametrů. Klíčovou částí aplikace používající pouze WinAPI je **hlavní smyčka aplikace**, která zabezpečuje příjem a zpracování jednotlivých zpráv zaslaných aplikaci. Tato smyčka bývá zpravidla umístěna ve funkce **WinMain**, která se volá při spuštění okenní aplikace. Zde se také provádí **inicializace aplikace a tvorba/zobrazení hlavního okna**. Všimněte si parametru **lpfnWndProc**, kde definujeme název funkce (**MainWndProc**), která bude obsluhovat události týkajících se okna.

```c
int WINAPI WinMain()
{
    WNDCLASS wcx; 
    wcx.lpfnWndProc = (WNDPROC) MainWndProc;
    wcx.hbrBackground = GetStockObject(WHITE_BRUSH); 
    … 
    RegisterClass(&wcx);
    HWND hWnd = CreateWindow("ITU", 200, 25, 1500, 950, …);
    …
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    …

    while((bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
    {
        if (bRet == -1)
        {
            // handle the error and possibly exit
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}
```

### Zpracování zpráv ###

Důležitou částí aplikace je funkce zpracovávající jednotlivé zprávy doručené danému oknu aplikace. V ukázkové aplikaci je to funkce **MainWndProc**. Můžete zde vidět větvení do příslušných case, podle doručené události a volání funkce DefWindowProc zabezpečující standardní zpracování ostatních událostí, které nejsou zpracovávány ve vlastní režii aplikace. Nastavení této funkce pro hlavní okno se provádí při registraci třídy okna ve funkci **WinMain**. Základní typy událostí jsou uvedené v ukázkové aplikaci.

```c
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case MESSAGE_TYPE: 
            /*process message*/

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
```

### Vykreslování do okna ###

Vykreslování do okna se provádí v rámci obsluhy události **WM_PAINT**. Tato událost se nevyvolá automaticky, ale nepřímo pomocí funkce [InvalidateRect](http://msdn.microsoft.com/en-us/library/windows/desktop/dd145002%28v=vs.85%29.aspx). Tato funkce přidá oblast (rectangle) do update region okna. Systém Windows pošle zprávu **WM_PAINT** kdykoliv je update region okna neprázdný a žádné jiné zprávy nečekají na zpracování.

```c
InvalidateRect(hWnd, lpRect, bErase)
```

- `hWnd` je *handle* okna
- `lpRect` je ukazatel na strukturu [RECT](https://docs.microsoft.com/en-us/windows/win32/api/windef/ns-windef-rect), která specifikuje koordináty oblasti, která má být přidána do *update region* okna
- `bErase` specifikuje, zda se pozadí v rámci *update region* smaže (překreslý)

Funkce pro vykreslování objektů: [Rectangle](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-rectangle), [Ellipse](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-ellipse), [MoveToEx](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-movetoex), [LineTo](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-lineto), [Polygon](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-polygon)<br />
Funkce pro nastavení barev: [CreatePen](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createpen), [GetStockObject](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-getstockobject), [SelectObject](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-selectobject), [DeleteObject](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject)<br />
Funkce pro práci s pozicí kurzoru myši: [GetCursorPos](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getcursorpos), [ScreenToClient](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-screentoclient)<br />

Ukázky použití funkcí jsou v přiložené prezentaci.

## Body ##

- Vykreslete několik základních objektů (čáry, obdelníky. elipsy, polygony, ...) – 1
- Implementujte pohyb některých objektů (využijte reakci na klávesnici nebo myš) – 1
- Implementujte změnu barev něktrerých objektů (využijte reakci na klávesnici nebo myš)\* – 1
- Vykreslete složitější objekt sestávající s více jednoduchých, který reaguje na klávesnici a myš (auto, panáček, abstrakt, apod.) – 1
- Vysvětlete princip fungování aplikace ve WinAPI (zpracování zpráv, jak dojde k vykreslení do okna, co dělá funkce InvalidateRect, ...) – 1

\* Pokud pohybujete objektem pomocí myši, implementujte změnu barev pomocí kláves a naopak.

--------------------

Naposledy změněno: pondělí, 10. října 2022, 13.27
